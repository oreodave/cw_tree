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
 * Description: Computation necessary for generating the tree
 */

#ifndef NUMERICS_HPP
#define NUMERICS_HPP

#include <cstdint>

#include <optional>
#include <queue>
#include <string>
#include <vector>

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((B) < (A) ? (A) : (B))
typedef uint64_t word_t;
typedef std::optional<word_t> index_t;

struct Fraction
{
  word_t numerator, denominator;
  long double norm;

  Fraction(word_t numerator = 0, word_t denominator = 1);
  bool operator<(const Fraction other);
  bool operator==(const Fraction &other);
};

struct Node
{
  Fraction value;
  index_t left, right;

  Node(Fraction val = {}, index_t left = std::nullopt,
       index_t right = std::nullopt);
};

struct NodeAllocator
{
  std::vector<Node> vec;

  NodeAllocator(word_t capacity);
  word_t alloc(Node n);
  Node getVal(word_t n) const;
  Node &getRef(word_t n);
};

word_t gcd(word_t a, word_t b);
Fraction iterate(std::queue<word_t> &queue, NodeAllocator &allocator);

std::string to_string(const Fraction &);
std::string to_string(const NodeAllocator &, const index_t, int depth = 1);

#endif
