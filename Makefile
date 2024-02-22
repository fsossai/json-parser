PREFIX=install
GENERATOR="Unix Makefiles" # or Ninja
JOBS=2

all: build
	cmake --build build -- -j$(JOBS)

build:
	cmake -S . -B build -G$(GENERATOR) -DCMAKE_INSTALL_PREFIX=$(PREFIX)

install: build
	cmake --build build -- -j$(JOBS)
	cmake --install build

uninstall:
	-cat build/install_manifest.txt | xargs rm -f

test:
	$(MAKE) -C build test

gnu/jcheck:
	$(MAKE) -C gnu
	
clean:
	rm -rf build
	rm -f compile_commands.json
	$(MAKE) -C gnu clean

.PHONY: all clean install test
