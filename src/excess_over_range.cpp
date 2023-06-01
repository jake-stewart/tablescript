#include "../include/excess_over_range.hpp"

/* This function evenly distributes excess height to cells within multiple
   ranges. Cells can be covered by one or more ranges, and cells covered by more
   ranges receive higher priority for height increase. The objective is to
   minimize the number of affected cells while successfully distributing the
   excess height across all ranges. The function returns the updated heights of
   all cells after the distribution process. */

std::vector<int> excessOverRanges(
    std::vector<ExcessRange> ranges, int numCells
) {
    std::vector<int> cells(numCells);
    while (ranges.size()) {
        int chosenCell = -1;
        int maxTimesInRange = -1;

        for (int cell = 0; cell < numCells; cell++) {
            int timesInRange = 0;
            for (ExcessRange &range : ranges) {
                if (cell >= range.start && cell < range.end) {
                    timesInRange += 1;
                }
            }
            if (timesInRange > maxTimesInRange) {
                chosenCell = cell;
                maxTimesInRange = timesInRange;
            }
            else if (timesInRange == maxTimesInRange) {
                if (cells[chosenCell] > cells[cell]) {
                    chosenCell = cell;
                }
            }
        }

        assert(chosenCell != -1);
        cells[chosenCell] += 1;
        for (int i = ranges.size() - 1; i >= 0; i--) {
            if (chosenCell >= ranges[i].start && chosenCell < ranges[i].end) {
                ranges[i].excessHeight--;
                if (ranges[i].excessHeight == 0) {
                    ranges.erase(ranges.begin() + i);
                }
            }
        }
    }
    return cells;
}
