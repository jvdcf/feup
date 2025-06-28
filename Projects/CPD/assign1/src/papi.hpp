#ifndef CPD_PAPI_HPP
#define CPD_PAPI_HPP

#include <cstddef>
#include <papi.h>

// Comment this #define to disable PAPI module
// #define PAPI_ENABLE

class PAPI {
    private:
        int eventSet = PAPI_NULL;
        long long values[4] = {0, 0, 0, 0};

    public:
        PAPI();
        void start();
        void stop();
        void print(double duration);
        void reset();
        void to_csv(double duration, std::size_t dimension, size_t block_size);
        ~PAPI();
};

#endif // CPD_PAPI_HPP
