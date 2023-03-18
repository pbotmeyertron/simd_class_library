#pragma once

/* Standard Headers */
#include <cstdint>
#include <iostream>
#include <cstring>
#include <cmath>
#include <type_traits>
#include <stdexcept>
#include <initializer_list> 

namespace scl {

/*============================================================================*/
/* Signed Integer Types                                                       */
/*============================================================================*/

/* Signed 8-Bit Integer */
using i8  = std::int8_t;
/* Signed 16-Bit Integer */
using i16 = std::int16_t;
/* Signed 32-Bit Integer */
using i32 = std::int32_t;
/* Signed 64-Bit Integer */
using i64 = std::int64_t;

/*============================================================================*/
/* Unsigned Integer Types                                                     */ 
/*============================================================================*/

/* Unsigned 8-Bit Integer */
using u8  = std::uint8_t;
/* Unsigned 16-Bit Integer */
using u16 = std::uint16_t;
/* Unsigned 32-Bit Integer */
using u32 = std::uint32_t;
/* Unsigned 64-Bit Integer */
using u64 = std::uint64_t;

/*============================================================================*/
/* Floating-Point Types                                                       */
/*============================================================================*/

/* 32-Bit Floating-Point Number */
using f32 = float;
/* 64-Bit Floating-Point Number */
using f64 = double;

/*============================================================================*/
/* Mathematical Constants                                                     */
/*============================================================================*/

#define scl_vector_size(n) __attribute__((vector_size(n)))
#define scl_align(n)       __attribute__((aligned(n)))
#define scl_alias          __attribute__((may_alias))

#if defined(__clang__)
    #define scl_inline __attribute__((always_inline))
#elif defined(__GNUC__)
    #define scl_inline __attribute__((always_inline))
#elif defined(_MSC_VER)
    #define scl_inline __forceinline
#endif

/* Used to produce correct alignment for vectors. */
constexpr scl_inline uint32_t 
next_power_of_two(uint32_t x) {
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

/* Reinterprets a value as a different type. Better than reinterpret_cast<> */
template<typename T, typename S>
constexpr static inline T 
reinterpret_as(const S& s) {
    static_assert(sizeof(T) == sizeof(S));
    T d;
    std::memcpy(&d, &s, sizeof(T));
    return d;
}

template<typename T, std::size_t N>
class vector {
public:
    #if defined(__clang__) || defined(__GNUC__)
        scl_align(next_power_of_two(N * sizeof(T))) 
        scl_vector_size(next_power_of_two(N * sizeof(T)))  T vec_type;
    #elif defined(_MSC_VER)
        alignas(next_power_of_two(N * sizeof(T))) T vec_type[N];
    #endif
public:

    /* Default constructor. */
    vector() = default;

    /* Variadic constructor. */
    template<typename... Args, 
             typename std::enable_if<sizeof...(Args) == N, int>::type = 0>
    vector(Args... args) {
        static_assert(std::conjunction<std::is_same<T, Args>...>::value, 
                "All arguments must be of the same type as T");
        vec_type = { static_cast<T>(args)... };
    }

    /* Constructor to broadcast the same value into all elements. */
    vector(const T& value) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] = value;
        }
    }

    /* Constructor to convert from type vector<T, N>. */
    vector(const vector<T, N>& other) {
        vec_type = other.vec_type;
    }

    /* Type cast operator to convert to an array. */
    operator T*() {
        return reinterpret_as<T*>(&vec_type);
    }

    /* Type cast operator to convert to a const array. */
    operator const T*() const {
        return reinterpret_as<const T*>(&vec_type);
    }

    /* Assignment operator to convert from type vector<T, N>. */
    vector<T, N>& operator=(const vector<T, N>& other) {
        vec_type = other.vec_type;
        return *this;
    }

    /* Type cast operator to convert to type vector<T, N>. */
    operator vector<T, N>() const {
        return vector<T, N>(*this);
    }

    /* Member function to store into array (unaligned). */
    constexpr scl_inline void 
    store(T* p) const {
        for (std::size_t i = 0; i < N; ++i) {
            p[i] = vec_type[i];
        }
    }

    /* Member function to extract a single element from vector. */
    constexpr scl_inline T 
    extract_element(const std::size_t& index) const {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        T arr[N];
        store(arr);
        return arr[index & (N - 1)];
    }

    /* Indexing operator for array-style addressing. */
    constexpr scl_inline T 
    operator[](const std::size_t& index) const {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return vec_type[index];
    }

    /*------------------*/
    /* Member Functions */
    /*------------------*/

    /* Member function to set a single element in the vector. */
    constexpr scl_inline void 
    set_element(const std::size_t& index, const T& value) {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        vec_type[index] = value;
    }

    /* Minimum Value in Vector. */
    constexpr scl_inline T
    min() const {
        T result = vec_type[0];
        for (std::size_t i = 1; i < N; ++i) {
            if (vec_type[i] < result) {
                result = vec_type[i];
            }
        }
        return result;
    }

    /* Maximum Value in Vector. */
    constexpr scl_inline T
    max() const {
        T result = vec_type[0];
        for (std::size_t i = 1; i < N; ++i) {
            if (vec_type[i] > result) {
                result = vec_type[i];
            }
        }
        return result;
    }

    /* Sum of All Elements in Vector. */
    constexpr scl_inline T
    horizontal_sum() const {
        T result = vec_type[0];
        for (std::size_t i = 1; i < N; ++i) {
            result += vec_type[i];
        }
        return result;
    }

    /* Product of All Elements in Vector. */
    constexpr scl_inline T
    horizontal_product() const {
        T result = vec_type[0];
        for (std::size_t i = 1; i < N; ++i) {
            result *= vec_type[i];
        }
        return result;
    }

    /* Average of All Elements in Vector. */
    constexpr scl_inline T
    horizontal_average() const {
        return horizontal_sum() / N;
    }

    /*------------------------*/
    /* Mathematical Functions */
    /*------------------------*/

    /* Absolute Value. */
    constexpr scl_inline vector
    abs() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::abs(vec_type[i]);
        }
        return result;
    }

    /* Square Root. */
    constexpr scl_inline vector
    sqrt() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::sqrt(vec_type[i]);
        }
        return result;
    }

    /* Reciprocal Square Root. */
    constexpr scl_inline vector
    rsqrt() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = 1.0 / std::sqrt(vec_type[i]);
        }
        return result;
    }

    /* Cube Root. */
    constexpr scl_inline vector
    cbrt() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::cbrt(vec_type[i]);
        }
        return result;
    }

    /* Reciprocal. */
    constexpr scl_inline vector
    reciprocal() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = 1.0 / vec_type[i];
        }
        return result;
    }

    /* Exponential. */
    constexpr scl_inline vector
    exp() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::exp(vec_type[i]);
        }
        return result;
    }

    /* Natural Logarithm. */
    constexpr scl_inline vector
    log() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::log(vec_type[i]);
        }
        return result;
    }

    /* Base 10 Logarithm. */
    constexpr scl_inline vector
    log10() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::log10(vec_type[i]);
        }
        return result;
    }

    /* Sine. */
    constexpr scl_inline vector
    sin() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::sin(vec_type[i]);
        }
        return result;
    }

    /* Cosine. */
    constexpr scl_inline vector
    cos() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::cos(vec_type[i]);
        }
        return result;
    }

    /* Tangent. */
    constexpr scl_inline vector
    tan() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::tan(vec_type[i]);
        }
        return result;
    }

    /* Arcsine. */
    constexpr scl_inline vector
    asin() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::asin(vec_type[i]);
        }
        return result;
    }

    /* Arccosine. */
    constexpr scl_inline vector
    acos() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::acos(vec_type[i]);
        }
        return result;
    }

    /* Arctangent. */
    constexpr scl_inline vector
    atan() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::atan(vec_type[i]);
        }
        return result;
    }

    /* Hyperbolic Sine. */
    constexpr scl_inline vector
    sinh() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::sinh(vec_type[i]);
        }
        return result;
    }

    /* Hyperbolic Cosine. */
    constexpr scl_inline vector
    cosh() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::cosh(vec_type[i]);
        }
        return result;
    }

    /* Hyperbolic Tangent. */
    constexpr scl_inline vector
    tanh() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::tanh(vec_type[i]);
        }
        return result;
    }

    /* Hyperbolic Arcsine. */
    constexpr scl_inline vector
    asinh() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::asinh(vec_type[i]);
        }
        return result;
    }

    /* Hyperbolic Arccosine. */
    constexpr scl_inline vector
    acosh() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::acosh(vec_type[i]);
        }
        return result;
    }

    /* Hyperbolic Arctangent. */
    constexpr scl_inline vector
    atanh() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::atanh(vec_type[i]);
        }
        return result;
    }

    /* Power. */
    constexpr scl_inline vector
    pow(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::pow(vec_type[i], rhs.vec_type[i]);
        }
        return result;
    }

    /* Round to nearest integer. */
    constexpr scl_inline vector
    round() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::round(vec_type[i]);
        }
        return result;
    }

    /* Round to nearest integer, rounding up. */
    constexpr scl_inline vector
    ceil() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::ceil(vec_type[i]);
        }
        return result;
    }

    /* Round to nearest integer, rounding down. */
    constexpr scl_inline vector
    floor() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::floor(vec_type[i]);
        }
        return result;
    }

    /* Truncate to nearest integer. */
    constexpr scl_inline vector
    trunc() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = std::trunc(vec_type[i]);
        }
        return result;
    }

    /* Sign. */
    constexpr scl_inline vector
    sign() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = (vec_type[i] > 0) - (vec_type[i] < 0);
        }
        return result;
    }

    /*----------------------*/
    /* Arithmetic Operators */
    /*----------------------*/

    constexpr scl_inline vector 
    operator+(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] + rhs.vec_type[i];
        }
        return result;
    }
    
    constexpr scl_inline vector& 
    operator+=(const vector& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] += rhs.vec_type[i];
        }
        return *this;
    }

    constexpr scl_inline vector
    operator+(const T& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] + rhs;
        }
        return result;
    }

    constexpr scl_inline vector&
    operator+=(const T& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] += rhs;
        }
        return *this;
    }

    constexpr scl_inline vector 
    operator-(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] - rhs.vec_type[i];
        }
        return result;
    }

    constexpr scl_inline vector& 
    operator-=(const vector& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] -= rhs.vec_type[i];
        }
        return *this;
    }

    constexpr scl_inline vector
    operator-(const T& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] - rhs;
        }
        return result;
    }

    constexpr scl_inline vector&
    operator-=(const T& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] -= rhs;
        }
        return *this;
    }

    /* Negation */
    constexpr scl_inline vector
    operator-() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = -vec_type[i];
        }
        return result;
    }

    constexpr scl_inline vector 
    operator*(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] * rhs.vec_type[i];
        }
        return result;
    }

    constexpr scl_inline vector
    operator*(const T& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] * rhs;
        }
        return result;
    }

    constexpr scl_inline vector& 
    operator*=(const vector& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] *= rhs.vec_type[i];
        }
        return *this;
    }

    constexpr scl_inline vector&
    operator*=(const T& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] *= rhs;
        }
        return *this;
    }

    constexpr scl_inline vector 
    operator/(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] / rhs.vec_type[i];
        }
        return result;
    }

    constexpr scl_inline vector
    operator/(const T& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] / rhs;
        }
        return result;
    }

    constexpr scl_inline vector& 
    operator/=(const vector& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] /= rhs.vec_type[i];
        }
        return *this;
    }

    constexpr scl_inline vector&
    operator/=(const T& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] /= rhs;
        }
        return *this;
    }

    constexpr scl_inline vector
    operator%(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] % rhs.vec_type[i];
        }
        return result;
    }

    constexpr scl_inline vector
    operator%(const T& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] % rhs;
        }
        return result;
    }

    constexpr scl_inline vector&
    operator%=(const vector& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] %= rhs.vec_type[i];
        }
        return *this;
    }

    constexpr scl_inline vector&
    operator%=(const T& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] %= rhs;
        }
        return *this;
    }

    /*----------------------*/
    /* Comparison Operators */
    /*----------------------*/
    
    constexpr scl_inline bool 
    operator==(const vector& rhs) const {
        for (std::size_t i = 0; i < N; ++i) {
            if (vec_type[i] != rhs.vec_type[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr scl_inline bool 
    operator!=(const vector& rhs) const {
        return !(*this == rhs);
    }

    constexpr scl_inline bool
    operator<(const vector& rhs) const {
        for (std::size_t i = 0; i < N; ++i) {
            if (vec_type[i] >= rhs.vec_type[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr scl_inline bool
    operator<=(const vector& rhs) const {
        for (std::size_t i = 0; i < N; ++i) {
            if (vec_type[i] > rhs.vec_type[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr scl_inline bool
    operator>(const vector& rhs) const {
        for (std::size_t i = 0; i < N; ++i) {
            if (vec_type[i] <= rhs.vec_type[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr scl_inline bool
    operator>=(const vector& rhs) const {
        for (std::size_t i = 0; i < N; ++i) {
            if (vec_type[i] < rhs.vec_type[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr scl_inline vector
    operator&&(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] && rhs.vec_type[i];
        }
        return result;
    }

    constexpr scl_inline vector
    operator||(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] || rhs.vec_type[i];
        }
        return result;
    }

    constexpr scl_inline bool
    operator!() const {
        for (std::size_t i = 0; i < N; ++i) {
            if (vec_type[i]) {
                return false;
            }
        }
        return true;
    }

    /*-------------------*/
    /* Bitwise Operators */
    /*-------------------*/

    constexpr scl_inline vector
    operator&(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] & rhs.vec_type[i];
        }
        return result;
    }

    constexpr scl_inline vector&
    operator&=(const vector& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] &= rhs.vec_type[i];
        }
        return *this;
    }

    constexpr scl_inline vector
    operator|(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] | rhs.vec_type[i];
        }
        return result;
    }

    constexpr scl_inline vector&
    operator|=(const vector& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] |= rhs.vec_type[i];
        }
        return *this;
    }

    constexpr scl_inline vector
    operator^(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] ^ rhs.vec_type[i];
        }
        return result;
    }

    constexpr scl_inline vector&
    operator^=(const vector& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] ^= rhs.vec_type[i];
        }
        return *this;
    }

    constexpr scl_inline vector
    operator~() const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = ~vec_type[i];
        }
        return result;
    }

    /*-------------------*/
    /* Shift Operators   */
    /*-------------------*/

    constexpr scl_inline vector
    operator<<(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] << rhs.vec_type[i];
        }
        return result;
    }

    constexpr scl_inline vector&
    operator<<=(const vector& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] <<= rhs.vec_type[i];
        }
        return *this;
    }

    constexpr scl_inline vector
    operator>>(const vector& rhs) const {
        vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] >> rhs.vec_type[i];
        }
        return result;
    }

    constexpr scl_inline vector&
    operator>>=(const vector& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            vec_type[i] >>= rhs.vec_type[i];
        }
        return *this;
    }

    /*------------------*/
    /* Stream Operators */
    /*------------------*/

    friend std::ostream&
    operator<<(std::ostream& os, const vector& vec) {
        os << "{";
        for (std::size_t i = 0; i < N; ++i) {
            os << vec.vec_type[i];
            if (i != N - 1) {
                os << ", ";
            }
        }
        os << "}";
        return os;
    }

    friend std::istream&
    operator>>(std::istream& is, vector& vec) {
        for (std::size_t i = 0; i < N; ++i) {
            is >> vec.vec_type[i];
        }
        return is;
    }
};

