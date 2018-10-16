// Copyright 2018 the Kartoffelskript project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "bitmask.h"

namespace kscript {
namespace internal {

inline void BitmaskSet(int &field, const int &flag) {
  field |= flag;
}

inline void BitmaskClear(int &field, const int &flag) {
  field ^= flag;
}

inline void BitmaskUnset(int &field, const int &flag) {
  field &= ~flag;
}

inline auto BitmaskCheck(int &field, const int &flag) -> const bool {
  return (field &= flag) != 0;
}

} // namespace internal
} // namespace kscript
