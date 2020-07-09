
int clampCanny(int value, int max)
{
    if (value < 0)
        return 0;
    if (value >= max)
        return max-1;
    return value;
}

__kernel void canny(__global int* vmatrix, __global int* hmatrix, __global unsigned char* in, __global float* out)
{
    int width = get_global_size(0);
    int height = get_global_size(1);
    int x = get_global_id(0);
    int y = get_global_id(1);
    float resultv = 0;
    float resulth = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int index = clampCanny(y + j, height) * width + clampCanny(x + i, width);
            int mindex = (j + 1) * 3 + i + 1;
            resultv += (int)in[index] * (int)vmatrix[mindex];
            resulth += (int)in[index] * (int)hmatrix[mindex];
        }
    }
    unsigned long index = (y * width + x) * 2;
    out[index] = sqrt(resultv * resultv + resulth * resulth);
    out[index + 1] = atan2(resultv, resulth);
}

__kernel void canny2(__global float* in, __global unsigned char* out)
{
    int width = get_global_size(0);
    int height = get_global_size(1);
    int x = get_global_id(0);
    int y = get_global_id(1);
    unsigned long index = (y * width + x);
    float angle = in[index * 2 + 1];
    float value = in[index * 2];
    float left;
    float right;
    if (angle < 0.3926991f || angle > 5.8904862f || (angle > 2.7488936f && angle < 3.5342917f)) {
        left = in[(clampCanny(x + 1, width) * width + y) * 2];
        right = in[(clampCanny(x - 1, width) * width + y) * 2];
    } else if ((angle >= 0.3926991f && angle < 1.178097f) || (angle >= 3.5342917f && angle < 4.3196899f)) {
        left = in[(clampCanny(x + 1, width) * width + y - 1) * 2];
        right = in[(clampCanny(x - 1, width) * width + y + 1) * 2];
    } else if ((angle >= 1.178097f && angle < 1.9634954f) || (angle >= 4.3196899f && angle < 5.1050881f)) {
        left = in[(clampCanny(x, width) * width + y - 1) * 2];
        right = in[(clampCanny(x, width) * width + y + 1) * 2];
    } else {
        left = in[(clampCanny(x - 1, width) * width + y - 1) * 2];
        right = in[(clampCanny(x + 1, width) * width + y + 1) * 2];
    }
    if (value <= left || value <= right)
        out[index] = 0;
    else
        out[index] = value;
}

__kernel void canny3(unsigned char low, unsigned char high, __global unsigned char* in, __global unsigned char* out) {
    int id = get_global_id(0);
    if (in[id] < low)
        out[id] = 0;
    else if (in[id] < high)
        out[id] = 127;
    else
        out[id] = 255;
}