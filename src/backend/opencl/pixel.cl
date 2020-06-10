__kernel void rgbToBw(__global unsigned char* in, __global unsigned char* out)
{
    unsigned long bwIndex = get_global_id(0);
	unsigned long rgbIndex = bwIndex * 3;
	out[bwIndex] = (in[rgbIndex] + in[rgbIndex+1] + in[rgbIndex+2]) / 3;
}