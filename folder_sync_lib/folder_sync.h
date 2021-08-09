#ifndef SYNC_FILES_FOLDER_SYNC_H
#define SYNC_FILES_FOLDER_SYNC_H

#include "folder_sync.h"
#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <iomanip>

namespace fs = std::filesystem;

class folder_sync {
public:
    folder_sync(fs::path _source, fs::path _target);

    void analyze_paths();

    void run_sync();

private:
    fs::path source;
    fs::path target;
    std::vector<fs::path> source_paths;
    std::vector<fs::path> target_paths;

    std::vector<fs::path> newer_paths;
    std::vector<fs::path> uniq_for_target;
    std::vector<fs::path> uniq_for_source;

    std::vector<fs::path> get_paths(const fs::path &_path);

    std::vector<fs::path> get_unique(const std::vector<fs::path> &_target, const fs::path &_target_parent,
                                     const std::vector<fs::path> &_other, const fs::path &_other_parent);

    std::vector<fs::path> get_newer(const std::vector<fs::path> &_first, const fs::path &_first_parent,
                                    const std::vector<fs::path> &_second, const fs::path &_second_parent);

    std::wstring get_perms(fs::perms pf);

    void demo_info(const fs::path &path, const fs::path &parent);

    void print_paths(const std::vector<fs::path> &paths, const fs::path &parent);

    void remove(const std::vector<fs::path> &_paths);

    void copy(const std::vector<fs::path> &from_copy, const fs::path &to_parent);
};

#endif //SYNC_FILES_FOLDER_SYNC_H