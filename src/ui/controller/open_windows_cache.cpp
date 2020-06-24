//
// Created by laurentiu on 25.06.2020.
//

#include "open_windows_cache.hpp"

OpenWindowsCache& OpenWindowsCache::instance() {
    static OpenWindowsCache instance;
    return instance;
}

void OpenWindowsCache::save(std::unique_ptr<QMainWindow> window) {
    windows.emplace_back(std::move(window));
}

void OpenWindowsCache::remove(QMainWindow *instance) {
    for (auto iterator = windows.begin(); iterator != windows.end(); ++iterator) {
        if (iterator->get() == instance) {
            windows.erase(iterator);
            return;
        }
    }
}
