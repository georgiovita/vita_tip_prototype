# Vita Tip Scanner Client

A C++ OpenGL application for visualizing scanner data with FreeGLUT.

## Prerequisites

Before building this project, you need to install the following dependencies:

### Required Software
- **CMake** (version 3.10 or higher)
- **C++ Compiler** (Visual Studio 2019/2022, MinGW, or GCC)
- **FreeGLUT** library

### Installing Dependencies

#### Windows (Visual Studio)
1. Install Visual Studio 2019 or 2022 with C++ development tools
2. Install CMake from [cmake.org](https://cmake.org/download/)
3. Download FreeGLUT:
   - Go to [FreeGLUT SourceForge](https://sourceforge.net/projects/freeglut/)
   - Download the Windows binaries
   - Extract to `C:/libs/freeglut/` (or update the path in CMakeLists.txt)

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install cmake build-essential freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev
```

#### macOS
```bash
# Install Xcode command line tools
xcode-select --install

# Install dependencies via Homebrew
brew install cmake freeglut
```

## Building the Project

### Step 1: Clone the Repository
```bash
git clone https://github.com/georgiovita/vita_tip_prototype.git
cd vita_tip_prototype
```

### Step 2: Create Build Directory
```bash
mkdir build
cd build
```

### Step 3: Configure with CMake
```bash
# Windows (Visual Studio)
cmake ../cpp_client

# Linux/macOS
cmake ../cpp_client
```

### Step 4: Build the Project
```bash
# Windows (Visual Studio)
cmake --build . --config Release

# Linux/macOS
cmake --build .
```

### Step 5: Run the Application
```bash
# Windows
../cpp_client/bin/Release/scanner_client.exe

# Linux/macOS
../cpp_client/bin/scanner_client
```

## Project Structure

```
vita_tip_prototype/
├── cpp_client/                 # Main C++ application
│   ├── include/               # Header files
│   │   ├── ArchiveClient.h
│   │   ├── CircularBuffer.h
│   │   └── third_party/
│   │       └── json.hpp       # JSON library
│   ├── src/                   # Source files
│   │   ├── main.cpp
│   │   ├── ArchiveClient.cpp
│   │   └── CircularBuffer.cpp
│   ├── bin/                   # Compiled executables
│   └── CMakeLists.txt         # CMake configuration
├── sample_archives/           # Sample data files
├── libs/                      # Static libraries
└── build/                     # Build directory (created during build)
```

## Configuration

### FreeGLUT Path
If you installed FreeGLUT in a different location, update the path in `cpp_client/CMakeLists.txt`:

```cmake
set(FREEGLUT_DIR "C:/your/path/to/freeglut")
```

### Sample Data
The application expects sample data files in the `sample_archives/` directory. These should be JSON files containing scanner data.

## Usage

1. Run the executable
2. The application will load sample data from the `sample_archives/` directory
3. Use the OpenGL window to visualize the scanner data
4. The application supports different energy modes and visualization options

## Troubleshooting

### Common Issues

**CMake can't find FreeGLUT:**
- Ensure FreeGLUT is installed in the correct directory
- Update the `FREEGLUT_DIR` path in CMakeLists.txt
- On Linux, install the development packages: `sudo apt install freeglut3-dev`

**Build errors on Windows:**
- Ensure Visual Studio C++ tools are installed
- Check that CMake is in your PATH
- Verify FreeGLUT library files are present

**Runtime errors:**
- Ensure `freeglut.dll` is in the same directory as the executable (Windows)
- Check that sample data files are present in `sample_archives/`

## Development

### Adding New Features
1. Add new source files to the `SOURCES` list in `CMakeLists.txt`
2. Add corresponding header files to the `include/` directory
3. Rebuild the project

### Dependencies
- **FreeGLUT**: OpenGL utility toolkit
- **OpenGL**: Graphics rendering
- **nlohmann/json**: JSON parsing library (header-only)

## License

This project is part of the Vita Tip Prototype system.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## Support

For issues and questions, please create an issue in the GitHub repository.
