cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(Poker)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# https://stackoverflow.com/questions/11813271/embed-resources-eg-shader-code-images-into-executable-library-with-cmake
# Creates C resources file from files in given directory
function(create_resources dir output)
    # Create empty output file
    
    string(REGEX MATCH "([^/]+)$" header ${output})
    file(WRITE ${output}.h "#ifndef EMBED_RESOURCES_H\n#define EMBED_RESOURCES_H\n\n")
    file(WRITE ${output}.c "#include \"""${header}.h""\""\n)
    # Collect input files
    file(GLOB bins ${dir}/*)
    # Iterate through input files
    foreach(bin ${bins})
        # Get short filename
        string(REGEX MATCH "([^/]+)$" filename ${bin})
        # Replace filename spaces & extension separator for C compatibility
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
        # Read hex data from file
        file(READ ${bin} filedata HEX)
        # Convert hex data for C compatibility
        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
        # Append data to output file
        file(APPEND ${output}.h "extern const unsigned char ${filename}[];\nextern const unsigned ${filename}_size;\n")
        file(APPEND ${output}.c "const unsigned char ${filename}[] = {${filedata}0x00,};\nconst unsigned ${filename}_size = sizeof(${filename});\n")
    endforeach()
    file(APPEND ${output}.h "#endif\n")
endfunction()

if(NOT DEFINED SERVER)
  # Dependencies
  set(RAYLIB_VERSION 4.5.0)
  find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
  if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
      raylib
      DOWNLOAD_EXTRACT_TIMESTAMP OFF
      URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
      set(FETCHCONTENT_QUIET NO)
      FetchContent_Populate(raylib)
      set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
      add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
  endif()
  create_resources("resources/" "src/client/resources")
endif()
# Our Project

file(GLOB_RECURSE COMMON_SOURCES "src/common/*.c")
file(GLOB_RECURSE CLIENT_SOURCES "src/client/*.c")
file(GLOB_RECURSE SERVER_SOURCES "src/server/*.c")

#set(raylib_VERBOSE 1)

if(DEFINED SERVER)
  add_executable(${PROJECT_NAME}Server ${COMMON_SOURCES} ${SERVER_SOURCES})
  add_compile_definitions(SERVER_BUILD)
  set(PLATFORM "Desktop")
  if(WIN32)
    target_link_libraries(${PROJECT_NAME}Server WS2_32)
  endif()
else()
  add_executable(${PROJECT_NAME} ${COMMON_SOURCES} ${CLIENT_SOURCES})
  target_link_libraries(${PROJECT_NAME} raylib)
  if(WIN32)
    target_link_libraries(${PROJECT_NAME} WS2_32)
  endif()
endif()

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WEB=1)
    target_link_libraries(${PROJECT_NAME} websocket.js)
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

else()
    add_compile_definitions(__STDC_WANT_LIB_EXT1__)
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
