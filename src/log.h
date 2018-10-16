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
#include "bitmask.h"

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

struct Record;
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
  auto Allow(const Record &entry) const -> bool;
};

class Handler {
 public:
  void Publish(Record &entry) const;
};

class Formatter {

};

class DateFormatter {

};

struct Record {
  string message;
  milliseconds timestamp;
  Level severity;
};

class Config {
 public:
  // Enables a timestamp at the beginning of a log.
  static const int kFlagEnableTimestamps = BITMASK(1);
  // Allows logging of stack traces (CxxTrace).
  static const int kFlagAllowStackTracing = BITMASK(2);
  // Enables runtime logging to the console.
  static const int kFlagEnableConsoleLog = BITMASK(3);
  // Allows colored console output when logging.
  static const int kFlagAllowConsoleColors = BITMASK(4);
  // Enables logging to files. If the lazy-file-logging
  // flag is not set, the logs are written eagerly.
  static const int kFlagEnableFileLogs = BITMASK(5);
  // Enables structured exports of the logs in a xml format.
  static const int kFlagExportStructured = BITMASK(6);

  static const int kFlagUseLazyFileLogging = BITMASK(7);

  static const int kFlagOnlySaveFileOnError = BITMASK(8);

  static const int kDirectoryFlagRelative = BITMASK(1);

  static const int kDirectoryFlagTemp = BITMASK(2);

  static const int kDirectoryFlagBuildDirChild = BITMASK(3);

  struct Directory {
    string path;
    char flags;
  };

  static auto CreateDefault(EngineState state) -> shared_ptr<Config>;

  inline Config();

  ~Config() = default;

  inline auto operator=(const Config &) -> Config &;

  inline auto is_enabling_timestamps() const -> bool {
    return BitmaskCheck(flags_, kFlagEnableTimestamps);
  }

  inline auto is_enabling_console_logs() const -> bool {
    return BitmaskCheck(flags_, kFlagEnableConsoleLog);
  }

  inline auto is_allowing_stack_tracing() const -> bool {
    return BitmaskCheck(flags_, kFlagAllowStackTracing);
  }

  inline auto is_allowing_console_colors() const -> bool {
    return BitmaskCheck(flags_, kFlagAllowConsoleColors);
  }

  inline auto is_enabling_file_logs() const -> bool {
    return BitmaskCheck(flags_, kFlagEnableFileLogs);
  }

  inline auto is_exporting_structures() const -> bool {
    return BitmaskCheck(flags_, kFlagExportStructured);
  }

  inline auto is_using_lazy_file_logging() const -> bool {
    return BitmaskCheck(flags_, kFlagUseLazyFileLogging);
  }

  inline auto is_only_saving_files_on_error() const -> bool {
    return BitmaskCheck(flags_, kFlagOnlySaveFileOnError);
  }

  inline auto flags() const -> const int {
    return flags_;
  }
  // Returns a reference of the configs flags, which is mutable.
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

  inline auto export_file_dir() const -> const Directory & {
    return export_file_dir_;
  }

  inline auto log_file_dir() const -> const Directory & {
    return log_file_dir_;
  }

  inline auto export_file_name_pattern() const -> const string {
    return export_file_name_pattern_;
  }

  inline auto log_file_name_pattern() const -> const string {
    return log_file_name_pattern_;
  }

  inline void enable_timestamps(bool value = true) {
    BitmaskBoolBasedSet(flags_, kFlagEnableTimestamps, value);
  }

  inline void enable_console_logs(bool value = true) {
    BitmaskBoolBasedSet(flags_, kFlagEnableConsoleLog, value);
  }

  inline void allow_console_colors(bool value = true) {
    BitmaskBoolBasedSet(flags_, kFlagAllowConsoleColors, value);
  }

  inline void allow_stack_tracing(bool value = true) {
    BitmaskBoolBasedSet(flags_, kFlagAllowStackTracing, value);
  }

  inline void enable_file_logs(bool value = true) {
    BitmaskBoolBasedSet(flags_, kFlagEnableFileLogs, value);
  }

  inline void export_structures(bool value = true) {
    BitmaskBoolBasedSet(flags_, kFlagExportStructured, value);
  }

  inline void use_lazy_file_logging(bool value = true) {
    BitmaskBoolBasedSet(flags_, kFlagUseLazyFileLogging, value);
  }

  inline void only_save_files_on_error(bool value = true) {
    BitmaskBoolBasedSet(flags_, kFlagOnlySaveFileOnError, value);
  }

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
  static auto AllowDirName(const Directory &dir) -> bool;
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
