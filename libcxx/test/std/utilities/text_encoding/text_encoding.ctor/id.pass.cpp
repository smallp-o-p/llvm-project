//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <text_encoding>

// REQUIRES: std-at-least-c++26

// class text_encoding

// text_encoding::text_encoding(id) noexcept

#include <algorithm>
#include <cassert>
#include <ranges>
#include <text_encoding>
#include <type_traits>

#include "../test_text_encoding.h"

using te_id = std::text_encoding::id;

constexpr bool id_ctor(te_id i, te_id expect_id, std::string_view expect_name) {
  auto te = std::text_encoding(i);

  assert(te.mib() == expect_id);
  assert(expect_name == te.name());
  assert(std::ranges::contains(te.aliases(), expect_name));

  return true;
}

constexpr bool id_ctors() {
  for (auto pair : unique_encoding_data) {
    assert(id_ctor(te_id(pair.mib), te_id(pair.mib), pair.name));
  }
  return true;
}

constexpr bool test_unknown_other() {
  {
    constexpr auto te = std::text_encoding(te_id::other);

    assert(te.mib() == te_id::other);
    assert(std::string_view("") == te.name());
    assert(std::ranges::empty(te.aliases()));
  }

  {
    constexpr auto te = std::text_encoding(te_id::unknown);
    assert(te.mib() == te_id::unknown);
    assert(std::string_view("") == te.name());
    assert(std::ranges::empty(te.aliases()));
  }
  return true;
}

constexpr bool tests() {
  {
    // 2. Constructing an object with a valid id must set mib() and the name to the corresponding value.
    assert(id_ctors());
  }

  {
    // 3. Constructing an object using id::unknown or id::other must set mib() to id::unknown or id::other, respectively, and the name to an empty string.
    assert(test_unknown_other());
  }

  return true;
}

int main(int, char**) {
  {
    // 1. text_encoding(id) must be nothrow
    static_assert(std::is_nothrow_constructible<std::text_encoding, std::text_encoding::id>::value,
                  "Must be nothrow constructible with id");
  }

  {
    static_assert(tests());
    assert(tests());
  }

  return 0;
}
