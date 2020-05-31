//
// Created by laurentiu on 31.05.2020.
//

#include <iostream>
#include "open_cl_context.hpp"
#include "types.hpp"

OpenCLContext& OpenCLContext::instance() {
    static OpenCLContext instance;
    return instance;
}

void OpenCLContext::deleteContext(cl_context context) {
    if (context == nullptr)
        return;
    terminateOnFailure(clReleaseContext(context), "Error while releasing context ");
}

void OpenCLContext::deleteCommandQueue(cl_command_queue queue) {
    if (queue == nullptr)
        return;
    terminateOnFailure(clReleaseCommandQueue(queue), "Error while releasing command queue ");
}

OpenCLContext::OpenCLContext() {
    loadPlatform();
    loadDevice();
    loadContext();
    loadCommandQueue();
}

void OpenCLContext::loadPlatform() {
    unsigned int platformsCount;
    terminateOnFailure(clGetPlatformIDs(0, nullptr, &platformsCount), "Error while getting the count of platforms ");
    if (platformsCount == 0) {
        std::clog << "WARNING: No platform found!" << std::endl;
        return;
    }
    if (platformsCount > expectedPlatformCount) {
        loadFirstPlatform(platformsCount);
        return;
    }
    cl_platform_id platformTemporary;
    terminateOnFailure(clGetPlatformIDs(platformsCount, &platformTemporary, nullptr),
            "Error while getting the platform ");
    platform = platformTemporary;
}

void OpenCLContext::loadFirstPlatform(unsigned int platformsCount) {
    std::vector<cl_platform_id> platforms;
    platforms.reserve(platformsCount);
    terminateOnFailure(clGetPlatformIDs(platforms.capacity(), platforms.data(), nullptr),
            "Error while getting the platforms ");

    size firstPlatformNameLength;
    terminateOnFailure(clGetPlatformInfo(platforms[0], CL_PLATFORM_NAME, 0, nullptr, &firstPlatformNameLength),
            "Error while getting the name of the first platform ");

    std::string firstPlatformName;
    firstPlatformName.reserve(firstPlatformNameLength + 1);
    terminateOnFailure(clGetPlatformInfo(platforms[0],
            CL_PLATFORM_NAME,
            firstPlatformName.capacity(),
            firstPlatformName.data(),
            nullptr), "Error while getting the name of the first platform ");
    std::clog << "WARNING: Multiple platforms found! Using " << firstPlatformName << " !" << std::endl;
    platform = platforms[0];
}

void OpenCLContext::terminateOnFailure(int errorCode, const std::string& errorMessage) {
    if (errorCode != CL_SUCCESS) {
        if (!errorMessage.empty()) {
            std::cerr << errorMessage << errorCode << std::endl;
        }
        std::terminate();
    }
}

void OpenCLContext::loadDevice() {
    loadDeviceRecursively({{CL_DEVICE_TYPE_ACCELERATOR, "Accelerator"},
                            {CL_DEVICE_TYPE_GPU, "GPU"},
                            {CL_DEVICE_TYPE_CPU, "CPU"}}, 0);
}

void OpenCLContext::loadDeviceRecursively(const std::vector<std::pair<size, std::string>>& types, size typeIndex) {
    if (types.size() <= typeIndex) {
        std::cerr << "Error while getting devices! No device supported!" << std::endl;
        std::terminate();
    }
    unsigned int devicesCount;
    auto error = clGetDeviceIDs(platform.value_or(nullptr), types[typeIndex].first, 0, nullptr, &devicesCount);
    if (error != CL_SUCCESS || devicesCount == 0) {
        loadDeviceRecursively(types, typeIndex + 1);
        return;
    }
    if (devicesCount > expectedDevicesCount) {
        loadFirstDevice(types[typeIndex], devicesCount);
        return;
    }
    cl_device_id deviceTemporary;
    terminateOnFailure(clGetDeviceIDs(platform.value_or(nullptr),
            types[typeIndex].first,
            devicesCount,
            &deviceTemporary,
            nullptr), "Error while getting the device ");
    std::clog << "Using " << types[typeIndex].second << " device" << std::endl;
}

void OpenCLContext::loadFirstDevice(const std::pair<size, std::string>& type, unsigned int count) {
    std::vector<cl_device_id> devices;
    devices.reserve(count);
    terminateOnFailure(clGetDeviceIDs(platform.value_or(nullptr),
            type.first,
            devices.capacity(),
            devices.data(),
            nullptr), "Error while getting the devices ");

    size firstDeviceNameLength;
    terminateOnFailure(clGetDeviceInfo(devices[0], CL_DEVICE_NAME, 0, nullptr, &firstDeviceNameLength),
                       "Error while getting the name of the first device ");

    std::string firstDeviceName;
    firstDeviceName.reserve(firstDeviceNameLength + 1);
    terminateOnFailure(clGetDeviceInfo(devices[0],
                                       CL_DEVICE_NAME,
                                       firstDeviceName.capacity(),
                                       firstDeviceName.data(),
                                       nullptr), "Error while getting the name of the first device ");
    std::clog << "WARNING: Multiple devices found! Using "
            << firstDeviceName
            << " ["
            << type.second
            << "] !"
            << std::endl;
    device = devices[0];
}

void OpenCLContext::loadContext() {
    cl_context_properties properties[] = {static_cast<cl_context_properties>(CL_CONTEXT_PLATFORM),
                                          reinterpret_cast<cl_context_properties>(platform.value_or(nullptr)),
                                          0};
    auto* propertiesPtr = platform.has_value() ? properties : nullptr;
    int errorCode;
    auto contextTemporary = clCreateContext(propertiesPtr,
            expectedDevicesCount,
            &device.value(),
            nullptr,
            nullptr,
            &errorCode);
    terminateOnFailure(errorCode, "Error while creating the context ");
    context = ContextPtr(contextTemporary, &deleteContext);
}

void OpenCLContext::loadCommandQueue() {
    int errorCode;
    auto commandQueueTemporary = clCreateCommandQueueWithProperties(context.get(), device.value(), nullptr, &errorCode);
    terminateOnFailure(errorCode, "Error while creating the command queue ");
    commandQueue = CommandQueuePtr(commandQueueTemporary, &deleteCommandQueue);
}
