/*
* Created by suemi on 2017/2/14.
*/

#include <cerrno>
#include <cstdio>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <dirent.h>
#include "slog/utils/file_util.h"
#include "slog/logging/log_guard.h"
#include "slog/utils/string_util.h"

namespace slog {

long const SLOG_FILE_NOT_FOUND = ENOENT;
const char DIR_SEP = '/';

long FileUtil::Touch(const std::string &path) {
  if (Exists(path)) {
    std::stringstream oss;
    LogGuard::Instance()->Warn("File Path : " + path + " Already Exists!");
    return 0;
  }
  std::ofstream dst;
  dst.open(path, std::ios_base::out);
  dst.close();
  return 0;
}

long FileUtil::Delete(const std::string &path) {
  long res = 0;
  if (IsDir(path)) {
    std::vector<std::string> children = ListDir(path);
    for (auto &child:children)
      if ((res = Delete(child)) != 0) break;
  }

  if (res == 0) res = std::remove(path.c_str()) ? errno : 0;

  if (res == 0) return 0;
  else {
    LogGuard* guard = LogGuard::Instance();
    if (res != SLOG_FILE_NOT_FOUND) {
      std::ostringstream oss;
      oss<<"Failed to delete file "<<path<<"; "<<"error: "<<res<<std::endl;
      guard->Error(oss.str());
    }
    return res;
  }
}

long FileUtil::Rename(const std::string &src, const std::string &dst) {
  if(std::rename(src.c_str(),dst.c_str())==0)
    return 0;
  else {
    LogGuard* guard = LogGuard::Instance();
    int res = errno;
    if(res!=0 && res!=SLOG_FILE_NOT_FOUND){
      std::ostringstream oss;
      oss<<"Failed to rename file from "<<src<<" to "<<dst<<" ; "<<"error: "<<res<<std::endl;
      guard->Error(oss.str());
    }
    return res;
  }
}

void FileUtil::RollOverFiles(const std::string &path, unsigned int max_backup) {
  using namespace std;

  // Delete the oldest file
  ostringstream buffer;
  buffer << path << "." << max_backup;
  long ret = Delete(buffer.str());

  ostringstream source_oss;
  ostringstream target_oss;

  // Map {(max_backup - 1), ..., 2, 1} to {max_backup, ..., 3, 2}
  for(int i = max_backup - 1; i >= 1; --i)
  {
    source_oss.str("");
    target_oss.str("");

    source_oss << path << "." << i;
    target_oss << path << "." <<(i+1);

    string const source(source_oss.str());
    string const target(target_oss.str());

    ret = Rename(source, target);
  }
}

bool FileUtil::IsDir(const std::string &path) {
  struct stat path_stat;
  if(stat(path.c_str(),&path_stat)!=0)
    return false;
  return S_ISDIR(path_stat.st_mode);
}

std::vector<std::string> FileUtil::ListDir(const std::string &path) {
  std::vector<std::string> res;
  if (!IsDir(path)) return res;
  std::string prefix = *(path.rbegin()) == '/' ? path : path + "/";
  struct dirent *ptr = nullptr;
  DIR *dir = opendir(path.c_str());
  while ((ptr = readdir(dir)) != nullptr) {
    if (!strcmp(ptr->d_name, ".")) continue;
    if (!strcmp(ptr->d_name, "..")) continue;
    res.push_back(prefix + std::string(ptr->d_name));
  }
  return res;
}

std::string FileUtil::ParentDir(const std::string &path) {
  auto path_end = path[path.length() - 1] == DIR_SEP ? (path.rbegin() + 1).base() : path.end();
  std::string tmp(path.begin(),path_end);
  auto last_sep_index = tmp.find_last_of(DIR_SEP);
  return last_sep_index==0?"/":tmp.substr(0,last_sep_index);
}

std::string FileUtil::CurrentDir() {
  char cwd[1024];
  if(getcwd(cwd, sizeof(cwd))!= nullptr)
    return std::string(cwd);
  else
    return std::string();
}

bool FileUtil::Exists(const std::string &path) {
  struct stat file_status;
  if(stat(path.c_str(),&file_status)==-1)
    return false;
  return true;
}

std::string FileUtil::ExpandRelativePath(const std::string &path) {
  auto current_dir = CurrentDir();
  auto components = StringUtil::Split(current_dir, '/');
  auto it = path.begin();
  for (; it != path.end() && (*it == '.' || *it == '/'); ++it) {
    if (it + 1 == path.end()) continue;
    if (*(it + 1) == '.') {
      if (components.size() > 1) components.pop_back();
      ++it;
    }
  }

  std::string suffix = std::string(it, path.end());
  components.push_back(suffix);
  return StringUtil::Join(components.begin(), components.end(), "/");
  //return *(current_dir.rbegin())=='/'?current_dir+path:current_dir+"/"+path;
}

long FileUtil::MakeDirectory(const std::string &dir) {
  if(mkdir(dir.c_str(),0777) == 0)
    return 0;
  else {
    auto ret = errno;
    std::ostringstream oss;
    oss<<"Failed to create directory "<<dir<<"; error: "<<ret<<std::endl;
    LogGuard::Instance()->Error(oss.str());
    return ret;
  }
}

void FileUtil::MakeDirs(const std::string &path) {
  std::vector<std::string> components = StringUtil::Split(path,DIR_SEP);
  components.pop_back();
  std::string dir_sep;
  std::string cur_path = StringUtil::Join(components.begin(),components.begin()+1,std::string(1,DIR_SEP));
  for(auto it=components.begin()+1;it!=components.end();++it){
    cur_path+=DIR_SEP;
    cur_path+=*it;

    if(Exists(cur_path)) continue;

    const long eno = MakeDirectory(cur_path);
    if(eno!=0) break;
  }

}

}