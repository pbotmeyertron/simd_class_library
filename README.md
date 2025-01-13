# SIMD Class Library (SCL)
A single-header vector class that takes advantage of GCC's, Clang's, and MSVC's auto-vectorization features and produces highly optimized SIMD code without the need to explicitly invoke SIMD functions. Even if your target architecture doesn't natively support SIMD, it should still be portable as long as you have a C++20-compatible compiler or higher. The library is primarily designed for Clang in mind, and the vectorization works best with that compiler, but GCC typically produces good results as well, with MSVC making a decent effort, as it usually does.

# Tested Architectures
While all architectures that can support a C++20 compiler should technically work, there are platforms that will generate better code than others.

Platforms with vector extensions that have been tested are:
  - x86-64
  - AArch64 (ARM)
  - RISC-V
  - WebAssembly
  - PowerPC
  - MIPS
  - SPARC

# Usage and Compilation
To begin using SCL, just simply include the SCL header (scl.hpp) in your project, and you're good to go. It is recommended that vector widths be kept to powers of two so that the compiler can align the data along the preferred architecture's vector extension registers.


```cpp

using namespace scl;

auto arithmetic_test(const simd<float, 8>& a, const simd<float, 8>& b, const simd<float, 8>& c, const simd<float, 8>& d) {
    return ((a - b * c) / d).horizontal_sum();
}

```



