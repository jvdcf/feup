#include "matrix.hpp"
#include "algorithms.hpp"
#include "papi.hpp"
#include "tui.hpp"
#include <iostream>
#include <time.h>
#include <assert.h>
#include <omp.h>

void execute(Choice choice, Matrix& a, Matrix& b, Matrix& res, size_t block_size) {
    switch (choice) {
        case Choice::SEQ_SIMPLE:
            algorithms::seq_simple_mult(a, b, res);
            break;
        
        case Choice::SEQ_LINE:
            algorithms::seq_line_mult(a, b, res);
            break;
        
        case Choice::SEQ_BLOCK:
            algorithms::seq_block_mult(a, b, res, block_size);
            break;
        
        case Choice::PAR_SIMPLE:
            algorithms::par_simple_mult(a, b, res);
            break;
        
        case Choice::PAR_LINE:
            algorithms::par_line_mult(a, b, res);
            break;
        
        default:
            std::cerr << "ERROR: User Choice not handled!" << std::endl;
            exit(-1);
    }
}

void benchmark() {
    Choice choices[] = {Choice::SEQ_SIMPLE, Choice::SEQ_LINE, Choice::SEQ_BLOCK, Choice::PAR_SIMPLE, Choice::PAR_LINE};
    size_t dimensions[] = {600, 1000, 1400, 1800, 2200, 2600, 3000, 4096, 6144, 8192, 10240};
    size_t block_sizes[] = {128, 256, 512};

    auto calculation = [=](Choice choice, size_t dimension, size_t block_size = 0) {
        Matrix a(dimension, dimension, FillMethod::ONE);
        Matrix b(dimension, dimension, FillMethod::INDEX);
        Matrix res(dimension, dimension, FillMethod::ZERO);

        assert(Matrix::multiplication_check(a, b));
        PAPI papi;
        std::cout << "Choice: " << static_cast<int>(choice) << "; Dimension: " << dimension;
        if (block_size != 0) std::cout << "; Block Size: " << block_size;
        std::cout << std::endl << "=========================================" << std::endl;
        papi.start();
        double time1 = omp_get_wtime();

        execute(choice, a, b, res, block_size);

        double time2 = omp_get_wtime();
        papi.stop();
        double duration = (time2 - time1);
        printf("Time: %3.3f seconds\n", duration);
        papi.print(duration);
        papi.reset();
    };

    for (Choice choice : choices) {
        for (size_t dimension : dimensions) {
            if (choice == Choice::SEQ_BLOCK) {
                for (size_t block_size : block_sizes) {
                    calculation(choice, dimension, block_size);
                }
            } else {
                calculation(choice, dimension);
            }
        }
    }
}

void benchmark_csv() {
    Choice choices[] = {Choice::SEQ_SIMPLE, Choice::SEQ_LINE, Choice::SEQ_BLOCK, Choice::PAR_SIMPLE, Choice::PAR_LINE};
    size_t dimensions[] = {600, 1000, 1400, 1800, 2200, 2600, 3000, 4096, 6144, 8192, 10240};
    size_t block_sizes[] = {128, 256, 512};

    auto calculation = [=](Choice choice, size_t dimension, size_t block_size = 0) {
            Matrix a(dimension, dimension, FillMethod::ONE);
            Matrix b(dimension, dimension, FillMethod::INDEX);
            Matrix res(dimension, dimension, FillMethod::ZERO);

            assert(Matrix::multiplication_check(a, b));
            PAPI papi;
            
            papi.start();
            double time1 = omp_get_wtime();

            execute(choice, a, b, res, block_size);

            double time2 = omp_get_wtime();
            papi.stop();
            double duration = (time2 - time1);
            papi.to_csv(duration, dimension, block_size);
            papi.reset();
        };

    for (Choice choice : choices) {
        if (choice == Choice::SEQ_BLOCK) {
            std::cout << "size, time, blocksize, l1DCM, l2DCM, l2DCA, l2hitpercent, dpops, mflops" << std::endl;
        } else {
            std::cout << "time, l1DCM, l2DCM, l2DCA, l2hitpercent, dpops, mflops" << std::endl;
        }
        for (size_t dimension : dimensions) {
            if (choice == Choice::SEQ_SIMPLE && dimension == 4096) break;
            if (choice == Choice::SEQ_BLOCK) {    
                // size, time, blocksize, l1DCM, l2DCM, l2DCA, l2hitpercent, dpops, mflops
                for (size_t block_size : block_sizes) {
                    calculation(choice, dimension, block_size);
                }
            } else {
                // size, time, l1DCM, l2DCM, l2DCA, l2hitpercent, dpops, mflops
                calculation(choice, dimension);
            }
        }
        std::cout << "\n\n" << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc == 2) {
        if (std::string(argv[1]) == "--all") {
            benchmark_csv();
            return 0;
        }
    }
    Choice choice = TUI::ask_option();
    if (choice == Choice::ALL) {
        benchmark();
        return 0;
    }
    PAPI papi;
    size_t dimension = TUI::ask_dimensions();
    size_t block_size = 0;
    if (choice == Choice::SEQ_BLOCK) {
        block_size = TUI::ask_block_size();
    }

    Matrix a(dimension, dimension, FillMethod::ONE);
    Matrix b(dimension, dimension, FillMethod::INDEX);
    Matrix res(dimension, dimension, FillMethod::ZERO);

    assert(Matrix::multiplication_check(a, b));

    papi.start();
    double time1 = omp_get_wtime();

    execute(choice, a, b, res, block_size);

    double time2 = omp_get_wtime();
    papi.stop();
    double duration = (time2 - time1);
    printf("Time: %3.3f seconds\n", duration);

    std::cout << res;

    papi.print(duration);
    papi.reset();

    return 0;
}
