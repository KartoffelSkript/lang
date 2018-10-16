// Copyright 2018 the Kartoffelskript project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef KSCRIPT_KSCRIPT_H
#define KSCRIPT_KSCRIPT_H

#include <iostream>
#include <cassert>

#define KSCRIPT_DEBUG_MODE

#define kscript_unreachable() static_assert(false)
#define kscript_static_assert(statement) static_assert(statement)
#define kscript_assert(statement) assert(statement)

#define kscript_bitmask(name, value) name = (1 << (value))
#define kscript_enum_bt(name, value) name = (1 >> (value))

namespace kscript {
namespace internal {

enum EngineState;

enum EngineState {
  kBooting,
  kParsing,
  kInterpreting
};

}
}

#endif //KSCRIPT_KSCRIPT_H