/*============================================================================*/
/* Non-Member Mathematical Functions                                          */
/*============================================================================*/

/* Returns the smaller of two vectors. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
min(const vector<T, N>& lhs, const vector<T, N>& rhs) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::min(lhs[i], rhs[i]));
    }
    return result;
}

/* Returns the larger of two vectors. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
max(const vector<T, N>& lhs, const vector<T, N>& rhs) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::max(lhs[i], rhs[i]));
    }
    return result;
}

/* Returns the absolute value of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
abs(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::abs(vec[i]));
    }
    return result;
}

/* Returns the square root of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
sqrt(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::sqrt(vec[i]));
    }
    return result;
}

/* Inverse square root. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
rsqrt(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, 1.0 / std::sqrt(vec[i]));
    }
    return result;
}

/* Cube root. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
cbrt(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::cbrt(vec[i]));
    }
    return result;
}

/* Returns the sine of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
sin(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::sin(vec[i]));
    }
    return result;
}

/* Returns the cosine of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
cos(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::cos(vec[i]));
    }
    return result;
}

/* Returns the tangent of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
tan(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::tan(vec[i]));
    }
    return result;
}

/* Returns the arcsine of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
asin(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::asin(vec[i]));
    }
    return result;
}

/* Returns the arccosine of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
acos(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::acos(vec[i]));
    }
    return result;
}

/* Returns the arctangent of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
atan(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::atan(vec[i]));
    }
    return result;
}

/* Returns the hyperbolic sine of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
sinh(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::sinh(vec[i]));
    }
    return result;
}

/* Returns the hyperbolic cosine of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
cosh(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::cosh(vec[i]));
    }
    return result;
}

/* Returns the hyperbolic tangent of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
tanh(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::tanh(vec[i]));
    }
    return result;
}

/* Returns the hyperbolic arcsine of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
asinh(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::asinh(vec[i]));
    }
    return result;
}

/* Returns the hyperbolic arccosine of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
acosh(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::acosh(vec[i]));
    }
    return result;
}

/* Returns the hyperbolic arctangent of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
atanh(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::atanh(vec[i]));
    }
    return result;
}

/* Returns the exponential of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
exp(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::exp(vec[i]));
    }
    return result;
}

/* Returns the natural logarithm of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
log(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::log(vec[i]));
    }
    return result;
}

/* Returns the base-10 logarithm of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
log10(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::log10(vec[i]));
    }
    return result;
}

/* Returns the base-2 logarithm of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
log2(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::log2(vec[i]));
    }
    return result;
}

/* Powers a vector by a scalar. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
pow(const vector<T, N>& vec, T scalar) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::pow(vec[i], scalar));
    }
    return result;
}

/* Powers a vector by a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
pow(const vector<T, N>& vec1, const vector<T, N>& vec2) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::pow(vec1[i], vec2[i]));
    }
    return result;
}

/* Hypotenuse of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
hypot(const vector<T, N>& vec1, const vector<T, N>& vec2) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::hypot(vec1[i], vec2[i]));
    }
    return result;
}

/* Floor of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
floor(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::floor(vec[i]));
    }
    return result;
}

/* Ceiling of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
ceil(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::ceil(vec[i]));
    }
    return result;
}

/* Truncates a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
trunc(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::trunc(vec[i]));
    }
    return result;
}

/* Rounds a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
round(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::round(vec[i]));
    }
    return result;
}

/* Returns the nearest integer to a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
nearbyint(const vector<T, N>& vec) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::nearbyint(vec[i]));
    }
    return result;
}

/* Returns the remainder of a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
remainder(const vector<T, N>& vec1, const vector<T, N>& vec2) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::remainder(vec1[i], vec2[i]));
    }
    return result;
}

/* Clamp a vector. */
template<typename T, std::size_t N>
constexpr scl_inline vector<T, N>
clamp(const vector<T, N>& vec, 
      const vector<T, N>& min, 
      const vector<T, N>& max) {
    vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result.set_element(i, std::min(std::max(vec[i], min[i]), max[i]));
    }
    return result;
}

