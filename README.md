#slog

一个轻量级C++多线程日志库

---

### 基本思路

- 架构方面主要参考log4j的设计
- 延迟刷写日志的思路主要参考muduo的日志实现

### 配置示例

```
# logger的配置

slog.logger.root.layout=SimpleLayout
slog.logger.root.filter=AcceptAllFilter

slog.logger.app.layout=PatternLayout
slog.logger.app.layout.pattern=%d - %p - %m %n

slog.logger.app.filter=AndFilter
slog.logger.app.filter.1=LevelMatchFilter
slog.logger.app.filter.1.logLevelToMatch=INFO
slog.logger.app.filter.2=AcceptAllFilter

# Appender配置

slog.appender.consoleAppender=ConsoleAppender

slog.appender.fileAppender=FileAppender
slog.appender.fileAppender.path=./test/slog/resources/log/log.txt
slog.appender.fileAppender.createDirs=true
slog.appender.fileAppender.reopenDelay=3

# scheduler的配置

slog.scheduler.flushInterval=3

```

### 使用示例

```cpp
#include <slog.h>

using namespace slog;
using namespace std;

int main() {

  // 初始化读入配置
  PropertyConfigurator conf("./test/slog/resources/slog.properties");
  conf.Configure();
  LoggerFactory::Init(conf);
  
  // 获取选定的logger，未配置的logger会依据root的配置自动生成
  auto root_logger = LoggerFactory::GetLogger();
  auto tmp_logger = LoggerFactory::GetLogger("tmp");
  
  // 使用宏记录日志
  LOG_INFO(tmp_logger)<<"Hello,World!";
  
  return 0;
}
```

### 输出示例

```
[2017-03-10 17:08:43][WARN](/Users/suemi/Workspace/slog/test_main.cpp:38 main) - 0x700007a10000 : Hello,World! 0     

[2017-03-10 17:08:43][WARN](/Users/suemi/Workspace/slog/test_main.cpp:38 main) - 0x700007887000 : Hello,World! 1     
```

### 后续改进

- 看门狗和热更新
- 从全局单线程刷入文件到每个logger拥有一个scheduler
- 支持网络的appender