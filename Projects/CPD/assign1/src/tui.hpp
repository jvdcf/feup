#ifndef CPD_TUI_HPP
#define CPD_TUI_HPP

#include <cstddef>

enum class Choice {
    ALL,
    SEQ_SIMPLE,
    SEQ_LINE,
    SEQ_BLOCK,
    PAR_SIMPLE,
    PAR_LINE
};

class TUI {
    public:
        static Choice ask_option();
        static size_t ask_dimensions();
        static size_t ask_block_size();
};

#endif // CPD_TUI_HPP
