#include "archiver.h"

/*
 * - Разбить функции
 * - Добавить вывод времени
 * - Добавить работу с консольными параметрами
 * - Продолжить рефакторинг
 */

void archiver::zip(std::vector<std::string> file_paths, const std::string& archive_name) {
    writer _writer(archive_name);
    for (int i = 0; i < file_paths.size(); i++) {
        reader _reader(file_paths[i]);
        int eof = i == file_paths.size() - 1 ? ARCHIVE_END : ONE_MORE_FILE;
        zip_file_(_reader, _writer, eof);
    }
}

void archiver::zip_file_(reader& _reader, writer& _writer, int eof) {
    // Подсчет количества вхождений
    std::unordered_map<int, ull> freq;
    freq[FILENAME_END]++;
    freq[ONE_MORE_FILE]++;
    freq[ARCHIVE_END]++;
    for (unsigned char c : _reader.get_name())
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
    for (unsigned char chr : _reader.get_name())
        _writer.write(huffmanTrie.get(chr));
    _writer.write(huffmanTrie.get(FILENAME_END));

    _reader.reopen();
    for (ull chr; _reader.read(8, chr);)
        _writer.write(huffmanTrie.get(chr));

    _writer.write(huffmanTrie.get(eof));
}

std::shared_ptr<archiver::node> archiver::build_trie(std::unordered_map<int, bitcode> codes) {
    std::shared_ptr<archiver::node> root = std::make_shared<archiver::node>(node());
    for (auto &i : codes) {
        bitcode& path = i.second;
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
    reader _reader(archive);
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

    std::unordered_map<int, bitcode> codes = huffman_trie::make_canonical(lens);
    std::shared_ptr<archiver::node> root = archiver::build_trie(codes);

    writer _writer(archiver::read_filename(root, _reader));

    return archiver::unzip_body(root, _reader, _writer);
}

int archiver::read_code(std::shared_ptr<archiver::node> cur, reader& _reader) {
    while (cur->l || cur->r) {
        bool go;
        _reader.read(go);
        if (!go)
            cur = cur->l;
        else
            cur = cur->r;
        if (!cur)
            throw std::runtime_error("The file is corrupted");
    }
    return cur->val;
}

std::string archiver::read_filename(const std::shared_ptr<archiver::node>& root, reader &_reader) {
    std::string file_name;
    while (true) {
        int x = read_code(root, _reader);
        if (x == FILENAME_END)
            return file_name;
        else
            file_name.push_back(static_cast<char>(x));
    }
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