/*============================================================================*/
/* Vector Class Type Aliases.                                                 */
/*============================================================================*/

/*============================================================================*/
/* Signed Vector Types                                                        */
/*============================================================================*/

/* A Vector of 8 Signed, 8-Bit Integers.
 * Is equivalent to (8 * sizeof(i8)) */
using i8x8   = vector<i8,  8>;
/* A Vector of 16 Signed, 8-Bit Integers. 
 * Is equivalent to (16 * sizeof(i8)) */
using i8x16  = vector<i8, 16>;
/* A Vector of 32 Signed, 8-Bit Integers. 
 * Is equivalent to (32 * sizeof(i8)) */
using i8x32  = vector<i8, 32>;
/* A Vector of 64 Signed, 8-Bit Integers. 
 * Is equivalent to (64 * sizeof(i8)) */
using i8x64  = vector<i8, 64>;
/* A Vector of 2 Signed, 16-Bit Integers. 
 * Is equivalent to (2 * sizeof(i16)) */
using i16x2  = vector<i16, 2>;
/* A Vector of 4 Signed, 16-Bit Integers. 
 * Is equivalent to (4 * sizeof(i16)) */
using i16x4  = vector<i16, 4>;
/* A Vector of 8 Signed, 16-Bit Integers. 
 * Is equivalent to (8 * sizeof(i16)) */
