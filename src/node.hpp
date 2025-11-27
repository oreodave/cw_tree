/* node.hpp: Definition of fractions and nodes on the Calkin-Wilf tree
 * Created: 2025-11-27
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <vector>

#include "base.hpp"

namespace cw::node
{
  struct Fraction
  {
    u64 numerator, denominator; // always simplified
    f64 norm;

    Fraction(u64 numerator = 0, u64 denominator = 1);

    // Complete ordering on Fractions
    bool operator<(const Fraction other);
    bool operator==(const Fraction &other);
  };

  std::string to_string(const Fraction &);

  struct Node
  {
    Fraction value;
    i64 left, right;

    Node(const Fraction &&val = {}, i64 left = -1, i64 right = -1);
  };

  struct NodeAllocator
  {
    std::vector<Node> vec;

    NodeAllocator(u64 capacity = 256);
    u64 alloc(Node n);
    Node get_val(u64 n) const;
    Node &get_ref(u64 n);
  };

  std::string to_string(const NodeAllocator &, const i64, int depth = 1);
} // namespace cw::node

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
