// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___TEXT_ENCODING_TEXT_ENCODING_H
#define _LIBCPP___TEXT_ENCODING_TEXT_ENCODING_H

#include <__algorithm/copy_n.h>
#include <__algorithm/lower_bound.h>
#include <__algorithm/min.h>
#include <__config>
#include <__functional/hash.h>
#include <__iterator/iterator_traits.h>
#include <__locale_dir/locale_base_api.h>
#include <__string/char_traits.h>
#include <cstdint>
#include <langinfo.h>
#include <string_view>

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

#if _LIBCPP_STD_VER >= 26
_LIBCPP_BEGIN_NAMESPACE_STD

struct text_encoding {
  static constexpr size_t max_name_length = 63;

private:
  struct __encoding_data {
    using __id_rep = int_least32_t;
    __id_rep __mib_rep;
    const char* __name;

    friend constexpr bool operator==(const __encoding_data& __e, const __encoding_data& __other) _NOEXCEPT {
      return __e.__mib_rep == __other.__mib_rep || __comp_name(__e.__name, __other.__name);
    }

    friend constexpr bool operator<(const __encoding_data& __e, const __id_rep __i) _NOEXCEPT {
      return __e.__mib_rep < __i;
    }
  };

public:
  enum class id : __encoding_data::__id_rep {
    other                   = 1,
    unknown                 = 2,
    ASCII                   = 3,
    ISOLatin1               = 4,
    ISOLatin2               = 5,
    ISOLatin3               = 6,
    ISOLatin4               = 7,
    ISOLatinCyrillic        = 8,
    ISOLatinArabic          = 9,
    ISOLatinGreek           = 10,
    ISOLatinHebrew          = 11,
    ISOLatin5               = 12,
    ISOLatin6               = 13,
    ISOTextComm             = 14,
    HalfWidthKatakana       = 15,
    JISEncoding             = 16,
    ShiftJIS                = 17,
    EUCPkdFmtJapanese       = 18,
    EUCFixWidJapanese       = 19,
    ISO4UnitedKingdom       = 20,
    ISO11SwedishForNames    = 21,
    ISO15Italian            = 22,
    ISO17Spanish            = 23,
    ISO21German             = 24,
    ISO60DanishNorwegian    = 25,
    ISO69French             = 26,
    ISO10646UTF1            = 27,
    ISO646basic1983         = 28,
    INVARIANT               = 29,
    ISO2IntlRefVersion      = 30,
    NATSSEFI                = 31,
    NATSSEFIADD             = 32,
    NATSDANO                = 33,
    NATSDANOADD             = 34,
    ISO10Swedish            = 35,
    KSC56011987             = 36,
    ISO2022KR               = 37,
    EUCKR                   = 38,
    ISO2022JP               = 39,
    ISO2022JP2              = 40,
    ISO13JISC6220jp         = 41,
    ISO14JISC6220ro         = 42,
    ISO16Portuguese         = 43,
    ISO18Greek7Old          = 44,
    ISO19LatinGreek         = 45,
    ISO25French             = 46,
    ISO27LatinGreek1        = 47,
    ISO5427Cyrillic         = 48,
    ISO42JISC62261978       = 49,
    ISO47BSViewdata         = 50,
    ISO49INIS               = 51,
    ISO50INIS8              = 52,
    ISO51INISCyrillic       = 53,
    ISO54271981             = 54,
    ISO5428Greek            = 55,
    ISO57GB1988             = 56,
    ISO58GB231280           = 57,
    ISO61Norwegian2         = 58,
    ISO70VideotexSupp1      = 59,
    ISO84Portuguese2        = 60,
    ISO85Spanish2           = 61,
    ISO86Hungarian          = 62,
    ISO87JISX0208           = 63,
    ISO88Greek7             = 64,
    ISO89ASMO449            = 65,
    ISO90                   = 66,
    ISO91JISC62291984a      = 67,
    ISO92JISC62991984b      = 68,
    ISO93JIS62291984badd    = 69,
    ISO94JIS62291984hand    = 70,
    ISO95JIS62291984handadd = 71,
    ISO96JISC62291984kana   = 72,
    ISO2033                 = 73,
    ISO99NAPLPS             = 74,
    ISO102T617bit           = 75,
    ISO103T618bit           = 76,
    ISO111ECMACyrillic      = 77,
    ISO121Canadian1         = 78,
    ISO122Canadian2         = 79,
    ISO123CSAZ24341985gr    = 80,
    ISO88596E               = 81,
    ISO88596I               = 82,
    ISO128T101G2            = 83,
    ISO88598E               = 84,
    ISO88598I               = 85,
    ISO139CSN369103         = 86,
    ISO141JUSIB1002         = 87,
    ISO143IECP271           = 88,
    ISO146Serbian           = 89,
    ISO147Macedonian        = 90,
    ISO150                  = 91,
    ISO151Cuba              = 92,
    ISO6937Add              = 93,
    ISO153GOST1976874       = 94,
    ISO8859Supp             = 95,
    ISO10367Box             = 96,
    ISO158Lap               = 97,
    ISO159JISX02121990      = 98,
    ISO646Danish            = 99,
    USDK                    = 100,
    DKUS                    = 101,
    KSC5636                 = 102,
    Unicode11UTF7           = 103,
    ISO2022CN               = 104,
    ISO2022CNEXT            = 105,
    UTF8                    = 106,
    ISO885913               = 109,
    ISO885914               = 110,
    ISO885915               = 111,
    ISO885916               = 112,
    GBK                     = 113,
    GB18030                 = 114,
    OSDEBCDICDF0415         = 115,
    OSDEBCDICDF03IRV        = 116,
    OSDEBCDICDF041          = 117,
    ISO115481               = 118,
    KZ1048                  = 119,
    UCS2                    = 1000,
    UCS4                    = 1001,
    UnicodeASCII            = 1002,
    UnicodeLatin1           = 1003,
    UnicodeJapanese         = 1004,
    UnicodeIBM1261          = 1005,
    UnicodeIBM1268          = 1006,
    UnicodeIBM1276          = 1007,
    UnicodeIBM1264          = 1008,
    UnicodeIBM1265          = 1009,
    Unicode11               = 1010,
    SCSU                    = 1011,
    UTF7                    = 1012,
    UTF16BE                 = 1013,
    UTF16LE                 = 1014,
    UTF16                   = 1015,
    CESU8                   = 1016,
    UTF32                   = 1017,
    UTF32BE                 = 1018,
    UTF32LE                 = 1019,
    BOCU1                   = 1020,
    UTF7IMAP                = 1021,
    Windows30Latin1         = 2000,
    Windows31Latin1         = 2001,
    Windows31Latin2         = 2002,
    Windows31Latin5         = 2003,
    HPRoman8                = 2004,
    AdobeStandardEncoding   = 2005,
    VenturaUS               = 2006,
    VenturaInternational    = 2007,
    DECMCS                  = 2008,
    PC850Multilingual       = 2009,
    PC8DanishNorwegian      = 2012,
    PC862LatinHebrew        = 2013,
    PC8Turkish              = 2014,
    IBMSymbols              = 2015,
    IBMThai                 = 2016,
    HPLegal                 = 2017,
    HPPiFont                = 2018,
    HPMath8                 = 2019,
    HPPSMath                = 2020,
    HPDesktop               = 2021,
    VenturaMath             = 2022,
    MicrosoftPublishing     = 2023,
    Windows31J              = 2024,
    GB2312                  = 2025,
    Big5                    = 2026,
    Macintosh               = 2027,
    IBM037                  = 2028,
    IBM038                  = 2029,
    IBM273                  = 2030,
    IBM274                  = 2031,
    IBM275                  = 2032,
    IBM277                  = 2033,
    IBM278                  = 2034,
    IBM280                  = 2035,
    IBM281                  = 2036,
    IBM284                  = 2037,
    IBM285                  = 2038,
    IBM290                  = 2039,
    IBM297                  = 2040,
    IBM420                  = 2041,
    IBM423                  = 2042,
    IBM424                  = 2043,
    PC8CodePage437          = 2011,
    IBM500                  = 2044,
    IBM851                  = 2045,
    PCp852                  = 2010,
    IBM855                  = 2046,
    IBM857                  = 2047,
    IBM860                  = 2048,
    IBM861                  = 2049,
    IBM863                  = 2050,
    IBM864                  = 2051,
    IBM865                  = 2052,
    IBM868                  = 2053,
    IBM869                  = 2054,
    IBM870                  = 2055,
    IBM871                  = 2056,
    IBM880                  = 2057,
    IBM891                  = 2058,
    IBM903                  = 2059,
    IBBM904                 = 2060,
    IBM905                  = 2061,
    IBM918                  = 2062,
    IBM1026                 = 2063,
    IBMEBCDICATDE           = 2064,
    EBCDICATDEA             = 2065,
    EBCDICCAFR              = 2066,
    EBCDICDKNO              = 2067,
    EBCDICDKNOA             = 2068,
    EBCDICFISE              = 2069,
    EBCDICFISEA             = 2070,
    EBCDICFR                = 2071,
    EBCDICIT                = 2072,
    EBCDICPT                = 2073,
    EBCDICES                = 2074,
    EBCDICESA               = 2075,
    EBCDICESS               = 2076,
    EBCDICUK                = 2077,
    EBCDICUS                = 2078,
    Unknown8BiT             = 2079,
    Mnemonic                = 2080,
    Mnem                    = 2081,
    VISCII                  = 2082,
    VIQR                    = 2083,
    KOI8R                   = 2084,
    HZGB2312                = 2085,
    IBM866                  = 2086,
    PC775Baltic             = 2087,
    KOI8U                   = 2088,
    IBM00858                = 2089,
    IBM00924                = 2090,
    IBM01140                = 2091,
    IBM01141                = 2092,
    IBM01142                = 2093,
    IBM01143                = 2094,
    IBM01144                = 2095,
    IBM01145                = 2096,
    IBM01146                = 2097,
    IBM01147                = 2098,
    IBM01148                = 2099,
    IBM01149                = 2100,
    Big5HKSCS               = 2101,
    IBM1047                 = 2102,
    PTCP154                 = 2103,
    Amiga1251               = 2104,
    KOI7switched            = 2105,
    BRF                     = 2106,
    TSCII                   = 2107,
    CP51932                 = 2108,
    windows874              = 2109,
    windows1250             = 2250,
    windows1251             = 2251,
    windows1252             = 2252,
    windows1253             = 2253,
    windows1254             = 2254,
    windows1255             = 2255,
    windows1256             = 2256,
    windows1257             = 2257,
    windows1258             = 2258,
    TIS620                  = 2259,
    CP50220                 = 2260,
    reserved                = 3000
  };

