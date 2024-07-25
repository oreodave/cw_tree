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
#include <raylib.h>

typedef uint64_t word_t;

struct Fraction
{
  word_t numerator, denominator;
  bool is_simplified;

  Fraction(word_t numerator = 0, word_t denominator = 1,
           bool is_simplified = false)
      : numerator{numerator}, denominator{denominator},
        is_simplified{is_simplified}
  {
  }

  void simplify(void)
  {
    // No-Op if already simplified
    if (is_simplified)
      return;
    word_t a = numerator < denominator ? numerator : denominator,
           b = numerator < denominator ? denominator : numerator;
    // Euclidean algorithm
    for (word_t r = b % a; r != 0; b = a, a = r, r = b % a)
      continue;
    // a will be the gcd
    numerator /= a;
    denominator /= a;
    // Ensure this is a noop after this
    is_simplified = true;
  }
};

int main(void)
{
  puts("Hello, world!");
  return 0;
}
