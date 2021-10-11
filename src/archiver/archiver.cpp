#include "archiver.h"

void archiver::zip(std::vector<std::string> file_paths, std::string archive_name) {
    std::ofstream out(archive_name);
    for (int i = 0; i < file_paths.size(); i++) {
        int eof = i == file_paths.size() - 1 ? this->ARCHIVE_END : this->FILENAME_END;
        zip_file_(file_paths[i], out, eof);
    }
}

void archiver::zip_file_(std::string &file_name, std::ofstream &out, int eof) {
    std::ifstream in(file_name, std::ifstream::binary);
    reader _reader(in);
    writer _writer(out);

    // Подсчет количества вхождений
    std::unordered_map<int, ull> freq;
    freq[this->FILENAME_END]++;
    freq[this->ONE_MORE_FILE]++;
    freq[this->ARCHIVE_END]++;
    for (ull c; _reader.read(8, c);)
        freq[c]++;

    huffman_trie huffmanTrie(freq);
    auto order = huffmanTrie.get_order();
    _writer.write(static_cast<ull>(order.size()), 9);
    for (auto s : order)
        _writer.write(static_cast<ull>(s), 9);

    // Запись количества кодов каждой длины
    int cnt = 0;
    int sz = 1;
    for (auto s : order) {
        int cur = huffmanTrie.get(s).size();
        for (int i = sz; i < cur; i++) {
            _writer.write(static_cast<ull>(cnt), 9);
            cnt = 0;
        }
        sz = cur;
        cnt++;
    }
    if (cnt)
        _writer.write(static_cast<ull>(cnt), 9);

    // Кодируем имя файла
    for (auto chr : file_name)
        _writer.write(huffmanTrie.get(chr));
    _writer.write(huffmanTrie.get(this->FILENAME_END));

    for (ull chr; _reader.read(8, chr);)
        _writer.write(huffmanTrie.get(chr));
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

void archiver::unzip(std::string archive) {
    std::ifstream in(archive);
    reader _reader(in);
    while (true)
        unzip_file_(_reader);
}

bool archiver::unzip_file_(reader &_reader) {
    ull abc_size;
    std::vector<ull>abc;
    _reader.read(9, abc_size);
    for (int i = 0; i < abc_size; i++)
        _reader.read(9, abc[i]);
    std::vector<std::pair<int, int>> lens;
    int abc_it = 0;

    for (int len = 0; abc_it != abc.size(); len++) {
        ull cnt;
        _reader.read(9, cnt);
        for (int i = 0; i < cnt; abc_it++) {
            if (abc_it == abc.size())
                throw std::runtime_error("The file is corrupted");
            lens.emplace_back(len, abc[abc_it]);
        }
    }
    if (abc_it != abc_size)
        throw std::runtime_error("The file is corrupted");
    
    bytecode bc;
    std::unordered_map<int, bytecode> codes;
    for (auto i : lens) {
        
    }







}


