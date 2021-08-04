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
    std::vector<fs::path> matches;
    std::vector<fs::path> uniq_for_target;


    std::wstring get_perms(fs::perms pf);

    void demo_info(const fs::path &path, const fs::path &parent);

    void print_paths(const std::vector<fs::path> &paths, const fs::path &parent);

    std::vector<fs::path> get_paths(const fs::path &_path);

    void remove();

    void copy();
};

#endif //SYNC_FILES_FOLDER_SYNC_H