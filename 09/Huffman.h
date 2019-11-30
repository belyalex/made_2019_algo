//
// Created by alex on 29.11.2019.
//

#ifndef HAFFMAN_HUFFMAN_H
#define HAFFMAN_HUFFMAN_H

using byte=unsigned char;

struct IInputStream {
    std::ifstream in_;
    IInputStream(char* fname): in_(fname, std::ifstream::in|std::ifstream::binary) {}

    bool Read(byte& value) {
        if (in_.eof()) return false;
        in_.read((char*)&value,sizeof(value));
        return in_.gcount()==sizeof(value);
    }
};

struct IOutputStream {
    std::ofstream out_;
    IOutputStream(char* fname): out_(fname, std::ofstream::out|std::ofstream::binary) {}
    void Write(byte value) {
        out_.write((const char*)&value,sizeof(value));
    }
};

#endif //HAFFMAN_HUFFMAN_H
