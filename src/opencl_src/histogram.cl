__kernel void histogram(unsigned long size ,__global unsigned char* in, __global unsigned long* out) {
    unsigned long g_size = get_global_size(0);
    unsigned long g_id = get_global_id(0);
    for (unsigned long index = g_id; index < size; index += g_size) {
        unsigned long value = in[index] + g_id * 256;
        out[value] = out[value] + 1;
    }
}

__kernel void histogram_join(unsigned long size, __global unsigned long* in, __global unsigned long* out) {
    unsigned long sum = 0;
    unsigned long g_id = get_global_id(0);
    for (unsigned long index = 0; index < size; ++index) {
        sum += in[g_id + 256 * index];
    }
    out[g_id] = sum;
}

__kernel void histogram_equalization(float multiply,
        float offset,
        __global unsigned char* in,
        __global unsigned char* out) {
    unsigned long g_id = get_global_id(0);
    out[g_id] = (float)in[g_id] * multiply - offset;
}

__kernel void histogram_equalization_2(int size, __global unsigned char* in, __global unsigned char* out)
{
    int width = get_global_size(0);
    int height = get_global_size(1);
    int x = get_global_id(0);
    int y = get_global_id(1);
    unsigned long index = y * width + x;

    int preOffset = (size - 1) / 2;
    int postOffset = size / 2;

    int startX = x - preOffset;
    startX = (startX < 0) ? 0 : startX;
    int stopX = x + postOffset;
    stopX = (stopX >= width) ? width - 1 : stopX;

    int startY = y - preOffset;
    startY = (startY < 0) ? 0 : startY;
    int stopY = y + postOffset;
    stopY = (stopY >= height) ? height - 1 : stopY;


    int max = 0;
    int min = 255;
    for (unsigned long i = startX; i <= stopX; ++i) {
        for (unsigned long j = startY; j <= stopY; ++j) {
            unsigned char value = in[j * width + i];
            if (max < value) {
                max = value;
            }
            if (min > value) {
                min = value;
            }
        }
    }
    if (max - min < 30) {
        out[index] = 7;
        return;
    }
    float multiply = 255.0f / (float)(max - min);
    float offset = (float)min * multiply;
    out[index] = (float)in[index] * multiply - offset;
}