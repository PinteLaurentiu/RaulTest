//
// Created by laurentiu on 31.05.2020.
//

#ifndef RAULTESTQT_OPEN_CL_CONTEXT_HPP
#define RAULTESTQT_OPEN_CL_CONTEXT_HPP


#include <optional>
#include <CL/cl.h>
#include <memory>
#include <vector>
#include "types.hpp"

class OpenCLContext {
public:
    OpenCLContext(const OpenCLContext&) = delete;
    OpenCLContext(OpenCLContext&&) noexcept = delete;
    OpenCLContext& operator=(const OpenCLContext&) = delete;
    OpenCLContext& operator=(OpenCLContext&&) noexcept = delete;
    static OpenCLContext& instance();
    ~OpenCLContext() = default;
private:
    static void deleteContext(cl_context context);
    static void deleteCommandQueue(cl_command_queue queue);
    static void terminateOnFailure(int errorCode, const std::string& errorMessage = "");

private:
    using ContextPtr = std::unique_ptr<_cl_context, decltype(&deleteContext)>;
    using CommandQueuePtr = std::unique_ptr<_cl_command_queue, decltype(&deleteCommandQueue)>;

private:
    OpenCLContext();
    void loadPlatform();
    void loadFirstPlatform(unsigned int platformsCount);
    void loadDevice();
    void loadDeviceRecursively(const std::vector<std::pair<size, std::string>>& types, size typeIndex);
    void loadFirstDevice(const std::pair<size, std::string> &pair, unsigned int count);
    void loadContext();
    void loadCommandQueue();

private:
    std::optional<cl_platform_id> platform;
    std::optional<cl_device_id> device;
    ContextPtr context = ContextPtr(nullptr, &deleteContext);
    CommandQueuePtr commandQueue = CommandQueuePtr(nullptr, &deleteCommandQueue);
    static constexpr auto expectedDevicesCount = 1;
    static constexpr auto expectedPlatformCount = 1;

};


#endif //RAULTESTQT_OPEN_CL_CONTEXT_HPP
