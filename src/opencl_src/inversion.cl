
__kernel void inversion(__global unsigned char* in, __global unsigned char* out) {
    int id = get_global_id(0);
    out[id] = 255 - in[id];
}