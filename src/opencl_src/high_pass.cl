
__kernel void rgbHighPass(int size, __global unsigned char* in, __global unsigned char* out) {
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

    unsigned long sumR = 0;
    unsigned long sumG = 0;
    unsigned long sumB = 0;
    for (unsigned long i = startX; i <= stopX; ++i) {
        for (unsigned long j = startY; j <= stopY; ++j) {
            unsigned long pixel = (j * width + i) * 3;
            sumR += in[pixel];
            sumG += in[pixel + 1];
            sumB += in[pixel + 2];
        }
    }
    unsigned long pixel = index * 3;

    float r = (float)sumR / (float)((stopX - startX + 1) * (stopY - startY + 1));
    if (in[pixel] < r)
        out[pixel] = 0;
    else
        out[pixel] = in[pixel] - r;

    ++pixel;

    float g = (float)sumG / (float)((stopX - startX + 1) * (stopY - startY + 1));
    if (in[pixel] < g)
        out[pixel] = 0;
    else
        out[pixel] = in[pixel] - g;

    ++pixel;

    float b = (float)sumB / (float)((stopX - startX + 1) * (stopY - startY + 1));
    if (in[pixel] < b)
        out[pixel] = 0;
    else
        out[pixel] = in[pixel] - b;
}

__kernel void bwHighPass(int size, __global unsigned char* in, __global unsigned char* out) {
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

    unsigned long sum = 0;
    for (unsigned long i = startX; i <= stopX; ++i) {
        for (unsigned long j = startY; j <= stopY; ++j) {
            sum += in[j * width + i];
        }
    }

    float value = (float)sum / (float)((stopX - startX + 1) * (stopY - startY + 1));
    if (in[index] < value)
        out[index] = 0;
    else
        out[index] = (unsigned char)(in[index] - value);
}