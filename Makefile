PREFIX=install
CMD=build/tools/jcheck

all: build
	cmake --build build 

build:
	cmake -S . -B build -DCMAKE_INSTALL_PREFIX=$(PREFIX)

install: build
	cmake --build build 
	cmake --install build

test:
	./scripts/test.sh $(CMD)

bench:
	./scripts/benchmark.sh $(CMD) | column -s, -t

gnu/jcheck:
	@make -C gnu
	
clean:
	rm -rf build
	@make -C gnu clean

.PHONY: all clean test benchmark
