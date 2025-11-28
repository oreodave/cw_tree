/* worker.cpp: Implementation of workers in our simulation
 * Created: 2025-11-27
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include <chrono>
#include <thread>
#include <tuple>

#include "worker.hpp"

namespace cw::worker
{
  using cw::node::Fraction;
  using cw::node::Node;

  std::tuple<u64, u64> generate_children(NodeAllocator &allocator, u64 index)
  {
    Node node = allocator.get_val(index);
    if (node.left < 0)
    {
      allocator.get_ref(index).left = allocator.alloc(Fraction{
          node.value.numerator, node.value.numerator + node.value.denominator});
    }
    if (node.right < 0)
    {
      allocator.get_ref(index).right = allocator.alloc(
          Fraction{node.value.numerator + node.value.denominator,
                   node.value.denominator});
    }
    return {node.left, node.right};
  }

  void do_iteration(State &state)
  {
    state.mutex_queue.lock();
    if (state.queue.empty())
    {
      // Unlock since there isn't any work to be done.
      state.mutex_queue.unlock();
      return;
    }
    u64 index = state.queue.front();
    state.queue.pop();
    state.mutex_queue.unlock();

    u64 left_child, right_child;
    state.mutex_allocator.lock();
    std::tie(left_child, right_child) =
        generate_children(state.allocator, index);
    state.mutex_allocator.unlock();

    state.mutex_queue.lock();
    state.queue.push(left_child);
    state.queue.push(right_child);
    state.mutex_queue.unlock();
  }

  void worker(State &state)
  {
    while (!state.stop_work)
    {
      std::this_thread::sleep_for(THREAD_GENERAL_DELAY);
      while (state.pause_work)
      {
        std::this_thread::sleep_for(THREAD_PAUSE_DELAY);
      }

      do_iteration(state);
    }
  }
} // namespace cw::worker

/* Copyright (C) 2025 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License Version 2 for
 * details.

 * You may distribute and modify this code under the terms of the GNU General
 * Public License Version 2, which you should have received a copy of along with
 * this program.  If not, please go to <https://www.gnu.org/licenses/>.

 */
