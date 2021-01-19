#ifndef rand_h_INCLUDED
#define rand_h_INCLUDED

class GlibcRand {

public:

    GlibcRand() = default;

    GlibcRand(unsigned int seed) {
        srand(seed);
    }

    void srand(unsigned int seed) {
        int type;
        int32_t *state;
        long int i;
        int32_t word;
        int32_t *dst;
        int kc;

        type = buf_.rand_deg;
        state = buf_.state;

        if (seed == 0)
            seed = 1;
        
        state[0] = seed;

        dst = state;
        word = seed;
        kc = buf_.rand_deg;

        for (i = 1; i < kc; ++i) {
            long int hi = word / 127773;
            long int lo = word % 127773;

            word = 16807 * lo - 2836 * hi;

            if (word < 0)
                word += 2147483647;
            
            *++dst = word;
        }

        buf_.fptr = &state[buf_.rand_sep];
        buf_.rptr = &state[0];
        kc *= 10;

        while (--kc >= 0) {
            rand();
        }
    }

    int rand() {
        int32_t *state;
        int result;
        state = buf_.state;

        int32_t *fptr = buf_.fptr;
        int32_t *rptr = buf_.rptr;
        int32_t *end_ptr = buf_.end_ptr;
        uint32_t val;

        val = *fptr += (uint32_t)*rptr;

        /* Chucking least random bit.  */
        result = val >> 1;
        ++fptr;

        if (fptr >= end_ptr) {
            fptr = state;
            ++rptr;
        } else {
            ++rptr;
            if (rptr >= end_ptr)
                rptr = state;
        }

        buf_.fptr = fptr;
        buf_.rptr = rptr;

        return result;
    }

private:

    static constexpr int TYPE_3 = 3, BREAK_3 = 128, DEG_3 = 31, SEP_3 = 3;

    struct random_data {
        int32_t *fptr;    /* Front pointer.  */
        int32_t *rptr;    /* Rear pointer.  */
        int32_t *state;   /* Array of state values.  */
        int rand_type;    /* Type of random number generator.  */
        int rand_deg;     /* Degree of random number generator.  */
        int rand_sep;     /* Distance between front and rear.  */
        int32_t *end_ptr; /* Pointer behind state table.  */
    } buf_ = {
        .fptr = &rand_table_[SEP_3 + 1],
        .rptr = &rand_table_[1],
        .state = &rand_table_[1],
        .rand_type = TYPE_3,
        .rand_deg = DEG_3,
        .rand_sep = SEP_3,
        .end_ptr = &rand_table_[sizeof (rand_table_) / sizeof (rand_table_[0])]
    };

    int32_t rand_table_[DEG_3 + 1] = {
        TYPE_3,
        -1726662223, 379960547, 1735697613, 1040273694, 1313901226,
        1627687941, -179304937, -2073333483, 1780058412, -1989503057,
        -615974602, 344556628, 939512070, -1249116260, 1507946756,
        -812545463, 154635395, 1388815473, -1926676823, 525320961,
        -1009028674, 968117788, -123449607, 1284210865, 435012392,
        -2017506339, -911064859, -370259173, 1132637927, 1398500161,
        -205601318
    };
};

#endif // rand_h_INCLUDED

