/*
   Simplified C++-style version of the trinomial RNG from glibc
   Copyright (C) 2021 vszabo2

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
   USA
 */
/*
   Copyright (C) 1995-2019 Free Software Foundation, Inc.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/*
   Copyright (C) 1983 Regents of the University of California.
   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   4. Neither the name of the University nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.
   THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
   SUCH DAMAGE.*/
/*
 * This is derived from the Berkeley source:
 *        @(#)random.c        5.5 (Berkeley) 7/6/88
 * It was reworked for the GNU C Library by Roland McGrath.
 * Rewritten to be reentrant by Ulrich Drepper, 1995
 */

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
