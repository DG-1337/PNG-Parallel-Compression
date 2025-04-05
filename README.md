# PNG-Parallel-Compression

PNG Parallel Compressor is a project designed to accelerate the PNG compression process using concurrency and parallelism. By parallelizing both the filtering and deflate compression stages, this tool aims to deliver faster image processing without sacrificing compression quality.

PNG Parallel Compressor is written using mainly C++ for comfortability of multithreading and familiarity. Access to librarys like std::thread and low level memory control makes it ideal for this purpose.

## Directory Structure
```bash
.
├── Enhancement of PNG Compression through Parallelization V0.1.pdf
├── LICENSE
├── README.md
├── include
│   └── lodepng.h
├── src
│   ├── ReadScanLine.cpp
│   ├── compression
│   │   └── compression.cpp
│   ├── filter
│   │   └── filter.cpp
│   ├── lodepng.cpp
│   └── main.cpp
├── test_image.png
└── tests
    └── test_files.cpp
```

## Installation
    
### Compile the Project
     ```
     cd PNG-Parallel-Compression
     g++ src/main.cpp src/lodepng.cpp src/compression/compression.cpp src/filter/adaptive_filter.cpp src/filter/read_scan_line.cpp src/filter/filter.cpp -std=c++11 -stdlib=libc++ -w -Weverything -o program.out -lz
     ```

### Testing
    ```
    g++ tests/test_files.cpp src/lodepng.cpp -std=c++17 -stdlib=libc++ -Weverything -o program_test.out
    ```

    Ensure tests/img folder exists 
### Run the Executable
   ```
    ./program.out
   ```

## Project Overview

We propose a multi-threaded approach to PNG compression by implementing parallelism in
the filtering and compression stages. Our strategy includes:
1. Image Segmentation: Instead of processing the entire image sequentially, we divide it into
smaller chunks (e.g., rows or blocks). Each segment is then processed independently.
2. Parallel Filters: Using C++ and the std::thread library, multiple threads will apply diƯerent
PNG filters simultaneously. The applied filters include:
    * None Filter: Leaves pixel values unchanged.
    * Sub Filter: Modifies each pixel based on the value of the previous pixel in the same
      row.
    * Up Filter: Adjusts pixels based on the corresponding pixel in the row above.
    * Average Filter: Uses the average of the left and upper pixels for encoding.
    * Paeth Filter: Uses the best of left, above, and upper-left pixels.
3. Concurrency in Entropy Encoding: The final compression step involves applying the
Deflate algorithm to encode the filtered image data. This stage can be further optimized
using parallelized HuƯman coding and run-length encoding.

More details can be found within the [PDF document](https://github.com/DG-1337/PNG-Parallel-Compression/blob/main/Enhancement%20of%20PNG%20Compression%20through%20Parallelization%20V0.1.pdf) in the repo

## License
This project is licensed under the Apache License. See the LICENSE file for details.
 
## Acknowledgments
[lodepng](https://github.com/lvandeve/lodepng)
