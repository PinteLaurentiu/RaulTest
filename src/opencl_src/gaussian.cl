
int clampGaussian(int value, int max)
{
    if (value < 0)
        return 0;
    if (value >= max)
        return max-1;
    return value;
}

__kernel void bwGaussian(__global int* matrix, __global unsigned char* in, __global unsigned char* out)
{
    int width = get_global_size(0);
    int height = get_global_size(1);
    int x = get_global_id(0);
    int y = get_global_id(1);
    unsigned long index = y * width + x;
    int result = 0;
    for (int i = -2; i <= 2; ++i) {
        for (int j = -2; j <= 2; ++j) {
            result += (int)in[clampGaussian(y + j, height) * width + clampGaussian(x + i, width)] * (int)matrix[(j + 2) * 5 + i + 2];
        }
    }
    result /= 273;
    out[index] = clampGaussian(result, 256);
}

__kernel void rgbGaussian(__global int* matrix, __global unsigned char* in, __global unsigned char* out)
{
    int width = get_global_size(0);
    int height = get_global_size(1);
    int x = get_global_id(0);
    int y = get_global_id(1);
    int resultR = 0;
    int resultG = 0;
    int resultB = 0;
    for (int i = -2; i <= 2; ++i) {
        for (int j = -2; j <= 2; ++j) {
            int index = (clampGaussian(y + j, height) * width + clampGaussian(x + i, width)) * 3;
            int matrixIndex = (j + 2) * 5 + i + 2;
            resultR += (int)in[index] * (int)matrix[matrixIndex];
            resultG += (int)in[index + 1] * (int)matrix[matrixIndex];
            resultB += (int)in[index + 2] * (int)matrix[matrixIndex];
        }
    }
    unsigned long index = (y * width + x) * 3;
    resultR /= 273;
    resultG /= 273;
    resultB /= 273;
    out[index] = clampGaussian(resultR, 256);
    out[index + 1] = clampGaussian(resultG, 256);
    out[index + 2] = clampGaussian(resultB, 256);
}
