ifeq ($(OS),Windows_NT)
	RMDIR = cmd /b /c rmdir /s /q build
else
	RMDIR = rm -rf build
endif

.PHONY: all generate build clean

all: generate build

web:
	@echo "Generating Build Files..."
	[ -d build ] || mkdir build
	cd build && emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html"
	@echo "Building..."
	cd build && emmake make

generate:
	@echo "Generating Build Files..."
	[ -d build ] || mkdir build
	cd build && cmake -DCMAKE_BUILD_TYPE=Release ..

build:
	@echo "Building..."
	cd build && cmake --build . --config Release
	@echo "Done!"
debug:
	[ -d build ] || mkdir build
	cd build && cmake ..
	@echo "Building..."
	cd build && cmake --build .
	@echo "Done!"

clean:
	@echo "Cleaning up..."
	@$(RMDIR)
	@echo "Clean completed."