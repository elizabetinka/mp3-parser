//
// Created by Елизавета Кравченкова on 08.02.2023.
//
#include "parsing.h"

// -f/--file "path_to_file"(обязательный параметр)
// -a/--all выводит данные всех фреймов
// -d/--data *названия требуемых фреймов*  выводит данные только с требуемых фреймов(если они есть)

void Parsing(int argc, char** argv, Operations& arguments) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 or strcmp(argv[i], "--file") == 0) {
            arguments.being_of_options[Path] = true;
            i++;
            if (i == argc) {
                std::cerr << "Не введен путь к mp3 файлу";
                exit(1);
            }
            arguments.path_to_mp3 = argv[i];
        } else if (strcmp(argv[i], "-a") == 0 or strcmp(argv[i], "--all") == 0) {
            arguments.being_of_options[AllFrames] = true;
        } else if (strcmp(argv[i], "-d") == 0 or strcmp(argv[i], "--data") == 0) {
            arguments.being_of_options[SomeFrames] = true;
            i++;
            while (i < argc) {
                if (argv[i][0] == '-') {
                    break;
                }
                arguments.need_frame.push_back(argv[i]);
                i++;
            }
        } else {
            std::cerr << "unknown option" << std::endl;
            break;
        }

    }
}
