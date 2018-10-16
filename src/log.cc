// Copyright 2018 the Kartoffelskript project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <vector>
#include "log.h"

using namespace std;

namespace kscript {
namespace internal {
namespace logger {

Config::Config() :
    flags_(0),
    logging_level_threshold_(Level::kUndefined),
    logging_level_fallback_(Level::kUndefined),
    export_file_dir_({"undefined"}),
    log_file_dir_({"undefined"}),
    export_file_name_pattern_(""),
    log_file_name_pattern_("") {}

static auto Config::CreateDefault(EngineState state) -> shared_ptr<Config> {
  auto cfg = make_shared<Config>();

  cfg->logging_level_fallback(Level::kInfo);
  cfg->logging_level_threshold(Level::kInfo);
  switch (state) {
    case EngineState::kParsing:
      cfg->enable_console_logs();
      cfg->enable_file_logs();
      cfg->only_save_files_on_error();
      cfg->use_lazy_file_logging();
#ifdef KSCRIPT_DEBUG_MODE
      cfg->allow_stack_tracing();
#endif
      cfg->log_file_dir({"logs", kDirectoryFlagRelative | kDirectoryFlagBuildDirChild});
      cfg->log_file_name_pattern("log-%s.txt");
      break;
    case EngineState::kInterpreting:
      cfg->enable_timestamps();
      cfg->enable_file_logs();
      cfg->enable_console_logs();
      cfg->allow_stack_tracing();
      cfg->allow_console_colors();
      cfg->log_file_dir({"kscript_rt", (kDirectoryFlagTemp)});
      cfg->log_file_name_pattern("log-%s.txt");
      break;
    default: {
      kscript_unreachable();
    }
  }
  return cfg;
}

} // namespace logger
} // namespace internal
} // namespace kscript