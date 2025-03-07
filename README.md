# PNG-Parallel-Compression

PNG Parallel Compressor is a project designed to accelerate the PNG compression process using concurrency and parallelism. By parallelizing both the filtering and deflate compression stages, this tool aims to deliver faster image processing without sacrificing compression quality.

### Installation

1. Compile the Project
     ```bash
     cd PNG-Parallel-Compression
     g++ src/ReadScanLine.cpp src/lodepng.cpp -Wall -Wextra -pedantic -ansi -O3 -o program.out
     ```

2. Run the Executable
   ```bash
    ./program.out
   ```

**Project Overview**

**License**
This project is licensed under the Apache License. See the LICENSE file for details.
 
**Acknowledgments**
[lodepng](https://github.com/lvandeve/lodepng)
