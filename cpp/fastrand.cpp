#include <iostream>
#include <climits>

#include "rand.h"

int main() {

    constexpr std::string_view seq = "heyvic";

    GlibcRand r;

    #pragma omp parallel for private(r) firstprivate(seq)
    for(unsigned long long seed = 0; seed <= UINT_MAX; seed++) {
        
        r.srand(seed);

        for(char c : seq) {
            if('a' + r.rand() % 26 != c) {
                goto next;
            }
        }
        
        std::cout << "Found seed: " << seed << std::endl;

        next:;
    }

    return 0;
}