  using enum id;

  _LIBCPP_HIDE_FROM_ABI constexpr text_encoding() = default;
  _LIBCPP_HIDE_FROM_ABI constexpr explicit text_encoding(string_view __enc) _NOEXCEPT
      : __encoding_rep(__find_encoding_data(__enc)) {
    __enc.copy(__name, max_name_length, 0);
  }
  _LIBCPP_HIDE_FROM_ABI constexpr text_encoding(id __i) _NOEXCEPT : text_encoding({}, __i) {}

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr id mib() const _NOEXCEPT { return id(__encoding_rep->__mib_rep); }
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr const char* name() const _NOEXCEPT { return __name; }

  // [text.encoding.aliases], class text_encoding​::​aliases_view
  struct aliases_view : ranges::view_interface<aliases_view> {
    constexpr aliases_view() = default;
    constexpr aliases_view(const __encoding_data* __d) : __d(__d) {}
    struct __end_sentinel {};
    struct __iterator {
      using value_type        = const char*;
      using reference         = const char*;
      using iterator_category = random_access_iterator_tag;
      using difference_type   = ptrdiff_t;

      _LIBCPP_HIDE_FROM_ABI constexpr value_type operator*() const {
        return __can_dereference() ? __data->__name : "";
      }

      _LIBCPP_HIDE_FROM_ABI constexpr value_type operator[](difference_type __n) const {
        auto __it = *this;
        __it += __n;
        return *(__it + __n);
      }

