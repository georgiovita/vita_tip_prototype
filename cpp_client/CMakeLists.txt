cmake_minimum_required(VERSION 3.10)
project(scanner_client)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Paths to FreeGLUT ----
# Try to find FreeGLUT automatically, fallback to default path
find_path(FREEGLUT_INCLUDE_DIR GL/freeglut.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/homebrew/include
        "C:/libs/freeglut/include"
        "C:/Program Files/freeglut/include"
    DOC "FreeGLUT include directory"
)

find_library(FREEGLUT_LIBRARY
    NAMES freeglut freeglut_static
    PATHS
        /usr/lib
        /usr/local/lib
        /opt/homebrew/lib
        "C:/libs/freeglut/lib/x64"
        "C:/Program Files/freeglut/lib"
    DOC "FreeGLUT library"
)

# If not found, use default path (for Windows)
if(NOT FREEGLUT_INCLUDE_DIR)
    set(FREEGLUT_DIR "C:/libs/freeglut")
    set(FREEGLUT_INCLUDE_DIR "${FREEGLUT_DIR}/include")
    set(FREEGLUT_LIBRARY "${FREEGLUT_DIR}/lib/x64/freeglut.lib")
    message(STATUS "Using default FreeGLUT path: ${FREEGLUT_DIR}")
else()
    message(STATUS "Found FreeGLUT include: ${FREEGLUT_INCLUDE_DIR}")
    message(STATUS "Found FreeGLUT library: ${FREEGLUT_LIBRARY}")
endif()

include_directories(
    ${FREEGLUT_INCLUDE_DIR}   # FreeGLUT headers
    ${CMAKE_SOURCE_DIR}/include  # Your own headers
)

# ---- Source files ----
set(SOURCES
    src/main.cpp
    src/ArchiveClient.cpp
    src/CircularBuffer.cpp
)

# ---- Create executable ----
add_executable(scanner_client ${SOURCES})

# ---- Link libraries ----
target_link_libraries(scanner_client
    ${FREEGLUT_LIBRARY}  # FreeGLUT library
    opengl32             # OpenGL (Windows)
    glu32                # GLU (Windows)
)

# Platform-specific libraries
if(UNIX)
    target_link_libraries(scanner_client
        GL
        GLU
        X11
    )
endif()

# ---- Optional: specify binary output folder ----
set_target_properties(scanner_client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
