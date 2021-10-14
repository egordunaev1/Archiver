#pragma once

#include <iostream>
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
    using nodeptr=std::shared_ptr<node>;

    // Архивация
    static std::unordered_map<int, ull> count_frequency_(reader& _reader);
    static std::string read_filename_(const std::shared_ptr<archiver::node>& root, reader& _reader);
    static void write_meta_(writer& _writer, reader& _reader, huffman_trie& huffmanTrie);
    static void write_body_(writer& _writer, reader& _reader, huffman_trie& huffmanTrie);
    static void zip_file_(reader& _reader, writer& _writer, int eof);

    // Разархивация
    static std::vector<ull> read_abc_(reader& _reader);
    static std::shared_ptr<node> build_trie_(const std::unordered_map<int, bitcode>& codes);
    static std::vector<std::pair<int, int>> read_lens_(reader& _reader, const std::vector<ull>& abc);
    static int read_code_(std::shared_ptr<archiver::node> root, reader& _reader);
    static bool unzip_body_(const std::shared_ptr<archiver::node>& root, reader& _reader, writer& _writer);
    static bool unzip_file_(reader& _reader);

public:
    static void zip(std::vector<std::string> file_paths, const std::string& archive_name);
    static void unzip(const std::string& archive);
};