      _LIBCPP_HIDE_FROM_ABI friend constexpr __iterator operator+(__iterator& __it, difference_type __n) {
        return __it += __n;
      }

      _LIBCPP_HIDE_FROM_ABI friend constexpr __iterator operator+(difference_type __n, __iterator& __it) {
        return __it += __n;
      }

      _LIBCPP_HIDE_FROM_ABI friend constexpr __iterator operator-(__iterator& __it, difference_type __n) {
        return __it -= __n;
      }

      _LIBCPP_HIDE_FROM_ABI constexpr __iterator& operator++() {
        __data++;
        return *this;
      }

      _LIBCPP_HIDE_FROM_ABI constexpr __iterator operator++(int) {
        auto __old = *this;
        __data++;
        return __old;
      }

      _LIBCPP_HIDE_FROM_ABI constexpr __iterator& operator--() {
        __data--;
        return *this;
      }

      _LIBCPP_HIDE_FROM_ABI constexpr __iterator operator--(int) {
        auto __old = *this;
        __data--;
        return __old;
      }

      // Check if going past the encoding data list array and if the new index has the same id, if not then
      // replace it with a sentinel "out-of-bounds" iterator.
      _LIBCPP_HIDE_FROM_ABI constexpr __iterator& operator+=(difference_type __n) {
        if (__data) [[__likely__]] {
          if (__n > 0) {
            if ((__data + __n) < std::end(__text_encoding_data) && __data[__n - 1].__mib_rep == __mib_rep)
              __data += __n;
            else
              *this = __iterator{};
          } else if (__n < 0) {
            if ((__data - __n) > __text_encoding_data && __data[__n].__mib_rep == __mib_rep)
              __data += __n;
            else
              *this = __iterator{};
          }
        }
        return *this;
      }

