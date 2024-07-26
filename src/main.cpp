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

struct Node
{
  Fraction value;
  Node *left, *right;

  Node(Fraction val, Node *left = nullptr, Node *right = nullptr)
      : value{val}, left{left}, right{right}
  {
  }
};

std::vector<Node> nodes;

int main(void)
{
  puts("Hello, world!");
  return 0;
}
