__kernel void minimum(int size, __global unsigned char* in, __global unsigned char* out)
{
    int width = get_global_size(0);
    int height = get_global_size(1);
    int x = get_global_id(0);
    int y = get_global_id(1);
    unsigned long index = y * width + x;

    int preOffset = (size - 1) / 2;
    int postOffset = size / 2;

    int startX = x - preOffset;
    int stopX = x + postOffset;
    int startY = y - preOffset;
    int stopY = y + postOffset;

    if (startX < 0 || stopX >= width || startY < 0 || stopY >= height) {
        out[index] = (in[index] == 0) ? 1 : 0;
        return;
    }

    unsigned char min = 255;
    for (unsigned long i = startX; i <= stopX; ++i) {
        for (unsigned long j = startY; j <= stopY; ++j) {
            unsigned long pixel = j * width + i;
            if (in[pixel] < min) {
                min = in[pixel];
            }
        }
    }
    out[index] = (in[index] == min) ? 1 : 0;
}