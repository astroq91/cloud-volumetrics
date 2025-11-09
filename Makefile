all: debug
run-debug: debug
	./build/debug/Clouds
run-release: release 
	./build/release/Clouds

.PHONY: debug
debug:
	cmake --preset debug
	cmake --build build/debug

.PHONY: release 
release:
	cmake --preset release 
	cmake --build build/release

