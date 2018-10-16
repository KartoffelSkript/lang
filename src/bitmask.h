// Copyright 2018 the Kartoffelskript project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef KSCRIPT_BITMASK_H
#define KSCRIPT_BITMASK_H

#define BITMASK(VALUE) (1 << (VALUE))

namespace kscript {
namespace internal {

inline void BitmaskSet(int &field, const int &flag);

inline void BitmaskClear(int &field, const int &flag);

inline auto BitmaskCheck(int &field, const int &flag) -> const bool;

} // namespace internal
} // namespace kscript

#endif //KSCRIPT_BITMASK_H
