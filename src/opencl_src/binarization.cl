__kernel void bwBinarization(unsigned char value, __global unsigned char* in, __global unsigned char* out) {
    int id = get_global_id(0);
    out[id] = in[id] < value ? 0 : 255;
}