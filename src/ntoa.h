// Copyright 2018 the Kartoffelskript project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef KSCRIPT_NTOA_H
#define KSCRIPT_NTOA_H

#include "kscript.h"

namespace kscript {
namespace internal {

using decimal_t = double;
using integer_t = long long;
using large_integer_t = unsigned long long;

enum CommonRadix {
  kBinary = 2,
  kDecimal = 10,
  kHexadecimal = 16
};

auto Itoa(integer_t value) -> std::string;

auto Itoa(integer_t value, CommonRadix radix) -> std::string;

auto Itoa(integer_t value, char custom_radix) -> std::string;

auto Dtoa(decimal_t value) -> std::string;

auto Dtoa(decimal_t value, char fp_digits) -> std::string;

auto AsciiNumber(integer_t offset) -> char;

} // namespace internal
} // namespace kscript

#endif //KSCRIPT_NTOA_H
