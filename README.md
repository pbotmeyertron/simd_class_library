# SIMD Class Library (SCL)
A single-header vector class that takes advantage of GCC's, Clang's, and MSVC's auto-vectorization features and produces highly optimized SIMD code without the need to explicitly invoke SIMD functions. Even if your target architecture doesn't natively support SIMD, it should still be portable as long as you have a C++20-compatible compiler or higher.

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

# Features
SCL is very intuitive and easy to use. There are a number of pre-defined type-aliases derived from the base class that correspond to common bit-widths of vector extensions. These are:
## Signed Integer Types
| TXN    | vector<T, N>    | Bit Width |
|--------|-----------------|-----------|
| i8x8   | vector<i8, 8>   | 64        |
| i8x16  | vector<i8, 16>  | 128       |
| i8x32  | vector<i8, 32>  | 256       |
| i8x64  | vector<i8, 64>  | 512       |
| i16x4  | vector<i16, 4>  | 64        |
| i16x8  | vector<i16, 8>  | 128       |
| i16x16 | vector<i16, 16> | 256       |
| i16x32 | vector<i16, 32> | 512       |
| i32x2  | vector<i32, 2>  | 64        |
| i32x4  | vector<i32, 5>  | 128       |
| i32x8  | vector<i32, 8>  | 256       |
| i32x16 | vector<i32, 16> | 512       |
| i64x2  | vector<i64, 2>  | 128       |
| i64x4  | vector<i64, 4>  | 256       |
| i64x8  | vector<i64, 8>  | 512       |

## Unsigned Integer Types
| TXN    | vector<T, N>    | Bit Width |
|--------|-----------------|-----------|
| u8x8   | vector<u8, 8>   | 64        |
| u8x16  | vector<u8, 16>  | 128       |
| u8x32  | vector<u8, 32>  | 256       |
| u8x64  | vector<u8, 64>  | 512       |
| u16x4  | vector<u16, 4>  | 64        |
| u16x8  | vector<u16, 8>  | 128       |
| u16x16 | vector<u16, 16> | 256       |
| u16x32 | vector<u16, 32> | 512       |
| u32x2  | vector<u32, 2>  | 64        |
| u32x4  | vector<u32, 5>  | 128       |
| u32x8  | vector<u32, 8>  | 256       |
| u32x16 | vector<u32, 16> | 512       |
| u64x2  | vector<u64, 2>  | 128       |
| u64x4  | vector<u64, 4>  | 256       |
| u64x8  | vector<u64, 8>  | 512       |

## Floating-Point Vector Types
| TXN    | vector<T, N>    | Bit Width |
|--------|-----------------|-----------|
| f32x2   | vector<f32, 2>   | 64        |
| f32x4   | vector<f32, 4>   | 128       |
| f32x8   | vector<f32, 8>   | 256       |
| f32x16  | vector<f32, 16>  | 512       |
| f64x2   | vector<f64, 2>   | 128        |
| f64x4   | vector<f64, 4>   | 256       |
| f64x8   | vector<f64, 8>   | 512       |

More types can be easily defined. For example, a 1024-bit vector of 16 doubles might be: vector<f64, 16>. 

## Overloads and Mathematical Functions
In addition, all common mathematical functions as well as arithmetic operators have been added to each type. This means that each type can be used just like you'd use any standard C data type. See below for example usage.

# Usage and Compilation
To begin using SCL, just simply include the SCL header (scl.hpp) in your project, and you're good to go. It is recommended that vector widths be kept to powers of two so that the compiler can align the data along the preferred architecture's vector extension registers.


```cpp

using namespace scl;

auto arithmetic_test(const f64x4& a, const f64x4& b, const f64x4& c, const f64x4& d) {
    return scl::sqrt((a - b * c) / d);
}

```



