/* Copyright (C) 2024 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License Version 2 for
 * details.

 * You may distribute and modify this code under the terms of the GNU General
 * Public License Version 2, which you should have received a copy of along with
 * this program.  If not, please go to <https://www.gnu.org/licenses/>.

 * Created: 2024-07-26
 * Author: Aryadev Chavali
 * Description: Implementation of numerics
 */

#include "./numerics.hpp"

#include <sstream>

Fraction::Fraction(word_t numerator, word_t denominator)
    : numerator{numerator}, denominator{denominator},
      norm{numerator / ((long double)denominator)}
{
  word_t hcf = gcd(MIN(numerator, denominator), MAX(numerator, denominator));
  numerator /= hcf;
  denominator /= hcf;
}

// floating point arithmetic inaccuracies blah blah blah better to use
// simplified fractions here

bool Fraction::operator<(const Fraction other)
{
  if (other.denominator == denominator)
    return numerator < other.numerator;
  // TODO: Is it better to use the GCD?
  return (numerator * other.denominator) < (other.numerator * denominator);
}

bool Fraction::operator==(const Fraction &other)
{
  return numerator == other.numerator && denominator == other.denominator;
}

Node::Node(Fraction val, int64_t left, int64_t right)
    : value{val}, left{left}, right{right}
{
}

NodeAllocator::NodeAllocator(word_t capacity) : vec{capacity}
{
}

word_t NodeAllocator::alloc_node(Node n)
{
  vec.push_back(n);
  return vec.size() - 1;
}

word_t gcd(word_t a, word_t b)
{
  if (a == b)
    return a;
  else if (a <= 1 || b <= 1)
    return 1;
  for (word_t r = b % a; r != 0; b = a, a = r, r = b % a)
    continue;
  return a;
}

Fraction iterate(std::queue<word_t> &queue, NodeAllocator &allocator)
{
  if (queue.empty())
    return {};
  word_t index = queue.front();
  Node node    = allocator.vec[index];
  if (node.left == -1)
  {
    allocator.vec[index].left = allocator.alloc_node(Fraction{
        node.value.numerator, node.value.numerator + node.value.denominator});
  }
  if (node.right == -1)
  {
    allocator.vec[index].right = allocator.alloc_node(Fraction{
        node.value.numerator + node.value.denominator, node.value.denominator});
  }
  queue.pop();
  queue.push(allocator.vec[index].left);
  queue.push(allocator.vec[index].right);
  node          = allocator.vec[index];
  Fraction best = MAX(node.value, allocator.vec[node.left].value);
  best          = MAX(best, allocator.vec[node.right].value);
  return best;
}

std::string to_string(const Fraction &f)
{
  std::stringstream ss;
  ss << f.numerator << "/" << f.denominator;
  return ss.str();
}

void indent_depth(int depth, std::stringstream &ss)
{
  for (int i = 0; i < depth; ++i)
    ss << "  ";
}

std::string to_string(const NodeAllocator &allocator, const word_t n, int depth)
{
  std::stringstream ss;
  Node x = allocator.vec[n];
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
