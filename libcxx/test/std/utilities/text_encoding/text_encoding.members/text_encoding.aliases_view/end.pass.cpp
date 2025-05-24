//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <text_encoding>

// REQUIRES: std-at-least-c++26

// struct text_encoding::aliases_view

// Concerns:

#include "test_text_encoding.h"
#include <text_encoding>

int main(int, char**) {
  auto a_other = std::text_encoding("foobar");
  auto other_aliases = a_other.aliases();

  // 1. begin() of an aliases_view() of "other" is equal to end()
  assert(other_aliases.begin() == other_aliases.end());

  return 0;
}
