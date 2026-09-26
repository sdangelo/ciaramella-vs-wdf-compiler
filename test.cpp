#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "rc_lowpass.h"
#include "wdf_compiler.h"

#define M 100000000
#define n_iter 4

int main() {
    rc_lowpass lp;

    lp.setSampleRate(48000.f);
    lp.reset();

    float *data_in = (float *)malloc(M * sizeof(float));
    float *data_out = (float *)malloc(M * sizeof(float));

    long long time_accum = 0LL;
    float save_out = 0;
    for (int iter = 0; iter < n_iter; ++iter)
    {
        for (int n = 0; n < M; ++n)
            data_in[n] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;

        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        long long start = (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;

        lp.process(data_in, data_out, M);

        clock_gettime(CLOCK_MONOTONIC, &ts);
        long long end = (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
        time_accum += end - start;
    }

    double ns_per_sample = ((double) time_accum) / n_iter / M;
    printf ("%f ns/sample\n", ns_per_sample);

    Params params;
    Impedances impedances;
    State state;
    state.C1_z = 0.f;

    calc_impedances(impedances, 48000.f, params);

    time_accum = 0LL;
    save_out = 0;
    for (int iter = 0; iter < n_iter; ++iter)
    {
        for (int n = 0; n < M; ++n)
            data_in[n] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;

        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        long long start = (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;

        for (int n = 0; n < M; ++n)
            data_out[n] = process(state, impedances, data_in[n]);

        clock_gettime(CLOCK_MONOTONIC, &ts);
        long long end = (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
        time_accum += end - start;
    }

    ns_per_sample = ((double) time_accum) / n_iter / M;
    printf ("%f ns/sample\n", ns_per_sample);

    free(data_in);
    free(data_out);

    return 0;
}