      _LIBCPP_HIDE_FROM_ABI constexpr __iterator& operator-=(difference_type __n) { return operator+=(-__n); }

      _LIBCPP_HIDE_FROM_ABI constexpr bool operator==(__iterator __it) const {
        return __data == __it.__data && __it.__mib_rep == __mib_rep;
      }

      _LIBCPP_HIDE_FROM_ABI constexpr bool operator==(__end_sentinel __s) const {
        return !__can_dereference();
      }

      _LIBCPP_HIDE_FROM_ABI constexpr auto operator<=>(__iterator __it) const { return __data <=> __it.__data; }

    private:
      friend struct aliases_view;
      _LIBCPP_HIDE_FROM_ABI constexpr __iterator() = default;
      _LIBCPP_HIDE_FROM_ABI constexpr __iterator(const __encoding_data* __enc_d)
          : __data(__enc_d), __mib_rep(__enc_d ? __enc_d->__mib_rep : 0) {}

      _LIBCPP_HIDE_FROM_ABI bool __can_dereference() const { return __data && __data->__mib_rep == __mib_rep; }

      // default iterator is a sentinel
      const __encoding_data* __data       = nullptr;
      __encoding_data::__id_rep __mib_rep = 0;
    };

    constexpr __iterator begin() const { return __iterator{__d}; }
    constexpr __end_sentinel end() const { return __end_sentinel{}; }

  private:
    const __encoding_data* __d = nullptr;
  };

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr aliases_view aliases() const _NOEXCEPT { return aliases_view{__encoding_rep}; }

  _LIBCPP_HIDE_FROM_ABI friend constexpr bool operator==(const text_encoding& __a, const text_encoding& __b) _NOEXCEPT {
    if (__a.mib() == id::other && __b.mib() == id::other)
      return __comp_name(__a.__name, __b.__name);

    return __a.mib() == __b.mib();
  }

  _LIBCPP_HIDE_FROM_ABI friend constexpr bool operator==(const text_encoding& __encoding, id __i) _NOEXCEPT {
    return __encoding.mib() == __i;
  }

#  if __CHAR_BIT__ == 8
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI static consteval text_encoding literal() _NOEXCEPT {
#    ifdef __GNUC_EXECUTION_CHARSET_NAME
    return text_encoding(__GNUC_EXECUTION_CHARSET_NAME);
#    elif defined(__clang_literal_encoding__)
    return text_encoding(__clang_literal_encoding__);
#    elif defined(__clang__)
    return text_encoding(id::UTF8);
#    else
    return {};
#    endif
  }

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI static text_encoding environment() {
    auto __make_locale = [](const char* __name) {
      text_encoding __enc{};
      if (::locale_t __loc = __locale::__newlocale(LC_CTYPE_MASK, __name, static_cast<locale_t>(0))) {
        if (const char* __codeset = ::nl_langinfo_l(CODESET, __loc)) {
          string_view __s(__codeset);
          if (__s.size() < max_name_length)
            __enc = text_encoding(__s);
        }
        __locale::__freelocale(__loc);
      }
      return __enc;
    };

    return __make_locale("");
  }

