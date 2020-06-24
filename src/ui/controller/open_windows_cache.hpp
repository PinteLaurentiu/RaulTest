//
// Created by laurentiu on 25.06.2020.
//

#ifndef RAULTESTQT_OPEN_WINDOWS_CACHE_HPP
#define RAULTESTQT_OPEN_WINDOWS_CACHE_HPP


#include <vector>
#include <bits/unique_ptr.h>
#include <QtWidgets/QMainWindow>

class OpenWindowsCache {
public:
    OpenWindowsCache(const OpenWindowsCache&) = delete;
    OpenWindowsCache(OpenWindowsCache&&) noexcept = delete;
    OpenWindowsCache& operator=(const OpenWindowsCache&) = delete;
    OpenWindowsCache& operator=(OpenWindowsCache&&) noexcept = delete;
    ~OpenWindowsCache() = default;
    static OpenWindowsCache& instance();
    void save(std::unique_ptr<QMainWindow> window);
    void remove(QMainWindow* instance);
private:
    OpenWindowsCache() = default;
    std::vector<std::unique_ptr<QMainWindow>> windows;
};


#endif //RAULTESTQT_OPEN_WINDOWS_CACHE_HPP