using i16x8  = vector<i16, 8>;
/* A Vector of 16 Signed, 16-Bit Integers. 
 * Is equivalent to (16 * sizeof(i16)) */
using i16x16 = vector<i16, 16>;
/* A Vector of 32 Signed, 16-Bit Integers. 
 * Is equivalent to (32 * sizeof(i16)) */
using i16x32 = vector<i16, 32>;
/* A Vector of 2 Signed, 32-Bit Integers. 
 * Is equivalent to (2 * sizeof(i32)) */
using i32x2  = vector<i32, 2>;
/* A Vector of 4 Signed, 32-Bit Integers. 
 * Is equivalent to (4 * sizeof(i32)) */
using i32x4  = vector<i32, 4>;
/* A Vector of 8 Signed, 32-Bit Integers. 
 * Is equivalent to (8 * sizeof(i32)) */
using i32x8  = vector<i32, 8>;
/* A Vector of 16 Signed, 32-Bit Integers.
 * Is equivalent to (16 * sizeof(i32)) */
using i32x16 = vector<i32, 16>;
/* A Vector of 2 Signed, 64-Bit Integers. 
 * Is equivalent to (2 * sizeof(i64)) */
using i64x2  = vector<i64, 2>;
/* A Vector of 4 Signed, 64-Bit Integers. 
 * Is equivalent to (4 * sizeof(i64)) */
