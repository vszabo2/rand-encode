import torch

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")


def int32toUint32(x):
    if x < 0:
        x += 2**32
    return x


class GlibcRand:
    DEG_3 = 31
    SEP_3 = 3

    def srand(self, seed):
        assert len(seed.shape) == 1
        self.rand_table = torch.tensor(
            (-1726662223, 379960547, 1735697613, 1040273694, 1313901226,
             1627687941, -179304937, -2073333483, 1780058412, -1989503057,
             -615974602, 344556628, 939512070, -1249116260, 1507946756,
             -812545463, 154635395, 1388815473, -1926676823, 525320961,
             -1009028674, 968117788, -123449607, 1284210865, 435012392,
             -2017506339, -911064859, -370259173, 1132637927, 1398500161,
             -205601318),
            dtype=torch.int32,
            device=device).repeat((len(seed), 1))
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
        result = (
            self.rand_table[:, self.fptr] >> 1) & 0x7fffffff  # logical shift
        self.fptr += 1

        if self.fptr >= GlibcRand.DEG_3:
            self.fptr = 0
            self.rptr += 1
        else:
            self.rptr += 1
            if self.rptr >= self.DEG_3:
                self.rptr = 0

        return result


class Encoder:

    def __init__(self, s, batch_size):
        self.batch_size = batch_size
        self.target = torch.tensor(
            [ord(i) for i in s], dtype=torch.int32, device=device) - ord('a')
        self.target.unsqueeze_(1)
        self.limits = torch.iinfo(torch.int32)

    def run_job(self, start):
        rand = GlibcRand()
        seed = torch.arange(start,
                            min(self.limits.max + 1, start + self.batch_size))
        rand.srand(seed)
        results = torch.empty((len(self.target), len(seed)),
                              dtype=torch.int32,
                              device=device)
        for row in results:
            row[:] = rand.rand()
        successful = seed[torch.eq(results % 26, self.target).all(dim=0)]
        for i in successful:
            print("Found seed:", int32toUint32(i.item()))

    def run_all(self):
        starts = range(self.limits.min, self.limits.max, self.batch_size)
        if device.type == "cpu":
            from concurrent.futures import ThreadPoolExecutor
            import os

            cpu_count = len(os.sched_getaffinity(0))
            print(f"Using {cpu_count} worker threads")
            with ThreadPoolExecutor() as e:
                e.map(self.run_job, starts)
        else:
            from tqdm import tqdm

            print("Using alternate compute source")
            for start in tqdm(starts):
                self.run_job(start)


if __name__ == "__main__":
    Encoder("heyvic", 2**20).run_all()
