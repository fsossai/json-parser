INSTALL_DIR=install
EXE=build/examples/checker

all: build
	cmake --build build 

build:
	cmake -S . -B build -DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR)

install: build
	cmake --build build 
	cmake --install build

test:
	./scripts/test.sh $(EXE)

benchmark:
	./scripts/benchmark.sh $(EXE) | column -s, -t

benchmark_gnu: gnu/checker
	./scripts/benchmark.sh gnu/checker | column -s, -t

gnu/checker:
	@make -C gnu
	
clean:
	rm -rf build
	@make -C gnu clean

.PHONY: all clean test benchmark
