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

#include <cstdio>

#include "./numerics.hpp"

int main(void)
{
  NodeAllocator allocator{256};
  std::queue<word_t> to_iterate;
  word_t root = allocator.alloc_node({{1, 2}});
  to_iterate.push(root);
  for (int i = 0; i < 10; ++i)
  {
    iterate(to_iterate, allocator);
    printf("step[%d]:\n%s\n\n", i, to_string(allocator, root).c_str());
  }
  return 0;
}
