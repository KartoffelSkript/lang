// Copyright 2018 the Kartoffelskript project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <cstring>
#include "ntoa.h"

namespace kscript {
namespace internal {

auto ItoaDecimal(integer_t value, char *out) -> void;
auto ItoaHexadecimal(integer_t value, char *out) -> void;
auto ItoaBinary(integer_t value, char *out) -> void;

auto BufferSizeForRadix(char radix) -> char;

auto Itoa(integer_t value) -> std::string {
  return Itoa(value, CommonRadix::kDecimal);
}

auto BufferSizeForRadix(const char radix) -> char {
  kscript_assert(radix < 125);
  return static_cast<>(radix + 2);
}

auto Itoa(integer_t value, CommonRadix radix) -> std::string {
  // Value is zero and therefor turned into an ASCII string manually.
  if (value == 0) {
    switch (radix) {
      case CommonRadix::kBinary:
      case CommonRadix::kDecimal:
        return "0";
      case CommonRadix::kHexadecimal:
        return "0x0";
      default: {
        kscript_unreachable();
      }
    }
  }
  // The itoa(s) of a negative and positive int with the same absolute value
  // only differ in the fact, that one of them has a leading minus sign.
  // To simplify the algorithms, the absolute value of every entry is used.
  bool negative = value < 0;
  if (negative) {
    value = -value;
  }
  // The value can be turned into an ASCII string manually.
  if (value >= 0 && value <= 9) {
    // The boolean is used for arithmetic.
    // Remember, that a true boolean has the value of 1.
    int bufsize = negative + (radix == CommonRadix::kHexadecimal ? 4 : 3);
    int pos = 0;
    char buf[bufsize];

    if (negative) {
      buf[pos++] = '-';
    }

    if (radix == CommonRadix::kHexadecimal) {
      buf[pos++] = '0';
      buf[pos++] = 'x';
    }
    buf[pos++] = AsciiNumber(value);
    // Add an explicit NUL terminator.
    buf[pos] = '\0';
    return std::string(buf);
  }
  // First resolves an unsigned itoa of the absolute value
  // and then either returns it, or in the case of an originally
  // negative value, appends it to an minus sign.
  char buffer[BufferSizeForRadix(radix)];
  switch (radix) {
    case CommonRadix::kDecimal:
      ItoaDecimal(value, buffer);
      break;
    case CommonRadix::kHexadecimal:
      ItoaHexadecimal(value, buffer);
      break;
    case CommonRadix::kBinary:
      ItoaBinary(value, buffer);
      break;
    default: {
      kscript_unreachable();
    }
  }

  if (!negative) {
    return "-" + std::string(buffer);
  }
  return std::string(buffer);
}

#define PUSH_ASCII_PAIR(VALUE, BUFFER, DECIMALS) memcpy((BUFFER) -1, (DECIMALS) + 2 * (VALUE), 2)
auto ItoaDecimal(integer_t value, char *const out) -> void {
  kscript_assert(value > 0);

  // String of ASCII number pairs that increments from '00' to '99.
  // The decimal itoa function uses this as a lookup for numbers
  // greater or equal to then. That way, two digits can be
  // turned into ASCII characters with just one memory copy.
  static const auto kAsciiDecimal = []() -> const char * {
    auto digits = new char[201];
    char offset = 0;
    for (int digit = 0; digit < 100; digit++) {
      digits[offset++] = digit < 10 ? '0' : AsciiNumber(digit / 10);
      digits[offset++] = AsciiNumber(digit);
    }

    digits[offset] = '\0';
    return digits;
  }();

  char *buffer = &out[BufferSizeForRadix(CommonRadix::kDecimal) - 1];
  while (value >= 100) {
    auto const remainder = value % 100;
    value /= 100;
    PUSH_ASCII_PAIR(remainder, buffer, kAsciiDecimal);
    // Two digits have been written to the buffer
    // and since every block of digits is written from left
    // to write, we go back two chars.
    buffer -= 2;
  }
  // The value is now not bigger than hundred and may still
  // be taken from the AsciiDecimalTable. If the value is
  // lower than ten whatsoever, we don't want to do that,
  // since that would add the zero prefix to the actual digit.
  if (value >= 10) {
    PUSH_ASCII_PAIR(value, buffer, kAsciiDecimal);
  }
  // The value is lower than ten and therefor
  // converted with simple char arithmetic.
  *buffer = AsciiNumber(value);
}

#undef PUSH_ASCII_PAIR

} // namespace kscript
} // namespace internal