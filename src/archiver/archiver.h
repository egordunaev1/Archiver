#include <fstream>
#include "huffman_trie.h"

class archiver {
private:
    const int FILENAME_END=256;
    const int ONE_MORE_FILE=257;
    const int ARCHIVE_END=258;

    huffman_trie::bytecode buffer_;

    void write_(huffman_trie::bytecode bc, std::ofstream& out);
    void write_(std::bitset<9>bs, std::ofstream& out);
    void write_buffer_(std::ofstream& out);

    void fill_buffer(std::ifstream& in);
    std::bitset<9>read_nine_(std::ifstream& in);
    bool read_one_(std::ifstream& in);

    void build_trie();

    void zip_file_(std::string& file_name, std::ofstream& out);
    bool unzip_file_(std::ifstream& in);

public:
    void zip(std::vector<std::string>file_paths, std::string archive_name);
    void unzip(std::string archive);

};