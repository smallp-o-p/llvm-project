//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <text_encoding>

// REQUIRES: std-at-least-c++26

// text_encoding::aliases_view::__iterator

#include <cassert>
#include <compare>
#include <string_view>
#include <text_encoding>

constexpr bool tests(){
  auto te = std::text_encoding(std::text_encoding::ASCII);
  auto i = te.aliases().begin(), j = te.aliases.begin(), k = te.aliases.end();
  static_assert(std::three_way_comparable<decltype(i), std::strong_ordering>);
  {
    assert(i == k);
    assert(i != j);
    assert((i <=> k) == std::strong_ordering_equal);
    assert(std::string_view(*i) == std::string_view(*j));
  }
  {
    assert(std::string_view(i[0]) == std::string_view(k[0]));
    assert(std::string_view(i[1]) != std::string_view(k[3]));
  }
  {
    i++;
    assert(i > k);
    assert((i <=> k) == std::strong_ordering::greater);
    assert(i != j);
    assert(std::string_view(*i) == std::string_view(*j));
  }
  {
    i--;
    assert(i == te.aliases().begin());
    assert(i == k);
    assert(i != j);
    assert(std::string_view(*i) == std::string_view(*j));
  }
  {
    i++;
    k++;
    assert(i != te.aliases().begin());
    assert(i == k);
    assert(i != j);
    assert(std::string_view(*i) == std::string_view(*j));
  }
  {
    auto temp = i + 2;
    assert(i != temp);
    assert(std::string_view(*temp) != std::string_view(*j));
    auto temp2 = temp - 2;
    assert(std::string_view(*temp) == std::string_view(*j));
  }
  {
    assert(i - k == 0);
    assert(j - i > 0);
  }
  {
    auto temp = ++i;
    assert(temp == i);
    auto temp2 = k++;
    assert(temp2 == k - 1);
    assert(i == k);
  }
  {
    i += 2;
    k += 3;

    auto tempi = i;
    auto tempk = k;
    assert(i != k);
    assert((i <=> k) == std::strong_ordering::less);
    i -= 2;
    k -= 3;
    assert(i == k);
    assert(i != tempi && (tempi - i) == 2);
    assert(k != tempk && (tempk - k) == 3);
  }

  return true;
}

int main(){
  static_assert(tests());
  assert(tests());
}