using i64x4  = vector<i64, 4>;
/* A Vector of 8 Signed, 64-Bit Integers. 
 * Is equivalent to (8 * sizeof(i64)) */
using i64x8  = vector<i64, 8>;

/*============================================================================*/
/* Unsigned Vector Types                                                      */
/*============================================================================*/

/* A Vector of 8 Unsigned, 8-Bit Integers.
 * Is equivalent to (8 * sizeof(u8)) */
using u8x8   = vector<u8,  8>;
/* A Vector of 16 Unsigned, 8-Bit Integers. 
 * Is equivalent to (16 * sizeof(u8)) */
using u8x16  = vector<u8, 16>;
/* A Vector of 32 Unsigned, 8-Bit Integers. 
 * Is equivalent to (32 * sizeof(u8)) */
using u8x32  = vector<u8, 32>;
/* A Vector of 64 Unsigned, 8-Bit Integers. 
 * Is equivalent to (64 * sizeof(u8)) */
using u8x64  = vector<u8, 64>;
/* A Vector of 2 Unsigned, 16-Bit Integers. 
 * Is equivalent to (2 * sizeof(u16)) */
using u16x2  = vector<u16, 2>;
/* A Vector of 4 Unsigned, 16-Bit Integers. 
 * Is equivalent to (4 * sizeof(u16)) */
