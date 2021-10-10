#include <fstream>
#include "huffman_trie.h"

class archiver {
private:
    const int FILENAME_END=256;
    const int ONE_MORE_FILE=257;
    const int ARCHIVE_END=258;

    huffman_trie::bytecode buffer_;

    void write(huffman_trie::bytecode bc, std::ofstream& out);
    void write(std::bitset<9>bs, std::ofstream& out);
    void write_buffer(std::ofstream& out);

    std::bitset<9>read_nine(std::ifstream);
    bool read_one(std::ifstream);


public:
    void zip(std::vector<std::string>file_paths, std::string archive_name);
    void zip_file(std::string& file_name, std::ofstream& out);

    void unzip(std::ostream& output_stream);
    void unzip_file(std::ofstream out, std::string file_name);
};