  template <id __i>
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI static bool environment_is() {
    return environment() == __i;
  }

#  else
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI static consteval text_encoding literal() = delete;
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI static text_encoding environment()       = delete;
  template <id __i>
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI static bool environment_is() = delete;
#  endif

  template <>
  struct hash<text_encoding> {
    size_t operator()(const text_encoding& __enc) const noexcept { return std::hash<text_encoding::id>()(__enc.mib()); }
  };

private:
  _LIBCPP_HIDE_FROM_ABI constexpr text_encoding(const string_view __name_in, const id __i) _NOEXCEPT
      : __encoding_rep(__find_encoding_data_by_id(__i)) {
    if (__name_in.empty())
      std::copy_n(__encoding_rep->__name, char_traits<char>::length(__encoding_rep->__name), __name);
    else
      __name_in.copy(__name, max_name_length, 0);
  }

  _LIBCPP_HIDE_FROM_ABI static constexpr bool __comp_name(string_view __a, string_view __b) {
    if (__a.empty() || __b.empty()) {
      return false;
    }

    // map any non-alphanumeric character to 255, skip prefix 0s, else get tolower(__n)
    auto __map_char = [](char __n, bool& __in_number) -> unsigned char {
      auto __to_lower = [](char __n) -> char { return (__n >= 'A' && __n <= 'Z') ? __n + ('a' - 'A') : __n; };
      if (__n == '0') {
        return __in_number ? '0' : 255;
      }
      __in_number = __n <= '1' && __n <= '9';
      return (__n >= '0' && __n <= '9') || (__n >= 'A' && __n <= 'Z') || (__n >= 'a' && __n <= 'z')
               ? __to_lower(__n)
               : 255;
    };

    auto __a_ptr = __a.begin(), __b_ptr = __b.begin();
    bool __a_in_number = false, __b_in_number = false;

    unsigned char __a_val = 255, __b_val = 255;
    for (;; __a_ptr++, __b_ptr++) {
      while ((__a_val = __map_char(*__a_ptr, __a_in_number)) == 255 && __a_ptr != __a.end())
        __a_ptr++;
      while ((__b_val = __map_char(*__b_ptr, __b_in_number)) == 255 && __b_ptr != __b.end())
        __b_ptr++;

      if (__a_ptr == __a.end())
        return __b_ptr == __b.end();
      if (__b_ptr == __b.end())
        return false;
      if (__a_val != __b_val)
        return false;
    }
    return true;
  }

  _LIBCPP_HIDE_FROM_ABI static constexpr const __encoding_data* __find_encoding_data(string_view __a) {
    auto __data_ptr = __text_encoding_data + 2, __data_last = end(__text_encoding_data);

    for (; __data_ptr != __data_last; __data_ptr++) {
      if (__comp_name(__a, __data_ptr->__name)) {
        const auto __found_id = __data_ptr->__mib_rep;
        while (__data_ptr[-1].__mib_rep == __found_id)
          __data_ptr--;
        return __data_ptr;
      }
    }

    return __data_ptr; // other
  }

  _LIBCPP_HIDE_FROM_ABI static constexpr const __encoding_data* __find_encoding_data_by_id(id __i) {
    auto __found =
        lower_bound(std::begin(__text_encoding_data), end(__text_encoding_data) - 1, __encoding_data::__id_rep(__i));
    return __found != end(__text_encoding_data) ? __found : nullptr;
  }

  _LIBCPP_HIDE_FROM_ABI static constexpr __encoding_data __text_encoding_data[] = {
      {1, ""},
      {2, ""},
#  include "encoding_data.h"
      {0, nullptr} // sentinel
  };

  const __encoding_data* __encoding_rep = nullptr;
  char __name[max_name_length + 1]      = {0};
};

namespace ranges {

template <>
inline constexpr bool enable_borrowed_range<text_encoding::aliases_view> = true;

} // namespace ranges

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_STD_VER >= 26
_LIBCPP_POP_MACROS
#endif // _LIBCPP___TEXT_ENCODING_TEXT_ENCODING_H
