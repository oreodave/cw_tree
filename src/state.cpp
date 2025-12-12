/* state.cpp:
 * Created: 2025-11-27
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include "state.hpp"
#include <cmath>

namespace cw::state
{
  void DrawState::compute_bounds()
  {
    state.mutex.lock();
    bounds.leftmost = state.allocator.get_val(0);
    while (bounds.leftmost.left >= 0)
      bounds.leftmost = state.allocator.get_val(bounds.leftmost.left);

    bounds.rightmost = state.allocator.get_val(0);
    while (bounds.rightmost.right >= 0)
      bounds.rightmost = state.allocator.get_val(bounds.rightmost.right);
    state.mutex.unlock();

    bounds.lower_val = std::floorl(bounds.leftmost.value.norm);
    bounds.upper_val = std::ceill(bounds.rightmost.value.norm);
  }
} // namespace cw::state

/* Copyright (C) 2025 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License Version 2 for
 * details.

 * You may distribute and modify this code under the terms of the GNU General
 * Public License Version 2, which you should have received a copy of along with
 * this program.  If not, please go to <https://www.gnu.org/licenses/>.

 */
