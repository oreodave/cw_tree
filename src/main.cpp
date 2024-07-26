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
#include <tuple>

#include <raylib.h>

#define WIDTH       1024
#define HEIGHT      1024
#define FONT_SIZE   10
#define CIRCLE_SIZE 2
#define LINE_TOP    (7 * HEIGHT / 16)
#define LINE_BOTTOM (9 * HEIGHT / 16)


int main(void)
{
  // NodeAllocator allocator{256};
  // std::queue<word_t> to_iterate;
  // Fraction best_frac{1, 2};
  // word_t root = allocator.alloc_node({best_frac});
  // to_iterate.push(root);

  InitWindow(800, 600, "Calkin-Wilf Trees");
  while (!WindowShouldClose())
  {
    ClearBackground(BLACK);
    BeginDrawing();
    DrawText("Hello, world!", 50, 50, 25, WHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
