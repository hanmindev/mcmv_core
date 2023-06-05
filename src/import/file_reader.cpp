#include <stdexcept>
#include "file_reader.h"

bool FileReader::open_file(string path) {
  this->file = fopen(path.c_str(), "r");
    return this->file != nullptr;
}


void FileReader::fill_buffer() {
    remaining_buffer = fread(this->buffer, sizeof(char), BUFFER_SIZE, this->file);
    buffer_index = 0;

}

FileReader::FileReader(string path) {
    this->path = std::move(path);
    this->buffer_index = BUFFER_SIZE;
    if (!this->open_file(this->path)) {
        throw std::runtime_error("Could not open file " + this->path);
    }
}


char FileReader::read_char() {
    if (remaining_buffer == 0 && buffer_index != BUFFER_SIZE) {
        fclose(this->file);
        return EOF;
    }

    if (buffer_index == BUFFER_SIZE) {
        this->fill_buffer();
    }
    remaining_buffer--;
    return buffer[buffer_index++];
}