using u16x4  = vector<u16, 4>;
/* A Vector of 8 Unsigned, 16-Bit Integers. 
 * Is equivalent to (8 * sizeof(u16)) */
using u16x8  = vector<u16, 8>;
/* A Vector of 16 Unsigned, 16-Bit Integers. 
 * Is equivalent to (16 * sizeof(u16)) */
using u16x16 = vector<u16, 16>;
/* A Vector of 32 Unsigned, 16-Bit Integers. 
 * Is equivalent to (32 * sizeof(u16)) */
using u16x32 = vector<u16, 32>;
/* A Vector of 2 Unsigned, 32-Bit Integers. 
 * Is equivalent to (2 * sizeof(u32)) */
using u32x2  = vector<u32, 2>;
/* A Vector of 4 Unsigned, 32-Bit Integers. 
 * Is equivalent to (4 * sizeof(u32)) */
using u32x4  = vector<u32, 4>;
/* A Vector of 8 Unsigned, 32-Bit Integers. 
 * Is equivalent to (8 * sizeof(u32)) */
using u32x8  = vector<u32, 8>;
/* A Vector of 16 Unsigned, 32-Bit Integers.
 * Is equivalent to (16 * sizeof(u32)) */
using u32x16 = vector<u32, 16>;
/* A Vector of 2 Unsigned, 64-Bit Integers. 
 * Is equivalent to (2 * sizeof(u64)) */
