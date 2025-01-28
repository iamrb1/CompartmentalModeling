#pragma once
#include <vector>

namespace cse {

    class BitVector {
    public:
        std::vector<char> underlying = {};

        void grow();
    };

};

