#include "folder_sync.h"

namespace fs = std::filesystem;

folder_sync::folder_sync(fs::path _source, fs::path _target) {
    source = _source;
    target = _target;
}

std::wstring folder_sync::get_perms(fs::perms pf) {
    std::wstring perms;
    perms += ((pf & fs::perms::owner_read) != fs::perms::none ? L"r" : L"-");
    perms += ((pf & fs::perms::owner_write) != fs::perms::none ? L"w" : L"-");
    perms += ((pf & fs::perms::owner_exec) != fs::perms::none ? L"x" : L"-");
    perms += ((pf & fs::perms::group_read) != fs::perms::none ? L"r" : L"-");
    perms += ((pf & fs::perms::group_write) != fs::perms::none ? L"w" : L"-");
    perms += ((pf & fs::perms::group_exec) != fs::perms::none ? L"x" : L"-");
    perms += ((pf & fs::perms::others_read) != fs::perms::none ? L"r" : L"-");
    perms += ((pf & fs::perms::others_write) != fs::perms::none ? L"w" : L"-");
    perms += ((pf & fs::perms::others_exec) != fs::perms::none ? L"x" : L"-");
    return perms;
}

void folder_sync::demo_info(const fs::path &path, const fs::path &parent) {
    std::wcout << L"Full path: " << path.wstring() << std::endl;
    std::wcout << L"Path: " << fs::relative(path, parent).wstring() << std::endl;
    std::wcout << L"Perms: " << get_perms(fs::status(path).permissions()) << std::endl;
    if (fs::is_regular_file(path)) {
        std::wcout << "Size: " << fs::file_size(path) << " bytes" << std::endl;
    }
    // Uncomment, if compiler supports C++20
    //    fs::file_time_type ftime = fs::last_write_time(path);
    //    std::time_t cftime = std::chrono::system_clock::to_time_t(std::chrono::time_point::to_sys(ftime));
    //    std::wcout << "Last modified: " << std::put_time(std::localtime(&cftime), L"%Y.%m.%d %H:%M:%S") << std::endl;
}

void folder_sync::print_paths(const std::vector<fs::path> &paths, const fs::path &parent) {
    for (const auto &elem : paths) {
        std::wcout << L"------------------" << std::endl;
        demo_info(elem, parent);
    }
}

void folder_sync::analyze_paths() {
    source_paths = get_paths(source);
    target_paths = get_paths(target);

    for (auto it_t = target_paths.begin(); it_t != target_paths.end(); ++it_t) {
        for (auto it_s = source_paths.begin();; ++it_s) {
            if (it_s == source_paths.end()) {
                uniq_for_target.push_back(*it_t);
                break;
            }
            if (fs::relative(*it_s, source) == fs::relative(*it_t, target)) {
                matches.push_back(*it_s);
                break;
            }
        }
    }

    std::wcout << L"*******************Source paths*******************" << std::endl;
    print_paths(source_paths, source);

    std::wcout << L"*******************Unique paths to delete*******************" << std::endl;
    print_paths(uniq_for_target, target);
}

void folder_sync::run_sync() {
    remove();
    copy();
}

std::vector<fs::path> folder_sync::get_paths(const fs::path &_path) {
    std::vector<fs::path> paths;
    for (const auto &entry : fs::recursive_directory_iterator(_path)) {
        if (fs::is_regular_file(entry.path()) || fs::is_directory(entry.path())) {
            paths.push_back(entry.path());
        }
    }
    return paths;
}

void folder_sync::remove() {
    for (const auto &elem : uniq_for_target) {
        std::wcout << std::left << std::setw(50) << std::setfill(L'-') << elem.wstring();

        bool is_removed = false;

        try {
            is_removed = fs::remove(elem);
        } catch (fs::filesystem_error &e) {
            std::wcout << e.what();
        }

        if (is_removed) {
            std::wcout << L" Removed" << std::endl;
        } else {
            std::wcout << L" Not removed" << std::endl;
        }
    }
}

void folder_sync::copy() {
    for (const auto &elem : source_paths) {
        fs::path new_path = target.wstring() + L"\\" + fs::relative(elem, source).wstring();
        if (fs::is_directory(elem)) {
            fs::create_directory(new_path);
        } else if (fs::is_regular_file(elem)) {
            std::wcout << std::left << std::setw(50) << std::setfill(L'-') << elem.wstring();

            bool is_copied = false;

            try {
                is_copied = fs::copy_file(elem, new_path, fs::copy_options::update_existing);
            } catch (fs::filesystem_error &e) {
                std::wcout << e.what();
            }

            if (is_copied) {
                std::wcout << L" Copied" << std::endl;
            } else {
                std::wcout << L" Not copied" << std::endl;
            }
        }
    }
}