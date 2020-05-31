#include "controller/controller.hpp"
#include <QApplication>
#include <iostream>
#include <CL/cl.h>

int main(int argc, char** argv)
{
//    QApplication app(argc, argv);
//    Controller controller;
//    controller.show();
//
//    return QApplication::exec();
    cl_uint numPlatforms; //the NO. of platforms
    cl_platform_id platform = nullptr; //the chosen platform
    cl_int status = clGetPlatformIDs(0, nullptr, &numPlatforms);
    if (status != CL_SUCCESS)
    {
        std::cout << "Error: Getting platforms!" << std::endl;
        return EXIT_FAILURE;
    }
    cl_platform_id* platforms = (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));
    status = clGetPlatformIDs(numPlatforms, platforms, NULL);

    if (numPlatforms > 0)
    {
        cl_platform_id* platforms = (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        platform = platforms[0];
        free(platforms);
    }

    /*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
    cl_uint numDevices = 0;
    cl_device_id        *devices;
    status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
    if (numDevices == 0) //no GPU available.
    {
        std::cout << "No GPU device available." << std::endl;
        std::cout << "Choose CPU as default device." << std::endl;
        status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);
        devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
        status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, numDevices, devices, NULL);
    }
    else
    {
        devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
        status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
    }


    /*Step 3: Create context.*/
    cl_context context = clCreateContext(NULL, 1, devices, NULL, NULL, NULL);

    /*Step 4: Creating command queue associate with the context.*/
    cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);

    /*Step 5: Create program object */
    const char *filename = "HelloWorld_Kernel.cl";
    std::string sourceStr;
//    status = convertToString(filename, sourceStr);
//    const char *source = sourceStr.c_str();
//    size_t sourceSize[] = { strlen(source) };
//    cl_program program = clCreateProgramWithSource(context, 1, &source, sourceSize, NULL);

    /*Step 6: Build program. */
//    status = clBuildProgram(program, 1, devices, NULL, NULL, NULL);

    /*Step 7: Initial input,output for the host and create memory objects for the kernel*/
//    const char* input = "GdkknVnqkc";
//    size_t strlength = strlen(input);
//    cout << "input string:" << endl;
//    cout << input << endl;
//    char *output = (char*)malloc(strlength + 1);

//    cl_mem inputBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
//                                        (strlength + 1) * sizeof(char), (void *)input, NULL);
//    cl_mem outputBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
//                                         (strlength + 1) * sizeof(char), NULL, NULL);

    /*Step 8: Create kernel object */
//    cl_kernel kernel = clCreateKernel(program, "helloworld", NULL);

    /*Step 9: Sets Kernel arguments.*/
//    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer);
//    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&outputBuffer);

    /*Step 10: Running the kernel.*/
//    size_t global_work_size[1] = { strlength };
//    status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL,
//                                    global_work_size, NULL, 0, NULL, NULL);

    /*Step 11: Read the cout put back to host memory.*/
//    status = clEnqueueReadBuffer(commandQueue, outputBuffer, CL_TRUE, 0,
//                                 strlength * sizeof(char), output, 0, NULL, NULL);
//
//    output[strlength] = '\0'; //Add the terminal character to the end of output.
//    cout << "\noutput string:" << endl;
//    cout << output << endl;

    /*Step 12: Clean the resources.*/
//    status = clReleaseKernel(kernel); //Release kernel.
//    status = clReleaseProgram(program); //Release the program object.
//    status = clReleaseMemObject(inputBuffer); //Release mem object.
//    status = clReleaseMemObject(outputBuffer);
    status = clReleaseCommandQueue(commandQueue); //Release  Command queue.
    status = clReleaseContext(context); //Release context.

//    if (output != NULL)
//    {
//        free(output);
//        output = NULL;
//    }

    if (devices != NULL)
    {
        free(devices);
        devices = NULL;
    }

    std::cout << "Passed!\n";
    return EXIT_SUCCESS;
}
