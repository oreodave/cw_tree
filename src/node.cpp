/* node.cpp: Implementations of various functions for fractions and nodes
 * Created: 2025-11-27
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include <sstream>

#include "node.hpp"

namespace cw::node
{
  /**************************************/
  /*  ___            _   _              */
  /* | __| _ __ _ __| |_(_)___ _ _  ___ */
  /* | _| '_/ _` / _|  _| / _ \ ' \(_-< */
  /* |_||_| \__,_\__|\__|_\___/_||_/__/ */
  /*                                    */
  /**************************************/
  Fraction::Fraction(u64 numerator, u64 denominator)
      : numerator{numerator}, denominator{denominator},
        norm{numerator / ((f64)denominator)}
  {
    u64 hcf = gcd(MIN(numerator, denominator), MAX(numerator, denominator));
    numerator /= hcf;
    denominator /= hcf;
  }

  bool Fraction::operator<(const Fraction other)
  {
    if (other.denominator == denominator)
      return numerator < other.numerator;
    return (numerator * other.denominator) < (other.numerator * denominator);
  }

  bool Fraction::operator==(const Fraction &other)
  {
    return numerator == other.numerator && denominator == other.denominator;
  }

  std::string to_string(const Fraction &f)
  {
    std::stringstream ss;
    ss << f.numerator << "/" << f.denominator;
    return ss.str();
  }

  /***************************/
  /*  _  _         _         */
  /* | \| |___  __| |___ ___ */
  /* | .` / _ \/ _` / -_|_-< */
  /* |_|\_\___/\__,_\___/__/ */
  /*                         */
  /***************************/
  Node::Node(const Fraction &&val, i64 left, i64 right)
      : value{val}, left{left}, right{right}
  {
  }

  NodeAllocator::NodeAllocator(u64 capacity)
  {
    vec.reserve(capacity);
  }

  u64 NodeAllocator::alloc(Node n)
  {
    u64 ind = vec.size();
    vec.push_back(n);
    return ind;
  }

  // FIXME: This is annoying.  DRY?
  Node &NodeAllocator::get_ref(u64 n)
  {
    if (n >= vec.size())
      return vec[0];
    return vec[n];
  }

  Node NodeAllocator::get_val(u64 n) const
  {
    if (n >= vec.size())
      return vec[0];
    return vec[n];
  }

  void indent_depth(int depth, std::stringstream &ss)
  {
    for (int i = 0; i < depth; ++i)
      ss << "  ";
  }

  std::string to_string(const NodeAllocator &allocator, const i64 n, int depth)
  {
    if (n < 0)
      return "NIL";

    std::stringstream ss;
    Node x = allocator.get_val(n);
    ss << "(" << to_string(x.value) << "\n";

    indent_depth(depth, ss);
    if (x.left == -1)
      ss << "NIL";
    else
      ss << to_string(allocator, x.left, depth + 1);
    ss << "\n";

    indent_depth(depth, ss);
    if (x.right == -1)
      ss << "NIL";
    else
      ss << to_string(allocator, x.right, depth + 1);

    ss << ")";
    return ss.str();
  }

} // namespace cw::node

/* Copyright (C) 2025 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License Version 2 for
 * details.

 * You may distribute and modify this code under the terms of the GNU General
 * Public License Version 2, which you should have received a copy of along with
 * this program.  If not, please go to <https://www.gnu.org/licenses/>.

 */
