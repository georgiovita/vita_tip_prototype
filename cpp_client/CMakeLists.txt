cmake_minimum_required(VERSION 3.10)
project(scanner_client)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Paths to FreeGLUT ----
# Adjust this to wherever you unzipped FreeGLUT
set(FREEGLUT_DIR "C:/libs/freeglut")  

include_directories(
    ${FREEGLUT_DIR}/include   # FreeGLUT headers (should contain GL/freeglut.h and GL/freeglut_std.h)
    ${CMAKE_SOURCE_DIR}/include  # Your own headers (e.g., CircularBuffer.h)
)

# ---- Source files ----
set(SOURCES
    src/main.cpp
    # Add other cpp files here if you have more
)

# ---- Create executable ----
add_executable(scanner_client ${SOURCES})

# ---- Link libraries ----
target_link_libraries(scanner_client
    ${FREEGLUT_DIR}/lib/x64/freeglut.lib  # FreeGLUT library
    opengl32                          # OpenGL
    glu32                             # GLU
)

# ---- Optional: specify binary output folder ----
set_target_properties(scanner_client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
