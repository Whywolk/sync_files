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

std::vector<fs::path> folder_sync::get_paths(const fs::path &_path) {
    std::vector<fs::path> paths;
    for (const auto &entry : fs::recursive_directory_iterator(_path)) {
        if (fs::is_regular_file(entry.path())) {
            paths.push_back(entry.path());
        }
    }
    return paths;
}

std::vector<fs::path> folder_sync::get_unique(const std::vector<fs::path> &_target, const fs::path &_target_parent,
                                              const std::vector<fs::path> &_other, const fs::path &_other_parent){
    std::vector<fs::path> uniq;
    for (auto it_t = _target.begin(); it_t != _target.end(); ++it_t) {
        for (auto it_o = _other.begin();; ++it_o) {
            if (it_o == _other.end()) {
                uniq.push_back(*it_t);
                break;
            }
            if (fs::relative(*it_t, _target_parent) == fs::relative(*it_o, _other_parent)) {
                break;
            }
        }
    }
    return uniq;
}

std::vector<fs::path> folder_sync::get_newer(const std::vector<fs::path> &_first, const fs::path &_first_parent,
                                             const std::vector<fs::path> &_second, const fs::path &_second_parent){
    std::vector<fs::path> newer;
    for (const auto &f : _first) {
        for (const auto &s : _second) {
            if ((fs::relative(f, _first_parent) == fs::relative(s, _second_parent)) &&
                (fs::last_write_time(f) > fs::last_write_time(s))) {
                newer.push_back(f);
                break;
            }
        }
    }
    return newer;
}

void folder_sync::analyze_paths() {
    source_paths = get_paths(source);
    target_paths = get_paths(target);

    newer_paths = get_newer(source_paths, source, target_paths, target);
    uniq_for_source = get_unique(source_paths, source, target_paths, target);
    uniq_for_target = get_unique(target_paths, target, source_paths, source);

    std::wcout << L"*******************Update these paths*******************" << std::endl;
    print_paths(newer_paths, source);

    std::wcout << L"*******************Delete these paths*******************" << std::endl;
    print_paths(uniq_for_target, target);
}

void folder_sync::run_sync() {
    remove(uniq_for_target);
    copy(newer_paths, target);
    copy(uniq_for_source, target);
}

void folder_sync::remove(const std::vector<fs::path> &_paths) {
    for (const auto &elem : _paths) {
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

void folder_sync::copy(const std::vector<fs::path> &from_copy, const fs::path &to_parent) {
    for (const auto &elem : from_copy) {
        fs::path new_path = to_parent.wstring() + L"\\" + fs::relative(elem, source).wstring();
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