#include <iostream>
#include <string>
#include <filesystem>
#include <io.h>
#include <fcntl.h>
#include <vector>

#include "folder_sync_lib/folder_sync.h"

namespace fs = std::filesystem;

int main() {
//    std::setlocale(LC_ALL, "");
    _setmode(_fileno(stdout), _O_U16TEXT);

    fs::path source = L"F:\\DEV\\TEST\\sou";
    fs::path target = L"F:\\DEV\\TEST\\tar";

    folder_sync profile(source, target);
    profile.analyze_paths();

    char start = ' ';

    std::wcout << L"Do you want to sync files? [y/n]" << std::endl;
    std::cin >> start;

    if(start == 'y')
        profile.run_sync();

    return EXIT_SUCCESS;
}
