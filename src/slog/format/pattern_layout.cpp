/*
* Created by suemi on 2017/2/24.
*/

#include <unistd.h>
#include <sstream>

#include "slog/format/pattern_layout.h"
#include "slog/logging/log_level.h"
#include "slog/logging/log_event.h"
#include "slog/logging/logger.h"
#include "slog/logging/log_guard.h"
#include "slog/utils/string_util.h"
#include "slog/utils/time_util.h"
#include "slog/utils/properties.h"

namespace slog {

using namespace detail;

PatternConverter::PatternConverter(const FormattingInfo &info) : min_len_(info.min_len), max_len_(info.max_len),
                                                                 left_align_(info.left_align) {}

PatternConverter::~PatternConverter() {}

LiteralPatternConverter::LiteralPatternConverter(const std::string &str) : PatternConverter(FormattingInfo()),
                                                                           content_(str) {}

void LiteralPatternConverter::Convert(std::ostream &os, LogEvent &log) const {
  os << content_;
}

BasicPatternConverter::BasicPatternConverter(const FormattingInfo &info,
                                             BasicPatternConverter::PatternConverterType type) : PatternConverter(info),
                                                                                                 converter_type_(
                                                                                                     type) {}

void BasicPatternConverter::Convert(std::ostream &os, LogEvent &log) const {
  switch (converter_type_) {
    case LOG_LEVEL_CONVERTER:
      os << slog::ToString(log.log_level());
      break;
    case MESSAGE_CONVERTER:
      os << log.message();
      break;
    case NEWLINE_CONVERTER:
      os << '\n';
      break;
    case PROCESS_CONVERTER: {
      int pid = 0;
      pid = (int) ::getpid();
      os << StringUtil::ConvertInt(pid);
    }
      break;

    default:
      break; // Do Nothing
  }
}

LoggerPatternConverter::LoggerPatternConverter(const FormattingInfo &info, int precision) : PatternConverter(info),
                                                                                            precision_(precision) {}

void LoggerPatternConverter::Convert(std::ostream &os, LogEvent &log) const {
/*
  auto ptr = log.logger();

  if (ptr == nullptr) return;
  const std::string name = ptr->name();
*/
  // TODO

  const std::string &name = "root";

  if (precision_ <= 0) {
    os << name;
    return;
  }
  size_t len = name.length();

  auto end = len - 1;
  for (int i = precision_; i > 0; --i) {
    end = name.rfind('.', end - 1);
    if (end == std::string::npos) {
      os << name;
      return;
    }
  }
  os << name.substr(end + 1);
}

DatePatternConverter::DatePatternConverter(const FormattingInfo &info, const std::string &pattern) : PatternConverter(
    info), fmt_(pattern) {}

void DatePatternConverter::Convert(std::ostream &os, LogEvent &log) const {
  os << TimeUtil::Format(log.time(), fmt_);
}

EnvPatternConverter::EnvPatternConverter(const FormattingInfo &info, const std::string &env) : PatternConverter(info),
                                                                                               env_key_(env) {}

void EnvPatternConverter::Convert(std::ostream &os, LogEvent &log) const {
  std::string tmp;
  if (!StringUtil::Environment(tmp, env_key_))
    tmp.clear();
  os << tmp;
}

PatternParser::PatternParser(const std::string &pattern) : pattern_(pattern), state_(LITERAL_STATE), pos_(0) {}

static const char ESCAPE_CHAR = '%';

PatternConverterList PatternParser::Parse() {
  char cur;
  pos_ = 0;
  while (pos_ < pattern_.length()) {
    cur = pattern_[pos_++];
    switch (state_) {
      case LITERAL_STATE :
        // In literal state, the last char is always a literal.
        if (pos_ == pattern_.length()) {
          current_literal_ += cur;
          continue;
        }
        if (cur == ESCAPE_CHAR) {
          // peek at the next char.
          switch (pattern_[pos_]) {
            case ESCAPE_CHAR:
              current_literal_ += cur;
              pos_++; // move pointer
              break;
            default:
              if (!current_literal_.empty()) {
                converters_.push_back(std::make_shared<LiteralPatternConverter>(current_literal_));
              }
              current_literal_.resize(0);
              current_literal_ += cur; // append %
              state_ = CONVERTER_STATE;
              fmt_info_.Reset();
          }
        } else {
          current_literal_ += cur;
        }
        break;

      case CONVERTER_STATE:
        current_literal_ += cur;
        switch (cur) {
          case '-':
            fmt_info_.left_align = true;
            break;
          case '.':
            state_ = DOT_STATE;
            break;
          default:
            if (cur >= '0' && cur <= '9') {
              fmt_info_.min_len = cur - '0';
              state_ = MIN_STATE;
            } else {
              FinalizeConverter(cur);
            }
        } // switch
        break;

      case MIN_STATE:
        current_literal_ += cur;
        if (cur >= '0' && cur <= '9') {
          fmt_info_.min_len = fmt_info_.min_len * 10 + (cur - '0');
        } else if (cur == '.') {
          state_ = DOT_STATE;
        } else {
          FinalizeConverter(cur);
        }
        break;

      case DOT_STATE:
        current_literal_ += cur;
        if (cur >= '0' && cur <= '9') {
          fmt_info_.max_len = cur - '0';
          state_ = MAX_STATE;
        } else {
          std::ostringstream buf;
          buf << "Error occured in position "
              << pos_ << ".\n Was expecting digit, instead got char "
              << cur;
          LogGuard::Instance()->Error(buf.str());
          state_ = LITERAL_STATE;
        }
        break;

      case MAX_STATE:
        current_literal_ += cur;
        if (cur >= '0' && cur <= '9')
          fmt_info_.max_len = fmt_info_.max_len * 10 + (cur - '0');
        else {
          FinalizeConverter(cur);
          state_ = LITERAL_STATE;
        }
        break;
    } // end switch
  } // end while

  if (!current_literal_.empty()) {
    converters_.push_back(std::make_shared<LiteralPatternConverter>(current_literal_));
  }

  return converters_;
}

std::string PatternParser::ExtractOption() {
  using namespace std;
  if ((pos_ < pattern_.length()) && (pattern_[pos_] == '{')) {
    string::size_type end = pattern_.find_first_of('}', pos_);
    if (end != string::npos) {
      string r = pattern_.substr(pos_ + 1, end - pos_ - 1);
      pos_ = end + 1;
      return r;
    } else {
      ostringstream buf;
      buf << "No matching '}' found in conversion pattern string " << pattern_;
      LogGuard::Instance()->Error(buf.str());
      pos_ = pattern_.length();
    }
  }

  return "";
}


int PatternParser::ExtractPrecisionOption() {
  std::string opt = ExtractOption();
  int r = 0;
  if (!opt.empty())
    r = std::atoi(opt.c_str());

  return r;
}

void PatternParser::FinalizeConverter(char c) {
  std::shared_ptr<PatternConverter> pc = nullptr;
  switch (c) {

    case 'c':
      pc = std::make_shared<LoggerPatternConverter>(fmt_info_, ExtractPrecisionOption());
      break;

    case 'd':
    case 'D': {
      std::string opt = ExtractOption();
      if (opt.empty()) {
        opt = "%Y-%m-%d %H:%M:%S";
      }
      pc = std::make_shared<DatePatternConverter>(fmt_info_, opt);
    }
      break;

    case 'E':
      pc = std::make_shared<EnvPatternConverter>(fmt_info_, ExtractOption());
      break;

    case 'i':
      pc = std::make_shared<BasicPatternConverter>(fmt_info_, BasicPatternConverter::PROCESS_CONVERTER);
      break;

    case 'm':
      pc = std::make_shared<BasicPatternConverter>(fmt_info_, BasicPatternConverter::MESSAGE_CONVERTER);
      break;

    case 'n':
      pc = std::make_shared<BasicPatternConverter>(fmt_info_, BasicPatternConverter::NEWLINE_CONVERTER);
      break;

    case 'p':
      pc = std::make_shared<BasicPatternConverter>(fmt_info_, BasicPatternConverter::LOG_LEVEL_CONVERTER);
      break;

    default:
      std::ostringstream buf;
      buf << "Unexpected char [" << c
          << "] at position " << pos_
          << " in conversion pattern.";
      LogGuard::Instance()->Error(buf.str());
      pc = std::make_shared<LiteralPatternConverter>(current_literal_);
  }

  converters_.push_back(pc);
  current_literal_.resize(0);
  state_ = LITERAL_STATE;
  fmt_info_.Reset();
}

PatternLayout::PatternLayout(const std::string &pattern) : pattern_(pattern) {
  Init();
}

PatternLayout::PatternLayout(const Properties &properties) : Layout(properties) {
  if (properties.Exists("pattern")) {
    pattern_ = properties.GetProperty("pattern");
  } else {
    LogGuard::Instance()->Error("ConversionPattern not specified in properties", true);
  }
  Init();
}

void PatternLayout::Reformat(LogEvent &log) {
  auto &buf = log.buffer();
  std::ostringstream oss;
  for (auto converter:converters_)
    converter->Convert(oss, log);
  buf.Reset();
  log << oss.str();
}

void PatternLayout::Init() {
  converters_ = PatternParser(pattern_).Parse();

  for (auto it = converters_.begin(); it != converters_.end(); ++it) {
    if ((*it) == nullptr) {
      LogGuard::Instance()->Error("Parsed Pattern created a NULL PatternConverter");
      (*it) = std::make_shared<LiteralPatternConverter>("");
    }
  }
  if (pattern_.empty()) {
    LogGuard::Instance()->Error("PatternLayout pattern is empty.  Using default...");
    converters_.push_back(
        std::make_shared<BasicPatternConverter>(FormattingInfo(), BasicPatternConverter::MESSAGE_CONVERTER));
  }
}

}
