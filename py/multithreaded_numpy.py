from concurrent.futures import ThreadPoolExecutor
import os

import numpy as np

from rand_numpy import GlibcRand


class Encoder:

    def __init__(self, s, batch_size):
        self.batch_size = batch_size
        self.target = np.array([ord(i) for i in s], dtype=np.int32) - ord('a')
        self.target = self.target[:, np.newaxis]
        self.limits = np.iinfo(np.int32)

    def run_job(self, start):
        rand = GlibcRand()
        seed = np.arange(start,
                         min(self.limits.max + 1, start + self.batch_size))
        rand.srand(seed)
        results = np.empty((len(self.target), len(seed)), dtype=np.int32)
        for row in results:
            row[:] = rand.rand()
        successful = seed[np.all(np.equal(results % 26, self.target), axis=0)]
        for i in successful:
            print("Found seed:", np.uint32(i))

    def run_all(self):
        cpu_count = len(os.sched_getaffinity(0))
        print(f"Using {cpu_count} worker threads")
        with ThreadPoolExecutor() as e:
            e.map(self.run_job,
                  range(self.limits.min, self.limits.max, self.batch_size))


if __name__ == "__main__":
    Encoder("heyvic", 2**20).run_all()
