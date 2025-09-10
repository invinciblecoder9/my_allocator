# My_Allocator
# User-Space Memory Allocator

This project is a user-space memory allocator implemented in C++. It simulates the behavior of standard library functions like `malloc`, `free`, `calloc`, and `realloc` by managing a pre-allocated pool of memory from the operating system.

<br>

***

## Key Features

* **Core Functions**: Implements the fundamental memory management functions:
    * `my_malloc(size_t size)`
    * `my_free(void* ptr)`
    * `my_calloc(size_t num, size_t size)`
    * `my_realloc(void* ptr, size_t size)`
* **Data Structure**: Utilizes a **doubly linked list** to efficiently track and manage free memory blocks. Each memory chunk includes a custom header with essential metadata.
* **Allocation Strategy**: Employs a **First Fit** algorithm to find the first suitable memory block for a requested size.
* **Fragmentation Handling**: Implements **block splitting** to minimize memory waste and **coalescing** to merge adjacent free blocks, which helps combat external fragmentation.
* **Comprehensive Testing**: Includes a test suite to validate the allocator's functionality under various scenarios, including fragmentation stress tests, zero-initialization (`calloc`), and block resizing (`realloc`).

<br>

***

## Technical Details

* **Environment**: The project was developed and tested in a Linux environment using **GitHub Codespaces**, which is pre-configured with the `g++` compiler.
* **System Calls**: The allocator uses the `mmap()` system call to request a large initial memory pool from the operating system, demonstrating interaction with the kernel.
* **Language**: C++

<br>

***

## How to Build and Run

To build and run this project, you need a C++ compiler. GitHub Codespaces comes with `g++` pre-installed, making it a perfect environment for this project.

1.  Clone the repository:
    ```bash
    git clone [your-repo-url]
    ```
2.  Navigate to the project directory:
    ```bash
    cd [your-repo-name]
    ```
3.  Compile the source files with `g++`:
    ```bash
    g++ my_allocator.cpp main.cpp -o my_allocator
    ```
4.  Run the executable:
    ```bash
    ./my_allocator
    ```

<br>

***

## Project Use Case

This project's primary purpose is not for a commercial application but as a learning tool to demonstrate a deep understanding of core systems programming concepts. It serves as a strong portfolio piece for roles in systems software engineering by showcasing my ability to:

* Build a foundational system component from scratch.
* Work with low-level memory management.
* Apply data structures to solve complex problems.
* Write robust and testable code.