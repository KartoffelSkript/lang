// Copyright 2018 the Kartoffelskript project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <cstring>
#include "log.h"

namespace kscript {

// Returns the levels name.
inline const char *LoggerLevelName(Logger::Level level) {
  // Integer map containing the levels names.
  // NOTE: Keep this synchronized with the amount
  //       and order of Logger enum fields.
  static const char *level_names[] = {
      "Debug", "Info", "Warning", "Error", "Severe"
  };

  return level_names[level];
}

Logger::Logger(StringWriter *output, const std::string prefix, Level threshold_level)
    : output_(output),
      prefix_(prefix),
      threshold_level_(threshold_level) {}

Logger::~Logger() {
  delete (this->output_);
}

template<typename ...TArgs>
void Logger::Log(Level level, const std::string &message, TArgs...args) const {
  if (threshold_level_ >= level) {
    // The levels importance is to low, in order for it to be logged.
    return;
  }

  WriteLogPrefix(level);

  char msg_buf[kMessageBufSize];
  sprintf(msg_buf, message.c_str(), args...);
  output_->Write(msg_buf);
  output_->WriteAndFlush("\n");
}

void Logger::LogObject(Level level, Loggable &loggable) const {
  WriteLogPrefix(level);

  loggable.WriteToLogger(*output_);
  output_->Flush();
}

void Logger::LogObject(Level level, const Loggable &loggable) const {
  WriteLogPrefix(level);

  loggable.WriteToLogger(*output_);
  output_->Flush();
}

// Make sure to flush writer when calling this.
void Logger::WriteLogPrefix(Level level) const {
  output_->Write(prefix_);
  output_->Write(LoggerLevelName(level));
  output_->Write(": ");
}

template<typename ...TArgs>
void Logger::Debug(const std::string &message, TArgs... args) {
  Log(Level::kDebug, message, args...);
}

template<typename ...TArgs>
void Logger::Info(const std::string &message, TArgs... args) {
  Log(Level::kInfo, message, args...);
}

template<typename ...TArgs>
void Logger::Warning(const std::string &message, TArgs... args) {
  Log(Level::kWarning, message, args...);
}

template<typename ...TArgs>
void Logger::Error(const std::string &message, TArgs... args) {
  Log(Level::kError, message, args...);
}

template<typename ...TArgs>
void Logger::Severe(const std::string &message, TArgs... args) {
  Log(Level::kSevere, message, args...);
}

StreamStringWriter::StreamStringWriter(std::ostream &stream)
    : stream_(stream) {}

StreamStringWriter::~StreamStringWriter() {
  Flush();
}

void StreamStringWriter::Write(const std::string &str) const {
  stream_ << str;
}

void StreamStringWriter::Flush() const {
  stream_.flush();
}

} // namespace kscript