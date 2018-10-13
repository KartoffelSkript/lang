// Copyright 2018 the Kartoffelskript project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "kscript.h"
#include "log.h"

int main(int argc, char **argv) {
  using namespace kscript;

  StreamStringWriter writer(std::cout);
  Logger logger(&writer, "", Logger::Level::kDebug);

  logger.Log(Logger::Level::kDebug, "Hello Kartoffelkript!");
}
