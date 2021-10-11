#include <fstream>

class archiver {
private:
    const int FILENAME_END=256;
    const int ONE_MORE_FILE=257;
    const int ARCHIVE_END=258;

    void build_trie();

    void zip_file_(std::string& file_name, std::ofstream& out);
    bool unzip_file_(std::ifstream& in);

public:
    void zip(std::vector<std::string>file_paths, std::string archive_name);
    void unzip(std::string archive);

};