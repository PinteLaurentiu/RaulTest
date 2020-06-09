//
// Created by laurentiu on 31.05.2020.
//

#ifndef RAULTESTQT_FILE_DATA_HPP
#define RAULTESTQT_FILE_DATA_HPP

#include <string>

class FileData {
public:
    FileData() = default;
    FileData(const std::string& path);
    FileData(const FileData& other) = delete;
    FileData(FileData&& other) noexcept;
    FileData& operator=(const FileData& other) = delete;
    FileData& operator=(FileData&& other) noexcept;
    ~FileData() = default;
    std::string& getData();
private:
    std::string data;
};

#endif //RAULTESTQT_FILE_DATA_HPP
