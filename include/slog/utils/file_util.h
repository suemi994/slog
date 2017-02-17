/*
* Created by suemi on 2017/2/13.
*/

#ifndef SLOG_FILE_UTIL_H
#define SLOG_FILE_UTIL_H

#include <string>

namespace slog {

class FileUtil {
public:
  static long Delete(const std::string& path);

  static long Rename(const std::string& src, const std::string& dst);

  static void RollOverFiles(const std::string& path, unsigned int max_backup);

  static bool IsDir(const std::string& path);

  static std::string ParentDir(const std::string& path);

  static std::string CurrentDir();

  static bool Exists(const std::string &path);

  static std::string ExpandRelativePath(const std::string& path);

  static long MakeDirectory(const std::string& dir);

  static void MakeDirs(const std::string& path);



};

}

#endif 
