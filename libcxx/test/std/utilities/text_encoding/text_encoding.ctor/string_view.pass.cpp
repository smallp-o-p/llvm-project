//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <text_encoding>

// REQUIRES: std-at-least-c++26
// ADDITIONAL_COMPILE_FLAGS(has-fconstexpr-steps): -fconstexpr-steps=30000000
// ADDITIONAL_COMPILE_FLAGS(has-fconstexpr-ops-limit): -fconstexpr-ops-limit=90000000
// class text_encoding

// text_encoding::text_encoding(string_view) noexcept

#include <cassert>
#include <string_view>
#include <text_encoding>
#include <type_traits>

#include "../test_text_encoding.h"

constexpr bool test_ctor(std::string_view str, std::string_view expect, std::text_encoding::id expect_id) {
  auto te      = std::text_encoding(str);
  bool success = true;

  assert(te.mib() == expect_id);
  assert(te.name() == expect);

  return success;
}

constexpr bool test_primary_encoding_spellings() {
  for (auto& data : unique_encoding_data) {
    auto te = std::text_encoding(std::string_view(data.name));

    assert(te.mib() == std::text_encoding::id(data.mib));
    assert(te.name() == std::string_view(data.name));
  }
  return true;
}

constexpr bool test_others() {
  for (auto& name : other_names) {
    auto te = std::text_encoding(std::string_view(name));

    assert(te.mib() == std::text_encoding::other);
    assert(te.name() == std::string_view(name));
  }
  return true;
}

int main(int, char**) {
  {
    static_assert(std::is_nothrow_constructible<std::text_encoding, std::string_view>::value,
                  "Must be nothrow constructible with string_view");
  }

  // happy paths
  {
    static_assert(test_primary_encoding_spellings());
    assert(test_primary_encoding_spellings());
  }

  {
    static_assert(test_ctor("U_T_F-8", "U_T_F-8", std::text_encoding::UTF8));
    assert(test_ctor("U_T_F-8", "U_T_F-8", std::text_encoding::UTF8));
  }

  {
    static_assert(test_ctor("utf8", "utf8", std::text_encoding::UTF8));
    assert(test_ctor("utf8", "utf8", std::text_encoding::UTF8));
  }

  {
    static_assert(test_ctor("u.t.f-008", "u.t.f-008", std::text_encoding::UTF8));
    assert(test_ctor("u.t.f-008", "u.t.f-008", std::text_encoding::UTF8));
  }

  {
    static_assert(test_ctor("utf-80", "utf-80", std::text_encoding::other));
    assert(test_ctor("utf-80", "utf-80", std::text_encoding::other));
  }

  {
    static_assert(test_ctor("iso885931988", "iso885931988", std::text_encoding::ISOLatin3));
    assert(test_ctor("iso885931988", "iso885931988", std::text_encoding::ISOLatin3));
  }

  {
    static_assert(test_ctor("iso00885931988", "iso00885931988", std::text_encoding::ISOLatin3));
    assert(test_ctor("iso00885931988", "iso00885931988", std::text_encoding::ISOLatin3));
  }

  {
    static_assert(test_others());
    assert(test_others());
  }

  return 0;
}
