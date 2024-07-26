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

#include <cstdint>
#include <cstdio>

#include <queue>
#include <sstream>
#include <string>
#include <vector>

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
typedef uint64_t word_t;

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

struct Fraction
{
  word_t numerator, denominator;
  bool is_simplified;

  Fraction(word_t numerator = 0, word_t denominator = 1)
      : numerator{numerator}, denominator{denominator}, is_simplified{false}
  {
    // TODO: Figure out if this is a good idea, or simplifying afterwards
    simplify();
  }

  bool operator<(const Fraction other)
  {
    if (other.denominator == denominator)
      return numerator < other.numerator;
    // TODO: Is it better to use the GCD?
    return (numerator * other.denominator) < (other.numerator * denominator);
  }

  bool operator==(const Fraction &other)
  {
    return numerator == other.numerator && denominator == other.denominator;
  }

  void simplify(void)
  {
    // No-Op if already simplified
    if (is_simplified)
      return;
    word_t hcf = gcd(MIN(numerator, denominator), MAX(numerator, denominator));
    numerator /= hcf;
    denominator /= hcf;
    // Ensure this is a noop after this
    is_simplified = true;
  }
};

std::string to_string(const Fraction &f)
{
  std::stringstream ss;
  ss << f.numerator << "/" << f.denominator;
  return ss.str();
}

struct Node
{
  Fraction value;
  int64_t left, right;

  Node(Fraction val, int64_t left = -1, int64_t right = -1)
      : value{val}, left{left}, right{right}
  {
  }
};

std::vector<Node> nodes;

word_t alloc_node(Node n)
{
  nodes.push_back(n);
  return nodes.size() - 1;
}

void indent_depth(int depth, std::stringstream &ss)
{
  for (int i = 0; i < depth; ++i)
    ss << "  ";
}

std::string to_string(Node n, int depth = 1)
{
  std::stringstream ss;
  ss << "(" << to_string(n.value) << "\n";
  indent_depth(depth, ss);
  if (n.left == -1)
    ss << "NIL";
  else
    ss << to_string(nodes[n.left], depth + 1);
  ss << "\n";
  indent_depth(depth, ss);
  if (n.right == -1)
    ss << "NIL";
  else
    ss << to_string(nodes[n.right], depth + 1);
  ss << ")";
  return ss.str();
}

std::queue<word_t> to_iterate;

void iterate(void)
{
  if (to_iterate.empty())
    return;
  word_t index = to_iterate.front();
  if (nodes[index].left == -1)
  {
    nodes[index].left = alloc_node(Fraction{
        nodes[index].value.numerator,
        nodes[index].value.numerator + nodes[index].value.denominator});
  }
  if (nodes[index].right == -1)
  {
    nodes[index].right = alloc_node(
        Fraction{nodes[index].value.numerator + nodes[index].value.denominator,
                 nodes[index].value.denominator});
  }
  to_iterate.pop();
  to_iterate.push(nodes[index].left);
  to_iterate.push(nodes[index].right);
}

int main(void)
{
  puts("Hello, world!");
  return 0;
}