using u64x2  = vector<u64, 2>;
/* A Vector of 4 Unsigned, 64-Bit Integers. 
 * Is equivalent to (4 * sizeof(u64)) */
using u64x4  = vector<u64, 4>;
/* A Vector of 8 Unsigned, 64-Bit Integers. 
 * Is equivalent to (8 * sizeof(u64)) */
using u64x8  = vector<u64, 8>;

/*============================================================================*/
/* Floating Point Vector Types                                                */
/*============================================================================*/

/* A Vector of 2 Single-Precision Floating Point Numbers. 
 * Is equivalent to (2 * sizeof(f32)) */
using f32x2  = vector<f32, 2>;
/* A Vector of 4 Single-Precision Floating Point Numbers. 
 * Is equivalent to (4 * sizeof(f32)) */
using f32x4  = vector<f32, 4>;
/* A Vector of 8 Single-Precision Floating Point Numbers. 
 * Is equivalent to (8 * sizeof(f32)) */
using f32x8  = vector<f32, 8>;
/* A Vector of 16 Single-Precision Floating Point Numbers.
 * Is equivalent to (16 * sizeof(f32)) */
using f32x16 = vector<f32, 16>;
/* A Vector of 2 Double-Precision Floating Point Numbers. 
 * Is equivalent to (2 * sizeof(f64)) */
using f64x2  = vector<f64, 2>;
/* A Vector of 4 Double-Precision Floating Point Numbers. 
 * Is equivalent to (4 * sizeof(f64)) */
using f64x4  = vector<f64, 4>;
/* A Vector of 8 Double-Precision Floating Point Numbers. 
 * Is equivalent to (8 * sizeof(f64)) */
using f64x8  = vector<f64, 8>;

/*============================================================================*/
/* Boolean Vector Types                                                       */
/*============================================================================*/

/* A Vector of 8 Boolean Values. Is equivalent to (8 * sizeof(bool)) */
using bx8   = vector<bool, 8>;
/* A Vector of 16 Boolean Values. Is equivalent to (16 * sizeof(bool)) */
using bx16  = vector<bool, 16>;
/* A Vector of 32 Boolean Values. Is equivalent to (32 * sizeof(bool)) */
using bx32  = vector<bool, 32>;
/* A Vector of 64 Boolean Values. Is equivalent to (64 * sizeof(bool)) */
using bx64  = vector<bool, 64>;
/* A Vector of 128 Boolean Values. Is equivalent to (128 * sizeof(bool)) */
using bx128 = vector<bool, 128>;
/* A Vector of 256 Boolean Values. Is equivalent to (256 * sizeof(bool)) */
using bx256 = vector<bool, 256>;
/* A Vector of 512 Boolean Values. Is equivalent to (512 * sizeof(bool)) */
using bx512 = vector<bool, 512>;

} /* namespace scl */
