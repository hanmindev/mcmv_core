#ifndef MCMV_CORE_FILE_READER_H
#define MCMV_CORE_FILE_READER_H

#include <string>
#include <utility>
#include <vector>

#define BUFFER_SIZE 1024

using namespace std;

class FileReader {
private:
    FILE *file = nullptr;
    string path;
    char buffer[BUFFER_SIZE];
    int buffer_index;
    size_t remaining_buffer;

    bool open_file(string path);

    void fill_buffer();
public:
    explicit FileReader(string path);

    char read_char();
};

#endif //MCMV_CORE_FILE_READER_H