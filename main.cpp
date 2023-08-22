#include "parsing.h"
#include "MP3parse.h"

int main(int argc, char** argv) {
    struct Operations arguments;
    Parsing(argc, argv, arguments);
    if (!arguments.being_of_options[Path]) {
        std::cerr << "Не введен путь к mp3 файлу";
        exit(1);
    }
    MP3parse parser(arguments.path_to_mp3);
    parser.Parsing();
    if (arguments.being_of_options[AllFrames]) {
        parser.PrintAllDataParse();
    }
    else if (arguments.being_of_options[SomeFrames]) {
        parser.PrintSomeDataParse(arguments.need_frame);
    }

    return 0;
}
