//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <__config>
#include <__text_encoding/te_impl.h>
#include <__locale_dir/locale_base_api.h>

_LIBCPP_BEGIN_NAMESPACE_STD

__te_impl __te_impl::__get_locale_encoding(const char* __name){
  __te_impl __e;

  __locale::__locale_t __l = __locale::__newlocale(_LIBCPP_CTYPE_MASK, __name, static_cast<__locale::__locale_t>(0));
  
  if(!__l){
    return __e;
  }

  const char* __codeset = __locale::__nl_langinfo_l(_LIBCPP_NL_CODESET, __l);

  if(!__codeset){
    return __e; 
  }

  string_view __codeset_sv(__codeset);
  
  if(__codeset_sv.size() <= __te_impl::__max_name_length_){
    __e = __te_impl(__codeset_sv);
  }

  __locale::__freelocale(__l);

  return __e;
}

__te_impl __te_impl::__get_env_encoding() {
  return __get_locale_encoding("");
}

_LIBCPP_END_NAMESPACE_STD
