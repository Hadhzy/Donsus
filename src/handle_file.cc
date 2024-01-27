// Handling source file
#include <fstream>
#include <iostream>
#include <stdexcept>

std::size_t get_file_size(std::ifstream& file){
    file.seekg(0, file.end); // Go to the end of the file
    std::size_t size = file.tellg(); // Read the current position
    file.seekg(0, file.beg); // Go to the beginning of the file
    return size;
}

std::string handle_file(char **argv) {
    char *path = argv[1]; // get the path of the file
    std::ifstream in(path, std::ifstream::in);

    if(!in) {
        std::cout << "FILE NOT FOUND AT THIS PATH" << std::endl;
        exit(1);
    }

    std::size_t size = get_file_size(in);
    std::string result {};

    result.resize(size);

    std::string str(size, '\0');

    if (in.read(&str[0],  static_cast<int>(size)))
        result = str;

    in.close();
    return result;
}