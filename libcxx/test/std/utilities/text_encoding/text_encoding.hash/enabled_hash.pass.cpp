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

#include <text_encoding>

#include "poisoned_hash_helper.h"
#include "test_macros.h"

int main(int, char**) {
  test_library_hash_specializations_available();

  return 0;
}
