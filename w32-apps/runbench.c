#include <stdlib.h>
#include <stdio.h>
#include "w2xconv.h"

int
main(int argc, char **argv)
{
    int block_size = 512;
    int proc = 0;
    int num_proc;
    const struct W2XConvProcessor *proc_list;
    
    proc_list = w2xconv_get_processor_list(&num_proc);

    if (argc >= 2) {
        block_size = atoi(argv[1]);
    }

    if (argc >= 3) {
        proc = atoi(argv[2]);
    }

    struct W2XConv *c = w2xconv_init_with_processor(proc, 0, 1);
    puts(proc_list[proc].dev_name);


    int num_maps[7] = {
        32,
        32,
        64,
        64,
        128,
        128,
        1
    };

    int total = 0;
    for (int i=0; i<7; i++) {
        total += num_maps[i];
    }

    float *bias = calloc(total, sizeof(float));
    float *coef = calloc(total * 3 * 3, sizeof(float));
    float *dst = malloc(block_size * block_size * sizeof(float));
    float *src = malloc(block_size * block_size * sizeof(float));

    w2xconv_set_model_3x3(c,
                          W2XCONV_FILTER_SCALE2x,
                          7,
                          1,
                          num_maps,
                          coef,
                          bias);

    w2xconv_apply_filter_y(c,
                           W2XCONV_FILTER_SCALE2x,
                           (unsigned char*)dst, block_size * sizeof(float),
                           (unsigned char*)src, block_size * sizeof(float),
                           block_size, block_size,
                           block_size * 2);
    w2xconv_fini(c);

    return 0;
}
