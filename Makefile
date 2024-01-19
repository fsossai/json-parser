INSTALL_DIR=install
CMD=build/examples/checker

all: build
	cmake --build build 

build:
	cmake -S . -B build -DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR)

install: build
	cmake --build build 
	cmake --install build

test:
	./scripts/test.sh $(CMD)

benchmark:
	./scripts/benchmark.sh $(CMD) | column -s, -t

benchmark_gnu: gnu/checker
	./scripts/benchmark.sh gnu/checker | column -s, -t

gnu/checker:
	@make -C gnu
	
clean:
	rm -rf build
	@make -C gnu clean

.PHONY: all clean test benchmark
