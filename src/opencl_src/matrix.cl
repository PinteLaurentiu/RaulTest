int clamp(int value, int max) {
    if (value < 0)
        return 0;
    if (value >= max)
        return max-1;
    return value;
}

__kernel void bwMatrix(__global int* matrix, __global unsigned char* in, __global unsigned char* out)
{
    int width = get_global_size(0);
    int height = get_global_size(1);
    int x = get_global_id(0);
    int y = get_global_id(1);
    unsigned long index = y * width + x;
    int result = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            result += (int)in[clamp(y + j, height) * width + clamp(x + i, width)] * (int)matrix[(j + 1) * 3 + i + 1];
        }
    }
    out[index] = clamp(result, 256);
}

__kernel void rgbMatrix(__global int* matrix, __global unsigned char* in, __global unsigned char* out)
{
    int width = get_global_size(0);
    int height = get_global_size(1);
    int x = get_global_id(0);
    int y = get_global_id(1);
    unsigned long index = (y * width + x) * 3;
    int resultR = 0;
    int resultG = 0;
    int resultB = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int baseIndex = (clamp(y + j, height) * width + clamp(x + i, width)) * 3;
            int matrixValue = matrix[(j + 1) * 3 + i + 1];
            resultR += (int)in[baseIndex] * matrixValue;
            resultG += (int)in[baseIndex + 1] * matrixValue;
            resultB += (int)in[baseIndex + 2] * matrixValue;
        }
    }
    out[index] = clamp(resultR, 256);
    out[index + 1] = clamp(resultG, 256);
    out[index + 2] = clamp(resultB, 256);
}

__kernel void bwMatrixJoin(__global unsigned char* in1, __global unsigned char* in2, __global unsigned char* out) {
    int id = get_global_id(0);
    out[id] = (in1[id] + in2[id]) / 2;
}

__kernel void rgbMatrixJoin(__global unsigned char* in1, __global unsigned char* in2, __global unsigned char* out) {
    int id = get_global_id(0) * 3;
    out[id] = (in1[id] + in2[id]) / 2;
    ++id;
    out[id] = (in1[id] + in2[id]) / 2;
    ++id;
    out[id] = (in1[id] + in2[id]) / 2;
}