#include "archiver.h"

#include <utility>

void archiver::zip(std::vector<std::string> file_paths, const std::string& archive_name) {
    std::ofstream out(archive_name);
    for (int i = 0; i < file_paths.size(); i++) {
        int eof = i == file_paths.size() - 1 ? ARCHIVE_END : FILENAME_END;
        zip_file_(file_paths[i], out, eof);
    }
}

void archiver::zip_file_(std::string &file_name, std::ofstream &out, int eof) {
    std::ifstream in(file_name, std::ifstream::binary);
    reader _reader(in);
    writer _writer(out);

    // Подсчет количества вхождений
    std::unordered_map<int, ull> freq;
    freq[FILENAME_END]++;
    freq[ONE_MORE_FILE]++;
    freq[ARCHIVE_END]++;
    for (unsigned char c : file_name)
        freq[c]++;
    for (ull c; _reader.read(8, c);)
        freq[c]++;

    huffman_trie huffmanTrie(freq);
    auto root = build_trie(huffmanTrie.table_);
    auto order = huffmanTrie.get_order();
    _writer.write(static_cast<ull>(order.size()), 9);
    for (auto s : order)
        _writer.write(static_cast<ull>(s.second), 9);

    // Запись количества кодов каждой длины
    int cnt = 0;
    int sz = 1;
    for (auto s : order) {
        int cur = s.first;
        for (; sz < cur; sz++) {
            _writer.write(static_cast<ull>(cnt), 9);
            cnt = 0;
        }
        cnt++;
    }
    if (cnt)
        _writer.write(static_cast<ull>(cnt), 9);

    // Кодируем имя файла
    for (unsigned char chr : file_name)
        _writer.write(huffmanTrie.get(chr));
    _writer.write(huffmanTrie.get(FILENAME_END));

    in.open(file_name);
    for (ull chr; _reader.read(8, chr);)
        _writer.write(huffmanTrie.get(chr));

    _writer.write(huffmanTrie.get(eof));
    in.close();
}

std::shared_ptr<archiver::node> archiver::build_trie(std::unordered_map<int, bytecode> codes) {
    std::shared_ptr<archiver::node> root = std::make_shared<archiver::node>(node());
    for (auto &i : codes) {
        bytecode& path = i.second;
        std::reverse(path.begin(), path.end());
        std::shared_ptr<archiver::node> cur = root;

        while (!path.empty()) {
            bool go = path.back();
            path.pop_back();
            if (!go) {
                if (!cur->l)
                    cur->l = std::make_shared<archiver::node>(node());
                cur = cur->l;
            }
            else{
                if (!cur->r)
                    cur->r = std::make_shared<archiver::node>(node());
                cur = cur->r;
            }
        }
        cur->val = i.first;
    }
    return root;
}

void archiver::unzip(const std::string& archive) {
    std::ifstream in(archive, std::ifstream::binary);
    reader _reader(in);
    while (unzip_file_(_reader));
}

bool archiver::unzip_file_(reader &_reader) {
    ull abc_size;
    _reader.read(9, abc_size);
    std::vector<ull>abc(abc_size);
    for (int i = 0; i < abc_size; i++)
        _reader.read(9, abc[i]);

    std::vector<std::pair<int, int>> lens;
    int abc_it = 0;
    for (int len = 1; abc_it != abc.size(); len++) {
        ull cnt;
        _reader.read(9, cnt);
        for (int i = 0; i < cnt; i++, abc_it++) {
            if (abc_it == abc.size())
                throw std::runtime_error("The file is corrupted");
            lens.emplace_back(len, abc[abc_it]);
        }
    }
    if (abc_it != abc_size)
        throw std::runtime_error("The file is corrupted");

    std::unordered_map<int, bytecode> codes = huffman_trie::make_canonical(lens);
    std::shared_ptr<archiver::node> root = archiver::build_trie(codes);

    std::ofstream out(archiver::read_filename(root, _reader));
    writer _writer(out);

    return archiver::unzip_body(root, _reader, _writer);
}

int archiver::read_code(std::shared_ptr<archiver::node> cur, reader& _reader) {
    while (cur->l || cur->r) {
        bool go;
        _reader.read(go);
        if (!go) cur = cur->l;
        else cur = cur->r;
        if (!cur)
            throw std::runtime_error("The file is corrupted");
    }
    return cur->val;
}

std::string archiver::read_filename(std::shared_ptr<archiver::node> root, reader &_reader) {
    std::string file_name;
    int x = read_code(std::move(root), _reader);
    if (x == FILENAME_END)
        return file_name;
    else
        file_name.push_back(static_cast<char>(x));
    throw std::runtime_error("The file is corrupted");
}

bool archiver::unzip_body(const std::shared_ptr <node>& root, reader &_reader, writer &_writer) {
    while (true) {
        int x = archiver::read_code(root, _reader);
        if (x == ARCHIVE_END)
            return false;
        if (x == ONE_MORE_FILE)
            return true;
        _writer.write(x,8);
    }
}


