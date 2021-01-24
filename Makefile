help:
	@echo "Targets:"
	@echo "	all"
	@echo "	build/cpp-lambdarand"
	@echo "	build/cpp-vicrand"
	@echo "	build/cpp-fastrand"
	@echo "	build/go-rand"
	@echo "	clean"

all: build/cpp-lambdarand build/cpp-vicrand build/cpp-fastrand build/go-rand

build:
	mkdir build

build/cpp-lambdarand: cpp/lambdarand.cpp cpp/rand.h | build
	g++ -O3 -std=c++20 -pthread -o build/cpp-lambdarand cpp/lambdarand.cpp

build/cpp-vicrand: cpp/vicrand.cpp cpp/rand.h | build
	g++ -O3 -std=c++17 -pthread -o build/cpp-vicrand cpp/vicrand.cpp

build/cpp-fastrand: cpp/fastrand.cpp cpp/rand.h | build
	g++ -O3 -std=c++17 -fopenmp -pthread -o build/cpp-fastrand cpp/fastrand.cpp

build/go-rand: go/* | build
	go build -o build/go-rand ./go

clean:
	rm -rf build

.PHONY: help all clean
