INSTALL_DIR=install
CMD=build/tools/jcheck

all: build
	cmake --build build 

build:
	cmake -S . -B build -DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR)

install: build
	cmake --build build 
	cmake --install build

test:
	./scripts/test.sh $(CMD)

bench:
	./scripts/benchmark.sh $(CMD) | column -s, -t

bench_gnu: gnu/jcheck
	./scripts/benchmark.sh gnu/jcheck | column -s, -t

gnu/jcheck:
	@make -C gnu
	
clean:
	rm -rf build
	@make -C gnu clean

.PHONY: all clean test benchmark
