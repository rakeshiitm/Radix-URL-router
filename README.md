# Radix URL Router

A high-performance, lightweight C++ routing engine that maps web URLs to executable backend functions. Utilizing a custom Radix Tree architecture, this project bypasses standard linear scanning to provide highly optimized route resolution.

## Key Features

* **Optimized Performance:** Achieves strict $O(K)$ time complexity for route resolution, where $K$ is the number of URL segments.
* **Dynamic Parameter Extraction:** Automatically captures and stores dynamic URL variables (e.g., `{id}`) into a hash map for backend processing.
* **Zero Dependencies:** Developed entirely using the C++ Standard Template Library (STL) with no external libraries required.
