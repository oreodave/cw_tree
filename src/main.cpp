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

#include "base.hpp"
#include "node.hpp"
#include "worker.hpp"

#define WIDTH       1024
#define HEIGHT      1024
#define FONT_SIZE   20
#define CIRCLE_SIZE 2
#define LINE_TOP    (7 * HEIGHT / 16)
#define LINE_BOTTOM (9 * HEIGHT / 16)

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

constexpr u64 clamp_to_width(const DrawState &ds, f64 val)
{
  return (WIDTH / (ds.bounds.upper_val - ds.bounds.lower_val)) *
         (val - ds.bounds.lower_val);
}

void draw_tree(DrawState &ds, State &state)
{
  // Number line
  DrawLine(0, HEIGHT / 2, WIDTH, HEIGHT / 2, WHITE);

  // Bounds
  u64 lower_x = clamp_to_width(ds, ds.bounds.leftmost.value.norm);
  u64 upper_x = clamp_to_width(ds, ds.bounds.rightmost.value.norm);
  DrawLine(lower_x, LINE_TOP, lower_x, LINE_BOTTOM, WHITE);
  DrawLine(upper_x, LINE_TOP, upper_x, LINE_BOTTOM, WHITE);

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
    u64 x = clamp_to_width(ds, norm);
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
#define THREADS 15
  std::thread threads[THREADS];
  for (auto i = 0; i < THREADS; ++i)
  {
    threads[i] = std::move(std::thread(cw::worker::worker, std::ref(state)));
  }

  // Setup raylib window
  InitWindow(WIDTH, HEIGHT, "Calkin-Wilf tree");
  SetTargetFPS(60);

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
    {
      state.pause_work = !state.pause_work;
    }

    // Draw

    ClearBackground(BLACK);
    BeginDrawing();
    draw_tree(draw_state, state);
    DrawText(format_str.c_str(), WIDTH / 2 - format_str_width / 2, HEIGHT / 8,
             FONT_SIZE, WHITE);
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

#if 0
struct State
{
  NodeAllocator allocator;
  std::queue<word_t> iteration_queue;
  word_t root;

  struct Bounds
  {
    Node leftmost, rightmost;
    long double lower, upper;
  } bounds;

  struct Iteration
  {
    Fraction left, centre, right;
  } iteration;

  State(const Fraction start) : allocator{256}
  {
    root = allocator.alloc(start);
    iteration_queue.push(root);
    bounds.leftmost  = allocator.getVal(root);
    bounds.rightmost = allocator.getVal(root);
    compute_bounds();
  }

  void do_iteration(void)
  {
    std::tie(iteration.left, iteration.centre, iteration.right) =
        iterate(iteration_queue, allocator);
    compute_bound_nodes();
    compute_bounds();
  }

  void compute_bounds()
  {
    bounds.lower = std::floorl(bounds.leftmost.value.norm);
    bounds.upper = std::ceill(bounds.rightmost.value.norm);
  }

  void compute_bound_nodes()
  {
    bounds.leftmost = allocator.getVal(0);
    while (bounds.leftmost.left.has_value())
      bounds.leftmost = allocator.getVal(bounds.leftmost.left.value());

    bounds.rightmost = allocator.getVal(0);
    while (bounds.rightmost.right.has_value())
      bounds.rightmost = allocator.getVal(bounds.rightmost.right.value());
  }

  constexpr word_t clamp_to_width(long double value)
  {
    return (WIDTH / (bounds.upper - bounds.lower)) * (value - bounds.lower);
  }

  void draw_bounds()
  {
    word_t lower_x = clamp_to_width(bounds.leftmost.value.norm);
    word_t upper_x = clamp_to_width(bounds.rightmost.value.norm);
    DrawLine(lower_x, LINE_TOP, lower_x, LINE_BOTTOM, WHITE);
    DrawLine(upper_x, LINE_TOP, upper_x, LINE_BOTTOM, WHITE);
  }

  void draw_nodes()
  {
    std::stack<Node> stack;
    stack.push(allocator.getVal(0));
    while (!stack.empty())
    {
      Node n = stack.top();
      stack.pop();
      word_t x = clamp_to_width(n.value.norm);
      DrawLine(x, LINE_TOP, x, LINE_BOTTOM, RED);
      if (n.left.has_value())
        stack.push(allocator.getVal(n.left.value()));
      if (n.right.has_value())
        stack.push(allocator.getVal(n.right.value()));
    }
  }

  void draw_iteration_nodes()
  {
    word_t x_left   = clamp_to_width(iteration.left.norm);
    word_t x_centre = clamp_to_width(iteration.centre.norm);
    word_t x_right  = clamp_to_width(iteration.right.norm);
    DrawLine(x_left, LINE_TOP, x_left, LINE_BOTTOM, BLUE);
    DrawLine(x_right, LINE_TOP, x_right, LINE_BOTTOM, BLUE);
    DrawLine(x_centre, LINE_TOP, x_centre, LINE_BOTTOM, GREEN);
  }
};

using Clock = std::chrono::steady_clock;
using Ms    = std::chrono::milliseconds;

int main(void)
{
  // Setup state
  State state{{1, 1}};

  // Setup meta text (counter, iterations, etc)
  word_t count = 1, prev_count = 0;
  std::stringstream format_stream;
  std::string format_str;
  word_t format_str_width = 0;

  // Setup timer
  bool is_playing           = false;
  auto time_current         = Clock::now();
  auto time_previous        = time_current;
  constexpr auto time_delta = 1;

  InitWindow(WIDTH, HEIGHT, "Calkin-Wilf Tree");
  while (!WindowShouldClose())
  {
    // timer logic
    time_current = Clock::now();
    if (is_playing &&
        std::chrono::duration_cast<Ms>(time_current - time_previous).count() >=
            time_delta)
    {
      time_previous = time_current;
      state.do_iteration();
      count += 2;
    }

    // Input logic
    if (IsKeyDown(KEY_SPACE))
    {
      is_playing = true;
    }
    else if (IsKeyUp(KEY_SPACE))
    {
      is_playing = false;
    }

    if (IsKeyPressed(KEY_PERIOD))
    {
      state.do_iteration();
      count += 2;
    }

    // Meta text logic
    if (prev_count != count)
    {
      prev_count = count;
      format_stream << "Count=" << count << "\n\n"
                    << "Iterations=" << (count - 1) / 2 << "\n\n"
                    << "Lower=" << to_string(state.bounds.leftmost.value)
                    << "\n\n"
                    << "Upper=" << to_string(state.bounds.rightmost.value);
      format_str = format_stream.str();
      format_stream.str("");
      format_str_width = MeasureText(format_str.c_str(), FONT_SIZE * 2);
    }

    ClearBackground(BLACK);
    BeginDrawing();
    DrawLine(0, HEIGHT / 2, WIDTH, HEIGHT / 2, WHITE);
    state.draw_nodes();
    state.draw_bounds();
    state.draw_iteration_nodes();
    DrawText(format_str.c_str(), WIDTH / 2 - format_str_width / 2, HEIGHT / 8,
             FONT_SIZE, WHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
#endif

/* Copyright (C) 2024, 2025 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License Version 2 for
 * details.

 * You may distribute and modify this code under the terms of the GNU General
 * Public License Version 2, which you should have received a copy of along with
 * this program.  If not, please go to <https://www.gnu.org/licenses/>.

 */
