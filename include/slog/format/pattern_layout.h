/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_PATTERN_LAYOUT_H
#define SLOG_PATTERN_LAYOUT_H

#include <ostream>
#include <vector>

#include "slog/format/layout.h"

namespace slog {

namespace detail {

struct FormattingInfo {
  int min_len;
  std::size_t max_len;
  bool left_align;

  FormattingInfo() {
    Reset();
  }

  void Reset() {
    min_len = -1;
    max_len = 0x7FFFFFFF;
    left_align = false;
  }
};

/*
 * 用来对LogEvent中某个单独的部分（如日期、位置）做格式化的工具类
 */
class PatternConverter {
public:
  explicit PatternConverter(const FormattingInfo &info);

  virtual ~PatternConverter();

  virtual void Convert(std::ostream &os, LogEvent &log) const = 0;

private:
  int min_len_;
  size_t max_len_;
  bool left_align_;
};

/*
 * 为日志增加或替换特定的常量内容
 */
class LiteralPatternConverter : public PatternConverter {
public:
  LiteralPatternConverter(const std::string &str);

  virtual void Convert(std::ostream &os, LogEvent &log) const override;

private:
  std::string content_;
};

class BasicPatternConverter : public PatternConverter {

public:
  enum PatternConverterType {
    PROCESS_CONVERTER,
    LOG_LEVEL_CONVERTER,
    MESSAGE_CONVERTER,
    NEWLINE_CONVERTER,
  };

  BasicPatternConverter(const FormattingInfo &info, PatternConverterType type);

  virtual void Convert(std::ostream &os, LogEvent &log) const override;

private:
  PatternConverterType converter_type_;
};

/*
 * 对日志中和记录的logger相关的信息进行格式化
 */
class LoggerPatternConverter : public PatternConverter {
public:
  LoggerPatternConverter(const FormattingInfo &info, int precision);

  virtual void Convert(std::ostream &os, LogEvent &log) const override;

private:
  int precision_;
};

/*
 * 日期相关的格式化工具
 */
class DatePatternConverter : public PatternConverter {
public:
  DatePatternConverter(const FormattingInfo &info, const std::string &pattern);

  virtual void Convert(std::ostream &os, LogEvent &log) const override;

private:
  std::string fmt_;
};

/*
 * 格式化环境变量
 */
class EnvPatternConverter : public PatternConverter {
public:
  EnvPatternConverter(const FormattingInfo &info, const std::string &env);

  virtual void Convert(std::ostream &os, LogEvent &log) const override;

private:
  std::string env_key_;
};


using PatternConverterList = std::vector<std::shared_ptr<PatternConverter>>;

/*
 * 将模式字符串解析成一个个Converter
 */
class PatternParser {
public:

  PatternParser(const std::string &pattern);

  PatternConverterList Parse();

private:
  enum ParserState {
    LITERAL_STATE,
    CONVERTER_STATE,
    DOT_STATE,
    MIN_STATE,
    MAX_STATE
  };

  /*
 * 提取{}之间的选项字符串
 */
  std::string ExtractOption();

  /*
 * 将选项解析为数字
 */
  int ExtractPrecisionOption();

  void FinalizeConverter(char c);

  std::string pattern_;
  FormattingInfo fmt_info_;
  PatternConverterList converters_;
  ParserState state_;
  std::string::size_type pos_;
  std::string current_literal_;
};

}

class PatternLayout : public Layout {
public:
  PatternLayout(const std::string &pattern);

  PatternLayout(const Properties& properties);

  virtual void Reformat(LogEvent &log) override;

protected:
  virtual void Init();

  std::string pattern_;
  detail::PatternConverterList converters_;
};

}

#endif 
