__kernel void rgbToBw(__global unsigned char* in, __global unsigned char* out)
{
    unsigned long bwIndex = get_global_id(0);
    unsigned long rgbIndex = bwIndex * 3;
    out[bwIndex] = (11 * in[rgbIndex] + 16 * in[rgbIndex+1] + 5 * in[rgbIndex+2]) / 32;
}

__kernel void bwToRgb(__global unsigned char* in, __global unsigned char* out)
{
    unsigned long bwIndex = get_global_id(0);
    unsigned long rgbIndex = bwIndex * 3;
    out[rgbIndex] = in[bwIndex];
    out[rgbIndex+1] = in[bwIndex];
    out[rgbIndex+2] = in[bwIndex];
}
