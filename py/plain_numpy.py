import numpy as np
from tqdm import tqdm

from rand_numpy import GlibcRand


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
