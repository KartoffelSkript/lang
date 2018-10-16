// Copyright 2018 the Kartoffelskript project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef KSCRIPT_LOG_H
#define KSCRIPT_LOG_H

#include <sstream>
#include <memory>
#include <map>
#include <vector>
#include <chrono>

#include "kscript.h"

using namespace std;
using namespace std::chrono;

#define KSCRIPT_LOG_CONCURRENT

namespace kscript {
namespace internal {
namespace logger {

class Logger;
class LoggerFactory;
class ConfiguredLoggerFactory;
class Filter;
class LevelFilter;
class Handler;
class StreamPrintHandler;
class FilePrintHandler;
class StructuredPrintHandler;
class CrashReporter;
class XmlExporter;

class Config;
class ConfigParser;

class Formatter;
class DateFormat;
class DateFormatter;

struct Entry;
struct CxxTrace;
struct CxxMethodDescriptor;
struct CxxExceptionDescriptor;

enum Level;
enum Flags;

enum Level {
  kTrace,
  kDebug,
  kVerbose,
  kInfo,
  kWarning,
  kError,
  kFatal,
  kAll,
  kUndefined = -1
};

class Logger {

  Logger();

  ~Logger();

  void Log(string line, void *...args) const;

  void Log(Level level, string line, void *...args) const;

  void Trace(CxxTrace trace) const;

  void Debug(string line, void *...args) const;

  void Fatal(string line, void *...args) const;

  void Fatal(CxxExceptionDescriptor exception) const;

  void Error(string line, void *...args) const;

  void Error(CxxExceptionDescriptor exception) const;

  void Warning(string line, void *...args) const;

  void Verbose(string line, void *...args) const;

  void Info(string line, void *...args) const;

  void AddHandler(Handler *handler);

  void SetHandlers(vector<Handler *> handlers);

  auto severity_threshold() const -> Level {
    return severity_threshold_;
  }

 private:
  Level severity_threshold_;
  Filter *filter_;
  vector<Handler *> handlers_;
  Formatter formatter_;
};

class LoggerFactory {
 public:
  virtual auto GetInstance(const string &name) const -> const Logger & = 0;
};

class ConfiguredLoggerFactory final : public LoggerFactory {
  using CreatedLoggerStorage = map<string, unique_ptr<Logger>>;

 public:
 private:
  unique_ptr<Logger> CreateLogger(const string &name);

  CreatedLoggerStorage loggers_;
  Config config_;
};

class Filter {
 public:
  auto Allow(const Entry &entry) const -> bool;
};

class Handler {
 public:
  void Publish(Entry &entry) const;
};

class Formatter {

};

class DateFormatter {

};

struct Entry {
  string message;
  milliseconds timestamp;
  Level severity;
};

class Config {
 public:
  static const int kFlagEnableTimestamps = 1 << 0;
  static const int kFlagAllowStackTracing = 1 << 1;
  static const int kFlagEnableConsoleLog = 1 << 2;
  static const int kFlagAllowConsoleColors = 1 << 3;
  static const int kFlagEnableFileLogs = 1 << 4;
  static const int kFlagExportStructured = 1 << 5;
  static const int kFlagUseLazyFileLogging = 1 << 6;
  static const int kFlagOnlySaveFileOnError = 1 << 7;

  static const int kDirectoryFlagRelative = 1 << 0;
  static const int kDirectoryFlagTemp = 1 << 1;
  static const int kDirectoryFlagBuildDirChild = 1 << 2;

  struct Directory {
    string path;
    int flags;
  };

  static auto CreateDefault(EngineState state) -> shared_ptr<Config>;

  inline Config();

  ~Config() = default;

  inline auto operator=(const Config &) -> Config &;

  inline auto is_enabling_timestamps() const -> bool {
    return (kFlagEnableTimestamps & flags_) != 0;
  }

  inline auto is_enabling_console_logs() const -> bool {
    return (kFlagEnableConsoleLog & flags_) != 0;
  }

  inline auto is_allowing_console_colors() const -> bool {
    return (kFlagAllowConsoleColors & flags_) != 0;
  }

  inline auto is_allowing_stack_tracing() const -> bool {
    return (kFlagAllowStackTracing & flags_) != 0;
  }

