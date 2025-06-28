#include "tui.hpp"
#include <cstdint>
#include <iostream>

Choice TUI::ask_option() {
    std::cout << "0. Benchmark All" << std::endl;
    std::cout << "1. Multiplication" << std::endl;
    std::cout << "2. Line Multiplication" << std::endl;
    std::cout << "3. Block Multiplication" << std::endl;
    std::cout << "4. Parallel Multiplication" << std::endl;
    std::cout << "5. Parallel Line Multiplication" << std::endl;
    std::cout << "Selection?: ";

    int op;
    std::cin >> op;

    switch (op) {
        case 0:
            return Choice::ALL;

        case 1:
            return Choice::SEQ_SIMPLE;
        
        case 2:
            return Choice::SEQ_LINE;
        
        case 3:
            return Choice::SEQ_BLOCK;
        
        case 4:
            return Choice::PAR_SIMPLE;
        
        case 5:
            return Choice::PAR_LINE;
    }

    std::cout << "ERROR: Invalid option" << std::endl;
    exit(1);
}

size_t TUI::ask_dimensions()
{
    size_t dimensions;
    std::cout << "Dimensions: lins=cols ? ";
    std::cin >> dimensions;
    return dimensions;
}

size_t TUI::ask_block_size()
{
    size_t block_size;
    std::cout << "Block Size? ";
    std::cin >> block_size;
    return block_size;
}
