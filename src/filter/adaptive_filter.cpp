#include "../../include/adaptive_filter.h"
#include "../../include/filter.h"
#include <future>
#include <algorithm>
#include <utility>
#include <iostream>

using namespace std;

// Used for debugging stores information on filtering
struct FilterProfile {
    int row;
    vector<int> scores;
    int bestFilter;
    int bestScore;
};

// fast approach to compute the filter score by summation of absolute difference
int fastFilter_score(const vector<unsigned char>& filteredScanline) {
    int score = 0;
    for (size_t i = 2; i < filteredScanline.size(); ++i) {
        score += abs(filteredScanline[i] - filteredScanline[i - 1]);
    }
    return score;
}

// helper function to apply a single filter using modular methods
vector<unsigned char> applyFilterMethod(const vector<unsigned char>& row, const vector<unsigned char>& prevRow, unsigned w, int filterMethod) {
    switch (filterMethod) {
        case 0:
            return applyNoneFilter(row);
        case 1:
            return applySubFilter(row, w);
        case 2:
            return applyUpFilter(row, prevRow);
        case 3:
            return applyAverageFilter(row, prevRow, w);
        case 4:
            return applyPaethFilter(row, prevRow, w);
        default:
            throw runtime_error("Unknown filter method in applyFilterMethod");
    }
}

// Worker function that returns filtered scanlines and profiling data
pair<vector<unsigned char>, vector<FilterProfile>> filterWorkerFuture(
    const vector<unsigned char>& image, unsigned w, unsigned h,
    size_t startRow, size_t endRow
) {
    vector<unsigned char> localFiltered;
    vector<FilterProfile> localProfiles;

    for (size_t y = startRow; y < endRow; ++y) {
        vector<unsigned char> currScanline(image.begin() + y * w * 4, image.begin() + (y + 1) * w * 4);
        vector<unsigned char> prevScanline;

        if (y > 0) {
            prevScanline = vector<unsigned char>(image.begin() + (y - 1) * w * 4, image.begin() + y * w * 4);
        }

        vector<int> scores(5);
        vector<unsigned char> bestScanline;
        int bestScore = INT_MAX;
        int bestFilter = -1;

        for (int filter = 0; filter < 5; ++filter) {
            vector<unsigned char> filtered = applyFilterMethod(currScanline, prevScanline, w, filter);
            int score = fastFilter_score(filtered);
            scores[filter] = score;

            if (score < bestScore) {
                bestScore = score;
                bestFilter = filter;
                bestScanline = filtered;
            }
        }

        localFiltered.insert(localFiltered.end(), bestScanline.begin(), bestScanline.end());
        localProfiles.push_back({static_cast<int>(y), scores, bestFilter, bestScore});
    }

    return {localFiltered, localProfiles};
}

// Applies each filter and computes its score to decide the best one
ImageData adaptiveFilter(ImageData imgdata) {
    unsigned w = imgdata.w;
    unsigned h = imgdata.h;
    const vector<unsigned char>& image = imgdata.image;

    unsigned numThreads = thread::hardware_concurrency();
    size_t rowsPerThread = h / numThreads;
    size_t extra = h % numThreads;
    size_t start = 0;

    vector<future<pair<vector<unsigned char>, vector<FilterProfile>>>> futures;

    for (unsigned t = 0; t < numThreads; ++t) {
        size_t end = start + rowsPerThread + (t < extra ? 1 : 0);

        futures.push_back(
            async(launch::async, filterWorkerFuture, cref(image), w, h, start, end)
        );
        start = end;
    }

    vector<unsigned char> filteredVec;
    vector<FilterProfile> profiles;

    filteredVec.reserve(h * (1 + w * 4));
    profiles.reserve(h);

    for (auto& fut : futures) {
        auto [threadFiltered, threadProfiles] = fut.get();
        filteredVec.insert(filteredVec.end(), threadFiltered.begin(), threadFiltered.end());
        profiles.insert(profiles.end(), threadProfiles.begin(), threadProfiles.end());
    }

    sort(profiles.begin(), profiles.end(), [](const FilterProfile& a, const FilterProfile& b) {
        return a.row < b.row;
    });

    vector<unsigned char> orderedFiltered;
    orderedFiltered.reserve(h * (1 + w * 4));

    for (const auto& p : profiles) {
        size_t rowStart = p.row * (1 + w * 4);
        orderedFiltered.insert(
            orderedFiltered.end(),
            filteredVec.begin() + rowStart,
            filteredVec.begin() + rowStart + (1 + w * 4)
        );
    }

    return {orderedFiltered, w, h, imgdata.fileName};
}
