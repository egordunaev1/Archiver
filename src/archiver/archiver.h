#include <fstream>
#include <vector>
#include <unordered_map>
#include "huffman_trie.h"
#include "reader.h"
#include "writer.h"

using ull=unsigned long long;

class archiver {
private:
    class node{
    public:
        int val;
        std::shared_ptr<node> l = nullptr;
        std::shared_ptr<node> r = nullptr;
    };

    const int FILENAME_END=256;
    const int ONE_MORE_FILE=257;
    const int ARCHIVE_END=258;

    std::shared_ptr<node> build_trie(std::unordered_map<int, bytecode> codes);

    void zip_file_(std::string& file_name, std::ofstream& out, int eof);
    bool unzip_file_(reader& _reader);

public:
    void zip(std::vector<std::string>file_paths, std::string archive_name);
    void unzip(std::string archive);
};