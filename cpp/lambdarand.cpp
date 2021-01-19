#include <array>
#include <climits>
#include <iostream>
#include <thread>
#include <vector>

#include "rand.h"

int main() {

    std::vector<unsigned char> seq = {'h', 'e', 'y', 'v', 'i', 'c'};

    auto map = [](unsigned rand_value) {
        return 'a' + (rand_value % 26);
    };

    constexpr unsigned kNumThreads = 16;
    std::array<std::thread, kNumThreads> threads;

    for(unsigned threadID = 0; threadID < kNumThreads; threadID++) {
        threads[threadID] = std::thread([=]() {
            constexpr unsigned chunk_size = UINT_MAX / kNumThreads;
            unsigned start = chunk_size * threadID;

            GlibcRand r;

            auto is_correct = [&](unsigned seed) {
                r.srand(seed);

                for(const auto &v : seq) {
                    if(map(r.rand()) != v) {
                        return false;
                    }
                }

                return true;
            };

            for(unsigned seed = start; seed < start + chunk_size + 1; seed++) {
                if(is_correct(seed)) {
                    std::cout << "[Thread " << threadID << "] Found seed " << seed << std::endl;
                }
            }
        });
    }

    for(auto &t : threads)
        t.join();
    
    return 0;
}
