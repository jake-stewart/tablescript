#ifndef EXCESS_OVER_RANGE_HPP
#define EXCESS_OVER_RANGE_HPP

#include <vector>

struct ExcessRange
{
    int start;
    int end;
    int excessHeight;
};

std::vector<int> excessOverRanges(
    std::vector<ExcessRange> ranges, int numCells
);

#endif
