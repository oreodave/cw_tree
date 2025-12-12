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

  void do_iteration(State &state)
  {
    state.mutex.lock();
    if (state.queue.empty())
    {
      // Unlock since there isn't any work to be done.
      state.mutex.unlock();
      return;
    }
    u64 index = state.queue.front();
    state.queue.pop();

    Node node = state.allocator.get_val(index);

    i64 left = node.left, right = node.right;
    if (left < 0)
    {
      left = state.allocator.alloc(Fraction{
          node.value.numerator, node.value.numerator + node.value.denominator});
    }
    if (right < 0)
    {
      right = state.allocator.alloc(
          Fraction{node.value.numerator + node.value.denominator,
                   node.value.denominator});
    }

    Node &node_ref = state.allocator.get_ref(index);
    node_ref.left  = left;
    node_ref.right = right;

    state.queue.push(left);
    state.queue.push(right);
    state.mutex.unlock();
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
