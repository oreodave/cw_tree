/* main.cpp: Entrypoint
 * Created: 2025-11-20
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary: 2024-07-25
 */

#include <chrono>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <stack>
#include <thread>
#include <tuple>

#include <raylib.h>
#include <raymath.h>

#include "base.hpp"
#include "node.hpp"
#include "worker.hpp"

#define WIDTH       1024
#define HEIGHT      800
#define FONT_SIZE   20
#define CIRCLE_SIZE 2
#define LINE_TOP    (7 * HEIGHT / 16)
#define LINE_BOTTOM (9 * HEIGHT / 16)
#define N_THREADS   15

using cw::state::DrawState;
using cw::state::State;

std::pair<std::string, int> fraction_to_string(cw::node::Fraction f)
{
  std::string s{to_string(f)};
  int width = MeasureText(s.c_str(), FONT_SIZE);
  return std::make_pair(s, width);
}

void draw_fraction(cw::node::Fraction f, u64 x, u64 y)
{
  std::string s;
  int width;
  std::tie(s, width) = fraction_to_string(f);
  // Centered at (x, y)
  DrawText(s.c_str(), x - width / 2, y - FONT_SIZE, FONT_SIZE, WHITE);
}

void draw_tree(DrawState &ds, State &state)
{
  // Number line
  DrawLine(0, HEIGHT / 2, WIDTH, HEIGHT / 2, WHITE);

  // Bounds
  state.mutex.lock();
  std::stack<std::pair<u64, f64>> stack;
  cw::node::Node n = state.allocator.get_val(0);
  stack.push(std::make_pair(0, n.value.norm));
  while (!stack.empty())
  {
    u64 node_index;
    f64 norm;
    std::tie(node_index, norm) = stack.top();
    stack.pop();
    u64 x = Remap(norm, ds.bounds.lower_val, ds.bounds.upper_val, 0, WIDTH);
    DrawLine(x, LINE_TOP, x, LINE_BOTTOM, RED);

    cw::node::Node n = state.allocator.get_val(node_index);
    if (n.left >= 0)
    {
      cw::node::Node left = state.allocator.get_val(n.left);
      stack.push(std::make_pair(n.left, left.value.norm));
    }
    if (n.right >= 0)
    {
      cw::node::Node right = state.allocator.get_val(n.right);
      stack.push(std::make_pair(n.right, right.value.norm));
    }
  }
  state.mutex.unlock();

  DrawLine(0, LINE_TOP, 0, LINE_BOTTOM, WHITE);
  DrawText("0", 0, LINE_TOP - FONT_SIZE, FONT_SIZE, WHITE);
  DrawLine(WIDTH, LINE_TOP, WIDTH, LINE_BOTTOM, WHITE);
  char buffer[64];
  sprintf(buffer, "%d", (int)ds.bounds.upper_val);
  DrawText(buffer, WIDTH - (FONT_SIZE / 2), LINE_TOP - FONT_SIZE, FONT_SIZE,
           WHITE);
}

using Clock = std::chrono::steady_clock;
using Ms    = std::chrono::milliseconds;

int main(void)
{
  // Init timer
  auto time_current         = Clock::now();
  auto time_previous        = time_current;
  constexpr auto time_delta = 0;

  // Init general state
  cw::state::State state;
  state.stop_work  = false;
  state.pause_work = false;
  state.allocator.alloc(cw::node::Node{{1, 1}, -1, -1});
  state.queue.push(0);

  cw::state::DrawState draw_state{state};

  // Init meta text (counter, iterations, etc)
  u64 count = 1, prev_count = 0;
  std::stringstream format_stream;
  std::string format_str;
  u64 format_str_width = 0;

  // Init threads
  std::thread threads[N_THREADS];
  for (auto i = 0; i < N_THREADS; ++i)
  {
    threads[i] = std::move(std::thread(cw::worker::worker, std::ref(state)));
  }

  // Setup raylib window
  InitWindow(WIDTH, HEIGHT, "Calkin-Wilf tree");
  SetTargetFPS(60);

  // setup camera
  Camera2D camera;
  camera.target   = {.x = 0, .y = 0};
  camera.offset   = {.x = WIDTH / 16, .y = 0};
  camera.rotation = 0.0f;
  camera.zoom     = 0.8f;

  while (!WindowShouldClose())
  {
    // Update
    time_current = Clock::now();
    if (!state.pause_work &&
        std::chrono::duration_cast<Ms>(time_current - time_previous).count() >=
            time_delta)
    {
      time_previous = time_current;
      count         = state.allocator.vec.size();
    }

    if (prev_count != count)
    {
      draw_state.compute_bounds();
      prev_count = count;
      format_stream << "Count=" << count << "\n\n"
                    << "Iterations=" << (count - 1) / 2 << "\n\n"
                    << "Lower=" << to_string(draw_state.bounds.leftmost.value)
                    << "\n\n"
                    << "Upper=" << to_string(draw_state.bounds.rightmost.value);
      format_str = format_stream.str();
      format_stream.str("");
      format_str_width = MeasureText(format_str.c_str(), FONT_SIZE * 2);
    }

    if (IsKeyPressed(KEY_SPACE))
      state.pause_work = !state.pause_work;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
      Vector2 delta = GetMouseDelta();
      delta         = Vector2Scale(delta, -1.0f / camera.zoom);
      camera.target = Vector2Add(camera.target, delta);
    }
    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
      Vector2 mouse_to_world = GetScreenToWorld2D(GetMousePosition(), camera);
      camera.offset          = GetMousePosition();
      camera.target          = mouse_to_world;
      camera.zoom            = Clamp(camera.zoom + (wheel * 0.1f), 0.125, 64);
      printf("%lf\n", camera.zoom);
    }

    // Draw

    ClearBackground(BLACK);
    BeginDrawing();
    BeginMode2D(camera);
    draw_tree(draw_state, state);
    EndMode2D();
    DrawText(format_str.c_str(), (31 * WIDTH / 32) - format_str_width / 2,
             HEIGHT / 32, FONT_SIZE, WHITE);
    EndDrawing();
  }

  CloseWindow();

  state.stop_work = true;
  for (auto &thread : threads)
  {
    thread.join();
  }
  return 0;
}

/* Copyright (C) 2024, 2025 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License Version 2 for
 * details.

 * You may distribute and modify this code under the terms of the GNU General
 * Public License Version 2, which you should have received a copy of along with
 * this program.  If not, please go to <https://www.gnu.org/licenses/>.

 */
