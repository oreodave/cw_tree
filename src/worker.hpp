/* worker.hpp: Definition of "workers" who will generate nodes on the tree
 * Created: 2025-11-27
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#ifndef WORKER_HPP
#define WORKER_HPP

#include <chrono>
#include <tuple>

#include "state.hpp"

namespace cw::worker
{
  using cw::node::NodeAllocator;
  using cw::state::State;
  constexpr auto THREAD_PAUSE_DELAY   = std::chrono::milliseconds(1000);
  constexpr auto THREAD_GENERAL_DELAY = std::chrono::milliseconds(10);

  // Performs a single iteration which consists of the following:
  // 1) pop an index off the iteration queue
  // 2) generate the children of the node at that index
  // 3) push the indices of the children onto the iteration queue
  // Each step will block on the relevant mutex for the resource (1,3 will block
  // on the queue mutex, 2 will block on the allocator mutex) so is thread safe.
  void do_iteration(State &state);

  // Steady living thread worker which performs iterations.  If state.pause_work
  // is true, thread will pause until otherwise.
  void worker(State &state);
} // namespace cw::worker

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
