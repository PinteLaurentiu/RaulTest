__kernel void pseudocoloration(__global unsigned char* in, __global unsigned char* out) {
    unsigned long g_id = get_global_id(0);
    int in_value = in[g_id] * 1276 / 255;
    int out_valueR, out_valueG, out_valueB;
    if (in_value < 256) {
        out_valueR = 255 - in_value;
        out_valueG = 0;
        out_valueB = 255;
    } else if (in_value < 511) {
        out_valueR = 0;
        out_valueG = in_value - 255;
        out_valueB = 255;
    } else if (in_value < 766) {
        out_valueR = 0;
        out_valueG = 255;
        out_valueB = 765 - in_value;
    } else if (in_value < 1021) {
        out_valueR = in_value - 765;
        out_valueG = 255;
        out_valueB = 0;
    } else if (in_value < 1276) {
        out_valueR = 255;
        out_valueG = 1275 - in_value;
        out_valueB = 0;
    }
    g_id *= 3;
    out[g_id] = out_valueR;
    out[g_id + 1] = out_valueG;
    out[g_id + 2] = out_valueB;
}