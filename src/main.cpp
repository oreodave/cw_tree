/* Copyright (C) 2024 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License Version 2 for
 * details.

 * You may distribute and modify this code under the terms of the GNU General
 * Public License Version 2, which you should have received a copy of along with
 * this program.  If not, please go to <https://www.gnu.org/licenses/>.

 * Created: 2024-07-25
 * Author: Aryadev Chavali
 * Description: Entrypoint
 */

#include "./numerics.hpp"

#include <cmath>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <stack>
#include <tuple>

#include <raylib.h>

#define WIDTH       1024
#define HEIGHT      1024
#define FONT_SIZE   20
#define CIRCLE_SIZE 2
#define LINE_TOP    (7 * HEIGHT / 16)
#define LINE_BOTTOM (9 * HEIGHT / 16)

void draw_fraction(Fraction f, word_t x, word_t y)
{
  std::string s{to_string(f)};
  // Centered at (x, y)
  int width = MeasureText(s.c_str(), FONT_SIZE);
  DrawText(s.c_str(), x - width / 2, y - FONT_SIZE, FONT_SIZE, WHITE);
}

struct State
{
  NodeAllocator allocator;
  std::queue<word_t> iteration_queue;
  word_t root;
  Node leftmost, rightmost;
  long double lower_bound, upper_bound;

  State(const Fraction start) : allocator{256}
  {
    root = allocator.alloc(start);
    iteration_queue.push(root);
    leftmost  = allocator.getVal(root);
    rightmost = allocator.getVal(root);
    compute_bounds();
  }

  void compute_bounds()
  {
    lower_bound = std::floorl(leftmost.value.norm);
    upper_bound = std::ceill(rightmost.value.norm);
  }

  void compute_bound_nodes()
  {
    leftmost = allocator.getVal(0);
    while (leftmost.left.has_value())
      leftmost = allocator.getVal(leftmost.left.value());

    rightmost = allocator.getVal(0);
    while (rightmost.right.has_value())
      rightmost = allocator.getVal(rightmost.right.value());
  }

  constexpr word_t clamp_to_width(long double value)
  {
    return WIDTH / (upper_bound - lower_bound) * (value - lower_bound);
  }

  void draw_bounds()
  {
    word_t lower_x = clamp_to_width(leftmost.value.norm);
    word_t upper_x = clamp_to_width(rightmost.value.norm);
    draw_fraction(leftmost.value, lower_x, 3 * HEIGHT / 8);
    draw_fraction(rightmost.value, upper_x, 3 * HEIGHT / 8);
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
};

int main(void)
{
  // Setup state
  State state{{1, 2}};

  // Setup meta text (counter, iterations, etc)
  word_t count = 1, prev_count = 0;
  std::stringstream format_stream;
  std::string format_str;
  word_t format_str_width = 0;

  InitWindow(WIDTH, HEIGHT, "Calkin-Wilf Tree");
  while (!WindowShouldClose())
  {
    if (IsKeyPressed(KEY_SPACE))
    {
      iterate(state.iteration_queue, state.allocator);
      state.compute_bound_nodes();
      state.compute_bounds();
      count += 2;
    }
    if (prev_count != count)
    {
      prev_count = count;
      format_stream << "Count=" << count << "\n\n";
      format_stream << "Iterations=" << (count - 1) / 2;
      format_str = format_stream.str();
      format_stream.str("");
      format_str_width = MeasureText(format_str.c_str(), FONT_SIZE * 2);
    }

    ClearBackground(BLACK);
    BeginDrawing();
    DrawLine(0, HEIGHT / 2, WIDTH, HEIGHT / 2, WHITE);
    state.draw_nodes();
    state.draw_bounds();
    DrawText(format_str.c_str(), WIDTH / 2 - format_str_width / 2,
             LINE_TOP - HEIGHT / 4, FONT_SIZE * 2, WHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
