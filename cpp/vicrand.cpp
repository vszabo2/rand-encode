#include <array>
#include <iostream>
#include <string_view>
#include <thread>

#include "rand.h"

constexpr char map(unsigned rand_value) {
    return 'a' + (rand_value % 26);
}

constexpr std::string_view seq = "heyvic";
constexpr unsigned kNumThreads = 16;
constexpr unsigned chunk_size = (1 + static_cast<unsigned long long>(static_cast<unsigned>(-1))) / kNumThreads;

void job(unsigned threadID) {
    unsigned start = chunk_size * threadID;
    unsigned end = start + chunk_size;

    GlibcRand r;

    for(unsigned seed = start; seed != end; seed++) {
        r.srand(seed);

        for(const auto& v : seq) {
            if(map(r.rand()) != v) {
                goto next;
            }
        }

        std::cout << "[Thread " << threadID << "] Found seed " << seed << std::endl;

        next:;
    }
}

int main() {
    std::array<std::thread, kNumThreads> threads;

    for(unsigned threadID = 0; threadID < kNumThreads; threadID++) {
        threads[threadID] = std::thread(job, threadID);
    }

    for(auto& t : threads) {
        t.join();
    }

    return 0;
}
