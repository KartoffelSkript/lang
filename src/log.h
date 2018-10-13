// Copyright 2018 the Kartoffelskript project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef KSCRIPT_LOG_H
#define KSCRIPT_LOG_H

#include <sstream>
#include "kscript.h"

namespace kscript {

class Logger;
class Loggable;

class StringWriter;
class StreamStringWriter;

// Provides formatted and filtered logging functionality.
//
// Example Initialization:
//   StreamStringWriter out(std::cout);
//   Logger log(out, "MyLogger ", Logger::Level::kInfo);
//
// Example Usage:
//   FILE* files;
//   int num_files;
//   ioutil::ListFilesInPath("../src", &files, &num_files);
//
//   if (!num_files) {
//     log.Error("Found no files in the directory");
//   } else {
//     log.Info("Found %d files in the directory.", num_files);
//   }
//
// For multi-threaded environments, the delegated StringWriter
// implementation should provide sufficient synchronization.
class Logger {

  // Used size to initialize the messages char buffer
  // for formatting. To prevent bad formatting, the
  // length of a message should not exceed this value.
  static const size_t kMessageBufSize = 256;

 public:

  // Levels carry the importance and topic of a logged message.
  // There are several different types of information that can
  // be logged and each of them has its very own importance.
  // For that reason, each message has an assigned level.
  // The Logger itself uses a threshold-level to determine
  // whether a message has enough importance.
  //
  // The enumerators are ordered ascending by their importance.
  enum Level {
    kDebug,   // Debugging methods usually require additional cli-flags.
    kInfo,    // Are used for ignorable, neutral information.
    kWarning, // Differ based on the engines state: compiler, runtime.
    kError,   // Failures that are not classified as severe.
    kSevere   // Severe failures that cause the application to shut down.
  };

  inline explicit Logger(StringWriter *output, std::string prefix, Level threshold_level = kWarning);

  inline Logger &operator=(const Logger &log);

  inline ~Logger();

  // Formats message based on the varargs and then logs it.
  template<typename ...TArgs>
  inline void Log(Level level, const std::string &message, TArgs ...args) const;

  // Prints the object to the loggers output by using its
  // polymorphic 'WriteToLogger' method.
  inline void LogObject(Level level, Loggable &loggable) const;

  // Prints the constant object to the loggers output by
  // using its polymorphic 'WriteToLogger' method.
  inline void LogObject(Level level, const Loggable &loggable) const;

  // Formats the string and logs it as a debug message.
  //
  // This method is equivalent to:
  //    Logger* logger = ...;
  //    logger->Log(Logger::Level::kDebug, "Some debug message");
  //
  template<typename ...TArgs>
  inline void Debug(const std::string &message, TArgs ...args);

  // Formats the string and logs it as an info message.
  //
  // This method is equivalent to:
  //    Logger* logger = ...;
  //    logger->Log(Logger::Level::kInfo, "Some information");
  //
  template<typename ...TArgs>
  inline void Info(const std::string &message, TArgs ...args);

  // Formats the string and logs it as a warning message.
  //
  // This method is equivalent to:
  //    Logger* logger = ...;
  //    logger->Log(Logger::Level::kWarning, "Some warning");
  //
  template<typename ...TArgs>
  inline void Warning(const std::string &message, TArgs ...args);

  // Formats the string and logs it as an error message.
  //
  // This method is equivalent to:
  //    Logger* logger = ...;
  //    logger->Log(Logger::Level::kError, "Some error");
  //
  template<typename ...TArgs>
  inline void Error(const std::string &message, TArgs ...args);

  // Formats the string, logs it as a severe error message and
  // then shuts down the application gracefully.
  //
  // This method is equivalent to:
  //    Logger* logger = ...;
  //    logger->Log(Logger::Level::kSevere, "Some severe error");
  //
  template<typename ...TArgs>
  inline void Severe(const std::string &message, TArgs ...args);

  // Returns the loggers threshold-level.
  inline const Level threshold_level() const {
    return this->threshold_level_;
  }

  // Returns the loggers logging prefix.
  inline const std::string &prefix() const {
    return this->prefix_;
  }

  // Sets the loggers threshold-level.
  inline void threshold_level(Level level) {
    this->threshold_level_ = level;
  }

  // Sets the loggers logging prefix. If the
  // prefix string is empty, nothing is written.
  inline void prefix(const std::string &prefix) {
    this->prefix_ = prefix;
  }

 private:

  // Appends both the loggers prefix and stated
  // level to the writer and then flushes its stream.
  // Other methods of the Logger call this before
  // they are writing their appending their message.
  inline void WriteLogPrefix(Level level) const;

  // To log a message, the importance of the level
  // associated with the message must be at least
  // as high as this threshold.
  Level threshold_level_;
  // Prefix of every message, written by the logger.
  // If the value is not empty, it is advisable to
  // suffix it with a space or any other delimiter.
  std::string prefix_;
  // Writer used to actually log messages.
  // Do use a null-obj impl instead of nullptr.
  StringWriter *output_;
};

// Object that can be written to the Logger. Uses the
// Visitor pattern to allow logging of user-made objects.
class Loggable {
 protected:

  // Writes the objects contents to the loggers writer.
  virtual void WriteToLogger(const StringWriter &writer) const = 0;

};

// Abstract class that writes strings to an output.
class StringWriter {
 public:

  // Writes the string to the output.
  virtual void Write(const std::string &str) const = 0;

  // Flushes the output. If the output can't
  // be flushed, nothing will be done.
  virtual void Flush() const = 0;

  // Alias of the Write method.
  inline void operator<<(const std::string &str) const {
    Write(str);
  }

  // First writes the string and then flushes the output.
  // Can be thought of as an appended 'endl' without line-break.
  inline void WriteAndFlush(const std::string &str) const {
    Write(str);
    Flush();
  }
};

// Implementation of the StringWriter that utilises
// the 'ostream' of C's stdio lib to write chars to.
class StreamStringWriter final : public StringWriter {
 public:

  inline explicit StreamStringWriter(std::ostream &stream);

  inline ~StreamStringWriter();

  void Write(const std::string &str) const override;

  void Flush() const override;

 private:
  // Delegated ostream that strings are written to.
  std::ostream &stream_;
};

} // namespace kscript

#endif //KSCRIPT_LOG_H
