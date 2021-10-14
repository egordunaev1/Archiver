#include "archiver.h"

// ============
//  Архивация
// ============
// Сжатие файлов в архив
void archiver::zip(std::vector<std::string> file_paths, const std::string& archive_name) {
    writer _writer(archive_name);
    for (int i = 0; i < file_paths.size(); i++) {
        reader _reader(file_paths[i]);
        int eof = i == file_paths.size() - 1 ? ARCHIVE_END : ONE_MORE_FILE;
        zip_file_(_reader, _writer, eof);
    }
}

// Сжатие одного файла
void archiver::zip_file_(reader& _reader, writer& _writer, int eof) {
    huffman_trie huffmanTrie(archiver::count_frequency_(_reader));
    archiver::write_meta_(_writer, _reader, huffmanTrie);
    archiver::write_body_(_writer, _reader, huffmanTrie);
    _writer.write(huffmanTrie.get(eof));
}

// Запись содержимого файла
void archiver::write_body_(writer &_writer, reader &_reader, huffman_trie &huffmanTrie) {
    for (ull chr; _reader.read(8, chr);)
        _writer.write(huffmanTrie.get(static_cast<int>(chr)));
}

// Запись метаданных
void archiver::write_meta_(writer& _writer, reader& _reader, huffman_trie& huffmanTrie) {
    std::vector<std::pair<int, int>> order = huffmanTrie.get_order();

    // Запись алфавита
    _writer.write(static_cast<ull>(order.size()), 9);
    for (std::pair<int, int> s : order)
        _writer.write(static_cast<ull>(s.second), 9);

    // Запись количества кодов каждой длины
    for (size_t i = 0, cnt = 0, sz = 1; i < order.size(); i++) {
        int cur = order[i].first;
        for (; sz < cur; sz++) {
            _writer.write(static_cast<ull>(cnt), 9);
            cnt = 0;
        }
        cnt++;
        if (i == order.size() - 1)
            _writer.write(static_cast<ull>(cnt), 9);
    }

    // Кодируем имя файла
    for (unsigned char chr : _reader.get_name())
        _writer.write(huffmanTrie.get(chr));
    _writer.write(huffmanTrie.get(FILENAME_END));
}

// Подсчет вхождений
std::unordered_map<int, ull> archiver::count_frequency_(reader &_reader) {
    std::unordered_map<int, ull> freq;
    freq[FILENAME_END]++;
    freq[ONE_MORE_FILE]++;
    freq[ARCHIVE_END]++;
    for (unsigned char c : _reader.get_name())
        freq[c]++;
    for (ull c; _reader.read(8, c);)
        freq[static_cast<int>(c)]++;
    _reader.reopen();
    return freq;
}

// =============
//  Распаковка
// =============
// Распаковка архива
void archiver::unzip(const std::string& archive) {
    reader _reader(archive);
    while (unzip_file_(_reader));
}

// Распаковка файла
bool archiver::unzip_file_(reader &_reader) {
    std::vector<ull>abc = archiver::read_abc_(_reader);
    std::vector<std::pair<int, int>> lens = archiver::read_lens_(_reader, abc);
    std::unordered_map<int, bitcode> codes = huffman_trie::make_canonical(lens);
    archiver::nodeptr trie = archiver::build_trie_(codes);
    writer _writer(archiver::read_filename_(trie, _reader));
    return archiver::unzip_body_(trie, _reader, _writer);
}

// Чтение зашифрованного байта
int archiver::read_code_(archiver::nodeptr root, reader& _reader) {
    while (root->l || root->r) {
        bool go;
        if (!_reader.read(go))
            throw std::runtime_error("The file is corrupted");
        root = go ? root->r : root->l;
        if (!root)
            throw std::runtime_error("The file is corrupted");
    }
    return root->val;
}

// Чтение длин кодов
std::vector<std::pair<int, int>> archiver::read_lens_(reader &_reader, const std::vector<ull>& abc) {
    std::vector<std::pair<int, int>> lens;
    for (ull len = 1, abc_it = 0, cnt; abc_it != abc.size(); len++) {
        _reader.read(9, cnt);
        for (int i = 0; i < cnt; i++, abc_it++) {
            if (abc_it == abc.size())
                throw std::runtime_error("The file is corrupted");
            lens.emplace_back(len, abc[abc_it]);
        }
    }
    return lens;
}

// Построение бора для расшифровки
archiver::nodeptr archiver::build_trie_(std::unordered_map<int, bitcode> codes) {
    archiver::nodeptr root = std::make_shared<archiver::node>(node());
    for (auto &i : codes) {
        bitcode& path = i.second;
        std::reverse(path.begin(), path.end());
        archiver::nodeptr cur = root;

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

// Чтение алфавита
std::vector<ull> archiver::read_abc_(reader &_reader) {
    ull abc_size;
    _reader.read(9, abc_size);
    std::vector<ull>abc(abc_size);
    for (int i = 0; i < abc_size; i++)
        if (!_reader.read(9, abc[i]))
            throw std::runtime_error("The file is corrupted");
        return abc;
}

// Чтение имени файла
std::string archiver::read_filename_(const std::shared_ptr<archiver::node>& root, reader &_reader) {
    std::string file_name;
    while (true) {
        int x = read_code_(root, _reader);
        if (x == FILENAME_END)
            return file_name;
        else
            file_name.push_back(static_cast<char>(x));
    }
}

// Распаковка содержимого файла
bool archiver::unzip_body_(const std::shared_ptr <node>& root, reader &_reader, writer &_writer) {
    while (true) {
        int x = archiver::read_code_(root, _reader);
        if (x == ARCHIVE_END)
            return false;
        if (x == ONE_MORE_FILE)
            return true;
        _writer.write(x,8);
    }
}