//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26

// <text_encoding>

// template <> struct hash<text_encoding>

#include <cassert>
#include <cstdint>
#include <text_encoding>
#include <type_traits>

bool test_te_hash() {
  using T = std::text_encoding;
  using H = std::hash<T>;

  {
    const T te(std::text_encoding::ASCII);
    const H h{};
    assert(h(te) == h(te));
    static_assert(std::is_same_v<decltype(h(te)), std::size_t>);
  }

  {
    const T te1(std::text_encoding::ASCII);
    const T te2(std::text_encoding::UTF8);
    const H h{};

    assert(h(te1) != h(te2));
  }

  {
    const T te1(std::text_encoding::unknown);
    const T te2(std::text_encoding::unknown);
    const H h{};
    assert(h(te1) == h(te2));
  }

  {
    const T te1(std::text_encoding::other);
    const T te2(std::text_encoding::other);
    const H h{};
    assert(h(te1) == h(te2));
  }

  return true;
}

int main(int, char**) {
  {
    assert(test_te_hash());
  }

  return 0;
}
