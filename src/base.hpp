/* base.hpp: Basic definitions
 * Created: 2025-11-27
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#ifndef BASE_HPP
#define BASE_HPP

#include <cassert>
#include <cstdint>
#include <optional>

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((B) < (A) ? (A) : (B))

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

static_assert(sizeof(float) == 4, "f32 requires 4 byte floats");
static_assert(sizeof(double) == 8, "f64 requires 8 byte doubles");
using f32 = float;
using f64 = double;

inline u64 gcd(u64 a, u64 b)
{
  if (a == b)
    return a;
  else if (a <= 1 || b <= 1)
    return 1;
  for (u64 r = b % a; r != 0; b = a, a = r, r = b % a)
    continue;
  return a;
}

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
