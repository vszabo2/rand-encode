import numpy as np
from tqdm import tqdm


class GlibcRand:
    DEG_3 = 31
    SEP_3 = 3

    def srand(self, seed):
        assert len(seed.shape) == 1
        self.rand_table = np.array(
            (-1726662223, 379960547, 1735697613, 1040273694, 1313901226,
             1627687941, -179304937, -2073333483, 1780058412, -1989503057,
             -615974602, 344556628, 939512070, -1249116260, 1507946756,
             -812545463, 154635395, 1388815473, -1926676823, 525320961,
             -1009028674, 968117788, -123449607, 1284210865, 435012392,
             -2017506339, -911064859, -370259173, 1132637927, 1398500161,
             -205601318),
            dtype=np.int32)
        self.rand_table = np.tile(self.rand_table, (len(seed), 1))
        self.fptr = GlibcRand.SEP_3
        self.rptr = 0

        seed[seed == 0] = 1

        self.rand_table[:, 0] = seed
        dst = 0
        kc = GlibcRand.DEG_3

        for i in range(1, kc):
            hi = seed // 127773
            lo = seed % 127773
            seed = 16807 * lo - 2836 * hi
            seed[seed < 0] += 2147483647
            dst += 1
            self.rand_table[:, dst] = seed

        for i in range(kc * 10):
            self.rand()

    def rand(self):
        self.rand_table[:, self.fptr] += self.rand_table[:, self.rptr]
        result = (self.rand_table[:, self.fptr].view(np.uint32) >> 1).view(
            np.int32)
        self.fptr += 1

        if self.fptr >= GlibcRand.DEG_3:
            self.fptr = 0
            self.rptr += 1
        else:
            self.rptr += 1
            if self.rptr >= self.DEG_3:
                self.rptr = 0

        return result


def main():
    BATCH_SIZE = 2**20
    SEQ = "heyvic"

    target = np.array([ord(i) for i in SEQ], dtype=np.int32) - ord('a')
    target = target[:, np.newaxis]
    rand = GlibcRand()
    limits = np.iinfo(np.int32)
    for start in tqdm(range(limits.min, limits.max, BATCH_SIZE)):
        seed = np.arange(start, min(limits.max + 1, start + BATCH_SIZE))
        rand.srand(seed)
        results = np.empty((len(SEQ), len(seed)), dtype=np.int32)
        for row in results:
            row[:] = rand.rand()
        successful = seed[np.all(np.equal(results % 26, target), axis=0)]
        for i in successful:
            print("Found seed:", np.uint32(i))


if __name__ == "__main__":
    main()
