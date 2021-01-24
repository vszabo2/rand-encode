package main

const (
	DEG_3   = 31
	SEP_3   = 3
)

type GlibcRand struct {
	rand_table [DEG_3]int32
	fptr       int32
	rptr       int32
}

func (r *GlibcRand) Srand(seed int32) {
	r.rand_table = [DEG_3]int32{
		-1726662223, 379960547, 1735697613, 1040273694, 1313901226,
		1627687941, -179304937, -2073333483, 1780058412, -1989503057,
		-615974602, 344556628, 939512070, -1249116260, 1507946756,
		-812545463, 154635395, 1388815473, -1926676823, 525320961,
		-1009028674, 968117788, -123449607, 1284210865, 435012392,
		-2017506339, -911064859, -370259173, 1132637927, 1398500161,
		-205601318,
	}
	r.fptr = SEP_3
	r.rptr = 0

	if seed == 0 {
		seed = 1
	}

	r.rand_table[0] = seed
	dst := 0
	kc := DEG_3

	for i := 1; i < kc; i++ {
		hi := seed / 127773
		lo := seed % 127773
		seed = 16807*lo - 2836*hi
		if seed < 0 {
			seed += 2147483647
		}
		dst++
		r.rand_table[dst] = seed
	}

	for kc *= 10; kc > 0; kc-- {
		r.Rand()
	}
}

func (r *GlibcRand) Rand() (result int32) {
	temp := &r.rand_table[r.fptr]
	*temp += r.rand_table[r.rptr]
	result = int32(uint32(*temp) >> 1)
	r.fptr++

	if r.fptr >= DEG_3 {
		r.fptr = 0
		r.rptr++
	} else {
		r.rptr++
		if r.rptr >= DEG_3 {
			r.rptr = 0
		}
	}

	return
}
