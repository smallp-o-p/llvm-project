//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <text_encoding>

// REQUIRES: std-at-least-c++26
// REQUIRES: locale.en_US.UTF-8

// UNSUPPORTED: no-localization
// UNSUPPORTED: windows

// class text_encoding

// text_encoding text_encoding::environment(); 

// Concerns:
// 1. todo 

// The current implementation of text_encoding::environment() while conformant, 
// is unfortunately affected by changes to the "LANG" environment variable.

#include <cassert>
#include <cstdlib>
#include <string_view>
#include <text_encoding>

#include "platform_support.h" 
#include "test_macros.h"
#include "test_text_encoding.h"

int main() {
  auto te = std::text_encoding::environment(); 
  
  assert(te == std::text_encoding::environment());
  assert(te.mib() == std::text_encoding::id::ASCII);
  assert(te == std::text_encoding::id::ASCII);
  assert(std::string_view(te.name()) == "ANSI_X3.4-1968");
  assert(te == std::text_encoding("ANSI_X3.4-1968"));

  assert(std::text_encoding::environment_is<std::text_encoding::id::ASCII>());
  assert(!std::text_encoding::environment_is<std::text_encoding::id::UTF8>());
  
  return 0;
}
