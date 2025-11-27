/* state.hpp: General state of the simulation
 * Created: 2025-11-27
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#ifndef STATE_HPP
#define STATE_HPP

#include <mutex>
#include <queue>

#include "base.hpp"
#include "node.hpp"

namespace cw::state
{
  struct State
  {
    cw::node::NodeAllocator allocator;
    std::queue<u64> queue;

    std::mutex mutex_queue;
    std::mutex mutex_allocator;

    State(void);
  };

  struct DrawState
  {
    const State &state;
    struct Bounds
    {
      cw::node::Node leftmost, rightmost;
      f64 lower_val, upper_val;
    } bounds;

    DrawState(const State &);

    void compute_bounds(void);
  };
} // namespace cw::state

#endif

/* Copyright (C) 2025 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License Version 2 for
 * details.

 * You may distribute and modify this code under the terms of the GNU General
 * Public License Version 2, which you should have received a copy of along with
 * this program.  If not, please go to <https://www.gnu.org/licenses/>.

 */