  inline auto is_enabling_file_logs() const -> bool {
    return (kFlagEnableFileLogs & flags_) != 0;
  }

  inline auto is_exporting_structures() const -> bool {
    return (kFlagExportStructured & flags_) != 0;
  }

  inline auto is_using_lazy_file_logging() const -> bool {
    return (kFlagUseLazyFileLogging & flags_) != 0;
  }

  inline auto is_only_saving_files_on_error() const -> bool {
    return (kFlagOnlySaveFileOnError & flags_) != 0;
  }

  inline auto flags() const -> int {
    return flags_;
  }

  // Returns a reference of the configs flags, which is mutable.
  // This method may most commonly be used, to add and remove
  // flags from / to the configuration.
  //
  // To add a flag simply write:
  //    config.mutable_flags() |= Config::kFlagEnableTimestamps;
  inline auto mutable_flags() -> int & {
    return flags_;
  }

  inline auto logging_level_thresold() const -> Level {
    return logging_level_threshold_;
  }

  inline auto logging_level_fallback() const -> Level {
    return logging_level_fallback_;
  }

  inline auto export_file_dir() const -> string {
    return export_file_dir_;
  }

  inline auto export_file_name_pattern() const -> string {
    return export_file_name_pattern_;
  }

  inline auto log_file_dir() const -> string {
    return log_file_dir_;
  }

  inline auto log_file_name_pattern() const -> string {
    return log_file_name_pattern_;
  }

  inline void enable_timestamps(bool state = true);

  inline void enable_console_logs(bool state = true);

  inline void allow_console_colors(bool state = true);

  inline void allow_stack_tracing(bool state = true);

  inline void enable_file_logs(bool state = true);

  inline void export_structures(bool state = true);

  inline void use_lazy_file_logging(bool state = true);

  inline void only_save_files_on_error(bool state = true);

  inline void flags(const int flags) {
    flags_ = flags;
  }

  inline void logging_level_threshold(const Level threshold) {
    logging_level_threshold_ = threshold;
  }

  inline void logging_level_fallback(const Level fallback) {
    logging_level_fallback_ = fallback;
  }

  auto export_file_dir(const Directory &dir) -> bool {
    if (AllowDirName(dir)) {
      export_file_dir_ = dir;
      return true;
    }
    return false;
  }

  auto export_file_name_pattern(const string &fmt) -> bool {
    if (AllowFileFormat(fmt)) {
      export_file_name_pattern_ = fmt;
      return true;
    }
    return false;
  }

  auto log_file_dir(const Directory &dir) -> bool {
    if (AllowDirName(dir)) {
      log_file_dir_ = dir;
      return true;
    }
    return false;
  }

  auto log_file_name_pattern(const string &fmt) -> bool {
    if (AllowFileFormat(fmt)) {
      log_file_name_pattern_ = fmt;
      return true;
    }
    return false;
  }

 private:
  // Checks whether the given string is a valid name for an
  // directory. It does not matter, if the directory actually exists
  // in the file-system or, if it is an absolute path or just relative.
  // Directories should not contain non-alphabetical and non-number
  // symbols apart from dashes, underscores and dots at the first position.
  static auto AllowDirName(Directory dir) -> bool;
  // Checks whether the given format is valid. A file-pattern
  // specifies the log-files format and file-type. Valid patterns
  // either shouldn't contain andy dots, or at no dot
  // at their last char index. They are also not allowed to contain
  // any whitespaces or line-delimiters.
  static auto AllowFileFormat(string format) -> bool;

  int flags_;
  Level logging_level_threshold_;
  Level logging_level_fallback_;
  Directory export_file_dir_;
  Directory log_file_dir_;
  string export_file_name_pattern_;
  string log_file_name_pattern_;
};

struct CxxExceptionDescriptor {
  string name;
  string message;
  CxxTrace *trace;
};

struct CxxTrace {
  vector<CxxMethodDescriptor> descriptors;
};

struct CxxMethodDescriptor {
  string name;
  string return_type;
  string scope_name;
  vector<string> parameter_types;
};

} // namespace logger
} // namespace internal
} // namespace kscript

#endif //KSCRIPT_LOG_H
