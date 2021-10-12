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

    static const int FILENAME_END=256;
    static const int ONE_MORE_FILE=257;
    static const int ARCHIVE_END=258;

    static bool unzip_body(const std::shared_ptr<archiver::node>& root, reader& _reader, writer& _writer);
    static std::string read_filename(std::shared_ptr<archiver::node> root, reader& _reader);

    static int read_code(std::shared_ptr<archiver::node> root, reader& _reader);
    static std::shared_ptr<node> build_trie(std::unordered_map<int, bytecode> codes);

    void zip_file_(std::string& file_name, std::ofstream& out, int eof);
    bool unzip_file_(reader& _reader);

public:
    void zip(std::vector<std::string>file_paths, const std::string& archive_name);
    void unzip(const std::string& archive);
};