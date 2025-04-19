//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <text_encoding> 
#include <locale> 

_LIBCPP_BEGIN_NAMESPACE_STD

text_encoding locale::encoding() const {
  auto __locale_name = name(); 
  if(__locale_name.size() == 1){
    if(__locale_name[0] == 'C')
      return text_encoding(text_encoding::ASCII);
    if(__locale_name[0] == '*')
      return text_encoding(); 
  }

  return text_encoding(name().c_str());
}
_LIBCPP_END_NAMESPACE_STD 
