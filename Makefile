ifeq ($(OS),Windows_NT)
	RMDIR = cmd /b /c rmdir /s /q build buildweb buildserver
else
	RMDIR = rm -rf build buildweb buildserver
endif

.PHONY: all

all: desktop server web

server:
	[ -d buildServer ] || mkdir buildServer
	cd buildServer && cmake .. -DSERVER=1 -DCMAKE_BUILD_TYPE=Release
	@echo "Building..."
	cd buildServer && cmake --build . --config Release
	@echo "Done!"

web:
	@echo "Generating Build Files..."
	[ -d buildWeb ] || mkdir buildWeb
	cd buildWeb && emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html"
	@echo "Building..."
	cd buildWeb && emmake make
	cp buildWeb/Poker.wasm "nginx docker/html"
	cp buildWeb/Poker.js "nginx docker/html"

desktop:
	@echo "Generating Build Files..."
	[ -d build ] || mkdir build
	cd build && cmake -DCMAKE_BUILD_TYPE=Release ..
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
