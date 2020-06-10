//
// Created by laurentiu on 31.05.2020.
//

#include "file_data.hpp"
#include "types.hpp"
#include <fstream>
#include <algorithm>

FileData::FileData(const std::string& path) {
    std::ifstream input(path, std::ios::ate | std::ios::binary | std::ios::in);
    input.exceptions(std::ios::badbit | std::ios::failbit);
    size fileSize = input.tellg();
    data.reserve(fileSize);
    input.seekg(0, std::ios::beg);
    std::copy_n(std::istreambuf_iterator(input), fileSize, std::back_inserter(data));
    input.close();
}

FileData::FileData(FileData&& other) noexcept : data(std::move(other.data)) {}

FileData& FileData::operator=(FileData&& other) noexcept {
    data = std::move(other.data);
    return *this;
}

std::string& FileData::getData() {
    return data;
}
