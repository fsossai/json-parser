PREFIX=install

all: build
	cmake --build build 

build:
	cmake -S . -B build -DCMAKE_INSTALL_PREFIX=$(PREFIX)

install: build
	cmake --build build 
	cmake --install build

test:
	@make -C build test

gnu/jcheck:
	@make -C gnu
	
clean:
	rm -rf build
	@make -C gnu clean

.PHONY: all clean test
