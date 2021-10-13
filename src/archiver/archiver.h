#pragma once

#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

#include "huffman_trie.h"
#include "reader.h"
#include "writer.h"

using ull=unsigned long long;

const int ARCHIVE_END = 258;
const int FILENAME_END = 256;
const int ONE_MORE_FILE = 257;

class archiver {
private:
    class node{
    public:
        int val;
        std::shared_ptr<node> l = nullptr;
        std::shared_ptr<node> r = nullptr;
    };

    static bool unzip_body(const std::shared_ptr<archiver::node>& root, reader& _reader, writer& _writer);
    static std::string read_filename(const std::shared_ptr<archiver::node>& root, reader& _reader);

    static int read_code(std::shared_ptr<archiver::node> root, reader& _reader);
    static std::shared_ptr<node> build_trie(std::unordered_map<int, bytecode> codes);

    static void zip_file_(reader& _reader, writer& _writer, int eof);
    static bool unzip_file_(reader& _reader);

public:
    static void zip(std::vector<std::string> file_paths, const std::string& archive_name);
    static void unzip(const std::string& archive);
};