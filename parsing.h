//
// Created by Елизавета Кравченкова on 08.02.2023.
//
#pragma once

#include <iostream>
#include <cstring>
#include <vector>

enum Options {
    Path = 0, AllFrames = 1, SomeFrames = 2
};

struct Operations {
    std::string path_to_mp3;
    std::vector<char*> need_frame;
    bool being_of_options[3] = {false, false, false};
};


void Parsing(int argc, char** argv, Operations& arguments);

