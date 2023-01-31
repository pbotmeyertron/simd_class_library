#pragma once

/* Standard Headers */
#include <cstdint>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
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

/* A circle's circumference divided by its radius. */
template<typename T> constexpr static inline T PI
{static_cast<T>(3.141592653589793238462643383279502884)};

/* sqrt(2) */
template<typename T> constexpr static inline T SQRT_2
{static_cast<T>(1.414213562373095048801688724209698079)};

/* sqrt(3) */
template<typename T> constexpr static inline T SQRT_3
{static_cast<T>(1.732050807568877293527446341505872366)};

/* 1 / sqrt(2) */
template<typename T> constexpr static inline T INV_SQRT_2
{static_cast<T>(0.707106781186547524400844362104849039)};

/* e */
template<typename T> constexpr static inline T E
{static_cast<T>(2.718281828459045235360287471352662498)};

/* Euler-Mascheroni Constant */
template<typename T> constexpr static inline T EULER
{static_cast<T>(0.577215664901532860606512090082402431)};

/* Golden ratio */
template<typename T> constexpr static inline T PHI
{static_cast<T>(1.618033988749894848204586834365638118)};

/* Apery's Constant */
template<typename T> constexpr static inline T APERY
{static_cast<T>(1.202056903159594285399738161511449991)};

/* Catalan's Constant */
template<typename T> constexpr static inline T CATALAN
{static_cast<T>(0.915965594177219015054603514932384110)};

/* Khinchin's Constant */
template<typename T> constexpr static inline T KHINCHIN
{static_cast<T>(2.685452001065306445309714835481795693)};

/* Feigenbaum's Constant */
template<typename T> constexpr static inline T FEIGENBAUM
{static_cast<T>(4.669201609102990671853203820466201617)};

/* Landau's Constant */
template<typename T> constexpr static inline T LANDAU
{static_cast<T>(0.5)};

/* Glaisher-Kinkelin Constant */
template<typename T> constexpr static inline T GLAISHER_KINKELIN
{static_cast<T>(1.282427129100622636875342568869791727)};

/* Napier's Constant */
template<typename T> constexpr static inline T NAPIER
{static_cast<T>(2.685452001065306445309714835481795693)};

/* Planck's Constant */
template<typename T> constexpr static inline T PLANCK
{static_cast<T>(6.626070040e-34)};

/* Boltzmann's Constant */
template<typename T> constexpr static inline T BOLTZMANN
{static_cast<T>(1.38064852e-23)};

/* Avogadro's Constant */
template<typename T> constexpr static inline T AVOGADRO
{static_cast<T>(6.022140857e23)};

/* Faraday's Constant */
template<typename T> constexpr static inline T FARADAY
{static_cast<T>(96485.33289)};

/* Gas Constant */
template<typename T> constexpr static inline T GAS_CONSTANT
{static_cast<T>(8.3144598)};

/* Gravitational Constant */
template<typename T> constexpr static inline T GRAVITATIONAL_CONSTANT
{static_cast<T>(6.67408e-11)};

/* Speed of Light */
template<typename T> constexpr static inline T SPEED_OF_LIGHT
{static_cast<T>(299792458)};

/* Vacuum Permeability */
template<typename T> constexpr static inline T VACUUM_PERMEABILITY
{static_cast<T>(1.256637061435917295385057353311801153)};

/* Vacuum Permittivity */
template<typename T> constexpr static inline T VACUUM_PERMITTIVITY
{static_cast<T>(8.854187817620389850536563031710750485)};

/* Elementary Charge */
template<typename T> constexpr static inline T ELEMENTARY_CHARGE
{static_cast<T>(1.602176634e-19)};

/* Electron Mass */
template<typename T> constexpr static inline T ELECTRON_MASS
{static_cast<T>(9.1093837015e-31)};

/* Proton Mass */
template<typename T> constexpr static inline T PROTON_MASS
{static_cast<T>(1.67262192369e-27)};

/* Neutron Mass */
template<typename T> constexpr static inline T NEUTRON_MASS
{static_cast<T>(1.67492749804e-27)};

/* Rydberg Constant */
template<typename T> constexpr static inline T RYDBERG_CONSTANT
{static_cast<T>(10973731.568539)};

/*============================================================================*/
/* Macros Used For Vector Class                                               */
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
constexpr scl_inline u32 
next_power_of_two(u32 x) {
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

/*============================================================================*/
/*============================================================================*/
/*                                                                            */
/* Vector Class Template                                                      */
/*                                                                            */
/*============================================================================*/
/*============================================================================*/

template<typename T, std::size_t N>
class vector {
public:
    #if defined(__clang__) || defined(__GNUC__)
        scl_align(next_power_of_two(N * sizeof(T))) 
        scl_vector_size(next_power_of_two(N * sizeof(T)))  T vec_type;
    #elif defined(_MSC_VER)
        alignas(next_power_of_two(N * sizeof(T))) T vec_type[N];
    #endif
private:

    /* Variadic template to construct N number of vector elements
     * recursively. */
    template<typename... Args>
    constexpr scl_inline void 
    recursion_construct(const u32& i, const T& first, const Args&... args) {
        vec_type[i] = first;
        recursion_construct(i + 1, args...);
    }

    constexpr scl_inline vector
    load(const T* array) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = array[i];
        }
        return *this;
    }

    constexpr scl_inline void
    store(T* array) {
        for (u32 i = 0; i < N; ++i) {
            array[i] = vec_type[i];
        }
    }

    constexpr scl_inline vector&
    insert_element(const i32& index, const T& value) {
        vec_type[index & (N - 1)] = value;
        return *this;
    }

    constexpr scl_inline T&
    extract_element(const u32& index) {
        T x[N];
        store(x);
        return x[index & (N - 1)];
    }

public:

    /*--------------*/
    /* Constructors */
    /*--------------*/

    /* Default constructor. */
    constexpr scl_inline 
    vector() = default;

    /* Copy constructor. */
    constexpr scl_inline 
    vector(const vector&) = default;

    /* Move constructor. */
    constexpr scl_inline 
    vector(vector&&) = default;

    /* Construct from scalar. */
    constexpr scl_inline 
    vector(const T& scalar) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = scalar;
        }
    }

    /* Construct from array. */
    constexpr scl_inline 
    vector(const T* array) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = array[i];
        }
    }

    /* Construct recursively from individual elements. */
    template<typename... Args>
    constexpr scl_inline 
    vector(const T& first, const Args&... args) {
        static_assert(sizeof...(args) == N - 1, 
            "Invalid number of elements.");
        vec_type[0] = first;
        recursion_construct(1, args...);
    }    

    /* Construct from initializer list. */
    constexpr scl_inline 
    vector(const std::initializer_list<T>& list) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = list.begin()[i];
        }
    }

    /* Construct from std::vector. */
    constexpr scl_inline 
    vector(const std::vector<T>& vector) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = vector[i];
        }
    }

    /* Construct from std::array. */
    constexpr scl_inline 
    vector(const std::array<T, N>& array) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = array[i];
        }
    }

    /* Copy assignment. */
    constexpr scl_inline 
    vector& operator=(const vector&) = default;

    /* Move assignment. */
    constexpr scl_inline 
    vector& operator=(vector&&) = default;

    /* Assign from scalar. */
    constexpr scl_inline
    vector& operator=(const T& scalar) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = scalar;
        }
        return *this;
    }

    /* Assign from array. */
    constexpr scl_inline
    vector& operator=(const T* array) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = array[i];
        }
        return *this;
    }

    /* Assign from initializer list. */
    constexpr scl_inline
    vector& operator=(const std::initializer_list<T>& list) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = list.begin()[i];
        }
        return *this;
    }

    /* Extract a single element. Use store function if extracting more than
     * one element. Operator[] can only read an element, not write. */
    f64 operator[](const u32& index) const {
        return extract_element(index);
    }

    /* Cast to pointer. */
    constexpr scl_inline operator T*() {
        return vec_type;
    }

    /* Cast to const pointer. */
    constexpr scl_inline operator const T*() const {
        return vec_type;
    }

    /* Cast to std::array. */
    constexpr scl_inline operator std::array<T, N>() const {
        std::array<T, N> array;
        for (u32 i = 0; i < N; ++i) {
            array[i] = vec_type[i];
        }
        return array;
    }

    /* Cast to std::vector. */
    constexpr scl_inline operator std::vector<T>() const {
        std::vector<T> vector(N);
        for (u32 i = 0; i < N; ++i) {
            vector[i] = vec_type[i];
        }
        return vector;
    }

    /* Constructor to enable mixing between float and double. */
    template<typename U>
    constexpr scl_inline
    vector(const vector<U, N>& other) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = static_cast<T>(other[i]);
        }
    }

    /* Constructor to enable mixing between floating-point types and integer
     * types. */
    template<typename U>
    constexpr scl_inline
    vector(const vector<U, N>& other, std::enable_if_t<
        std::is_integral<U>::value && std::is_floating_point<T>::value, 
        int> = 0) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = static_cast<T>(other[i]);
        }
    }

    /* Destructor. */
    ~vector() = default;

    /*------------------*/
    /* Member Functions */
    /*------------------*/

    constexpr scl_inline u32 
    extract_element(const u64& index) const {
        return vec_type[index];
    }

    constexpr scl_inline void 
    insert_element(const u64& index, const T& value) {
        vec_type[index] = value;
    }

    constexpr scl_inline vector& 
    set_all(const T& value) {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = value;
        }
        return *this;
    }

    constexpr scl_inline vector& 
    set_zero() {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = 0;
        }
        return *this;
    }

    constexpr scl_inline vector& 
    set_one() {
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] = 1;
        }
        return *this;
    }

    /* Length. */
    constexpr scl_inline T
    length() const {
        T result = 0;
        for (u32 i = 0; i < N; ++i) {
            result += vec_type[i] * vec_type[i];
        }
        return std::sqrt(result);
    }

    /* Length Squared. */
    constexpr scl_inline T
    length_squared() const {
        T result = 0;
        for (u32 i = 0; i < N; ++i) {
            result += vec_type[i] * vec_type[i];
        }
        return result;
    }

    /* Normalize. */
    constexpr scl_inline vector&
    normalize() {
        T len = length();
        for (u32 i = 0; i < N; ++i) {
            vec_type[i] /= len;
        }
        return *this;
    }

    /*-------------------------------*/
    /* Mathematical Member Functions */
    /*-------------------------------*/

    /* Minimum Value in Vector. */
    constexpr scl_inline T 
    min() const {
        T result = vec_type[0];
        for (u32 i = 1; i < N; ++i) {
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
        for (u32 i = 1; i < N; ++i) {
            if (vec_type[i] > result) {
                result = vec_type[i];
            }
        }
        return result;
    }

    /* Sum of Vector Elements. */
    constexpr scl_inline T 
    horizontal_sum() const {
        T result = 0;
        for (u32 i = 0; i < N; ++i) {
            result += vec_type[i];
        }
        return result;
    }

    /* Average of Vector Elements. */
    constexpr scl_inline T 
    avg() const {
        T result = 0;
        for (u32 i = 0; i < N; ++i) {
            result += vec_type[i];
        }
        return result / N;
    }

    /* Absolute value. */
    constexpr scl_inline vector 
    abs() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::abs(vec_type[i]);
        }
        return result;
    }

    /* Square root. */
    constexpr scl_inline vector 
    sqrt() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::sqrt(vec_type[i]);
        }
        return result;
    }

    /* Reciprocal square root. */
    constexpr scl_inline vector
    rsqrt() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = (1 / std::sqrt(vec_type[i]));
        }
        return result;
    }

    /* Reciprocal. */
    constexpr scl_inline vector
    rcp() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = 1 / vec_type[i];
        }
        return result;
    }

    /* Cube root. */
    constexpr scl_inline vector
    cbrt() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::cbrt(vec_type[i]);
        }
        return result;
    }

    /* Exponential. */
    constexpr scl_inline vector 
    exp() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::exp(vec_type[i]);
        }
        return result;
    }

    /* Exponential minus one. */
    constexpr scl_inline vector
    expm1() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::expm1(vec_type[i]);
        }
        return result;
    }

    /* Exponential base 10. */
    constexpr scl_inline vector
    exp10() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::pow(10, vec_type[i]);
        }
        return result;
    }

    /* Natural logarithm. */
    constexpr scl_inline vector 
    log() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::log(vec_type[i]);
        }
        return result;
    }

    /* Base 10 logarithm. */
    constexpr scl_inline vector 
    log10() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::log10(vec_type[i]);
        }
        return result;
    }

    /* Logarithm plus one. */
    constexpr scl_inline vector
    log1p() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::log1p(vec_type[i]);
        }
        return result;
    }

    /* Logarithm base 2. */
    constexpr scl_inline vector
    log2() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::log2(vec_type[i]);
        }
        return result;
    }

    /* Logarithm base e. */
    constexpr scl_inline vector
    logb() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::logb(vec_type[i]);
        }
        return result;
    }

    /* Power. */
    constexpr scl_inline vector 
    pow(const vector& other) const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::pow(vec_type[i], other.vec_type[i]);
        }
        return result;
    }

    /* Sine. */
    constexpr scl_inline vector 
    sin() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::sin(vec_type[i]);
        }
        return result;
    }

    /* Cosine. */
    constexpr scl_inline vector 
    cos() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::cos(vec_type[i]);
        }
        return result;
    }

    /* Tangent. */
    constexpr scl_inline vector 
    tan() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::tan(vec_type[i]);
        }
        return result;
    }

    /* Arcsine. */
    constexpr scl_inline vector 
    asin() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::asin(vec_type[i]);
        }
        return result;
    }

    /* Arccosine. */
    constexpr scl_inline vector 
    acos() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::acos(vec_type[i]);
        }
        return result;
    }

    /* Arctangent. */
    constexpr scl_inline vector 
    atan() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::atan(vec_type[i]);
        }
        return result;
    }

    /* Hyperbolic sine. */
    constexpr scl_inline vector 
    sinh() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::sinh(vec_type[i]);
        }
        return result;
    }

    /* Hyperbolic cosine. */
    constexpr scl_inline vector 
    cosh() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::cosh(vec_type[i]);
        }
        return result;
    }

    /* Hyperbolic tangent. */
    constexpr scl_inline vector 
    tanh() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::tanh(vec_type[i]);
        }
        return result;
    }

    /* Arc hyperbolic sine. */
    constexpr scl_inline vector 
    asinh() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::asinh(vec_type[i]);
        }
        return result;
    }

    /* Arc hyperbolic cosine. */
    constexpr scl_inline vector 
    acosh() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::acosh(vec_type[i]);
        }
        return result;
    }

    /* Arc hyperbolic tangent. */
    constexpr scl_inline vector 
    atanh() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::atanh(vec_type[i]);
        }
        return result;
    }

    /* Hypotenuse. */
    constexpr scl_inline vector
    hypot(const vector& other) const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::hypot(vec_type[i], other.vec_type[i]);
        }
        return result;
    }

    /* Round. */
    constexpr scl_inline vector 
    round() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::round(vec_type[i]);
        }
        return result;
    }

    /* Floor. */
    constexpr scl_inline vector 
    floor() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::floor(vec_type[i]);
        }
        return result;
    }

    /* Ceiling. */
    constexpr scl_inline vector 
    ceil() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::ceil(vec_type[i]);
        }
        return result;
    }

    /* Truncate. */
    constexpr scl_inline vector 
    trunc() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::trunc(vec_type[i]);
        }
        return result;
    }

    /* Sign. */
    constexpr scl_inline vector 
    sign() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = (vec_type[i] > 0) - (vec_type[i] < 0);
        }
        return result;
    }

    /* Fractional part. */
    constexpr scl_inline vector
    fract() const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = vec_type[i] - std::floor(vec_type[i]);
        }
        return result;
    }

    /* Modulus. */
    constexpr scl_inline vector
    mod(const vector& other) const {
        vector result;
        for (u32 i = 0; i < N; ++i) {
            result.vec_type[i] = std::fmod(vec_type[i], other.vec_type[i]);
        }
        return result;
    }
}; // csr::vector

/*============================================================================*/
/* Vector Class Operators                                                     */
/*============================================================================*/

/*----------------------*/
/* Arithmetic Operators */
/*----------------------*/

/* Vector addition. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator+(const vector<T, N>& a, const vector<T, N>& b) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = a.vec_type[i] + b.vec_type[i];
    }
    return result;
}

/* Vector scalar addition. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator+(const vector<T, N>& v, const T& s) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = v.vec_type[i] + s;
    }
    return result;
}

/* Vector scalar addition. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator+(const T& s, const vector<T, N>& v) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = v.vec_type[i] + s;
    }
    return result;
}

/* Vector subtraction. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator-(const vector<T, N>& a, const vector<T, N>& b) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = a.vec_type[i] - b.vec_type[i];
    }
    return result;
}

/* Vector scalar subtraction. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator-(const vector<T, N>& v, const T& s) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = v.vec_type[i] - s;
    }
    return result;
}

/* Vector scalar subtraction. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator-(const T& s, const vector<T, N>& v) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = s - v.vec_type[i];
    }
    return result;
}

/* Vector negation. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator-(const vector<T, N>& v) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = -v.vec_type[i];
    }
    return result;
}

/* Vector multiplication. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator*(const vector<T, N>& a, const vector<T, N>& b) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = a.vec_type[i] * b.vec_type[i];
    }
    return result;
}

/* Vector scalar multiplication. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator*(const vector<T, N>& v, const T& s) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = v.vec_type[i] * s;
    }
    return result;
}

/* Vector scalar multiplication. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator*(const T& s, const vector<T, N>& v) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = v.vec_type[i] * s;
    }
    return result;
}

/* Vector division. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator/(const vector<T, N>& a, const vector<T, N>& b) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = a.vec_type[i] / b.vec_type[i];
    }
    return result;
}

/* Vector scalar division. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator/(const vector<T, N>& v, const T& s) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = v.vec_type[i] / s;
    }
    return result;
}

/* Vector scalar division. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator/(const T& s, const vector<T, N>& v) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = s / v.vec_type[i];
    }
    return result;
}

/* Vector addition assignment. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>&
operator+=(vector<T, N>& a, const vector<T, N>& b) {
    for (u32 i = 0; i < N; ++i) {
        a.vec_type[i] += b.vec_type[i];
    }
    return a;
}

/* Vector subtraction assignment. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>&
operator-=(vector<T, N>& a, const vector<T, N>& b) {
    for (u32 i = 0; i < N; ++i) {
        a.vec_type[i] -= b.vec_type[i];
    }
    return a;
}

/* Vector multiplication assignment. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>&
operator*=(vector<T, N>& a, const vector<T, N>& b) {
    for (u32 i = 0; i < N; ++i) {
        a.vec_type[i] *= b.vec_type[i];
    }
    return a;
}

/* Vector division assignment. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>&
operator/=(vector<T, N>& a, const vector<T, N>& b) {
    for (u32 i = 0; i < N; ++i) {
        a.vec_type[i] /= b.vec_type[i];
    }
    return a;
}

/* Vector scalar division assignment. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>&
operator/=(vector<T, N>& v, const T& s) {
    for (u32 i = 0; i < N; ++i) {
        v.vec_type[i] /= s;
    }
    return v;
}

/*-------------------*/
/* Logical Operators */
/*-------------------*/

/* Vector equality. */
template<typename T, u32 N>
constexpr scl_inline bool
operator==(const vector<T, N>& a, const vector<T, N>& b) {
    for (u32 i = 0; i < N; ++i) {
        if (a.vec_type[i] != b.vec_type[i]) {
            return false;
        }
    }
    return true;
}

/* Vector inequality. */
template<typename T, u32 N>
constexpr scl_inline bool
operator!=(const vector<T, N>& a, const vector<T, N>& b) {
    return !(a == b);
}

/* Vector less than. */
template<typename T, u32 N>
constexpr scl_inline bool
operator<(const vector<T, N>& a, const vector<T, N>& b) {
    for (u32 i = 0; i < N; ++i) {
        if (a.vec_type[i] >= b.vec_type[i]) {
            return false;
        }
    }
    return true;
}

/* Vector greater than. */
template<typename T, u32 N>
constexpr scl_inline bool
operator>(const vector<T, N>& a, const vector<T, N>& b) {
    for (u32 i = 0; i < N; ++i) {
        if (a.vec_type[i] <= b.vec_type[i]) {
            return false;
        }
    }
    return true;
}

/* Vector less than or equal to. */
template<typename T, u32 N>
constexpr scl_inline bool
operator<=(const vector<T, N>& a, const vector<T, N>& b) {
    for (u32 i = 0; i < N; ++i) {
        if (a.vec_type[i] > b.vec_type[i]) {
            return false;
        }
    }
    return true;
}

/* Vector greater than or equal to. */
template<typename T, u32 N>
constexpr scl_inline bool
operator>=(const vector<T, N>& a, const vector<T, N>& b) {
    for (u32 i = 0; i < N; ++i) {
        if (a.vec_type[i] < b.vec_type[i]) {
            return false;
        }
    }
    return true;
}

/*-------------------*/
/* Bitwise Operators */
/*-------------------*/

/* Vector bitwise AND. Must use integral type. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator&(const vector<T, N>& a, const vector<T, N>& b) {
    static_assert(std::is_integral<T>::value, 
        "Vector bitwise AND requires integral type.");
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = a.vec_type[i] & b.vec_type[i];
    }
    return result;
}

/* Vector bitwise OR. Must use integral type. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator|(const vector<T, N>& a, const vector<T, N>& b) {
    static_assert(std::is_integral<T>::value, 
        "Vector bitwise OR requires integral type.");
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = a.vec_type[i] | b.vec_type[i];
    }
    return result;
}

/* Vector bitwise XOR. Must use integral type. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator^(const vector<T, N>& a, const vector<T, N>& b) {
    static_assert(std::is_integral<T>::value, 
        "Vector bitwise XOR requires integral type.");
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = a.vec_type[i] ^ b.vec_type[i];
    }
    return result;
}

/* Vector bitwise AND assignment. Must use integral type. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>&
operator&=(vector<T, N>& a, const vector<T, N>& b) {
    static_assert(std::is_integral<T>::value, 
        "Vector bitwise AND assignment requires integral type.");
    for (u32 i = 0; i < N; ++i) {
        a.vec_type[i] &= b.vec_type[i];
    }
    return a;
}

/* Vector bitwise OR assignment. Must use integral type. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>&
operator|=(vector<T, N>& a, const vector<T, N>& b) {
    static_assert(std::is_integral<T>::value, 
        "Vector bitwise OR assignment requires integral type.");
    for (u32 i = 0; i < N; ++i) {
        a.vec_type[i] |= b.vec_type[i];
    }
    return a;
}

/* Vector bitwise XOR assignment. Must use integral type. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>&
operator^=(vector<T, N>& a, const vector<T, N>& b) {
    static_assert(std::is_integral<T>::value, 
        "Vector bitwise XOR assignment requires integral type.");
    for (u32 i = 0; i < N; ++i) {
        a.vec_type[i] ^= b.vec_type[i];
    }
    return a;
}

/* Vector bitwise NOT. Must use integral type. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator~(const vector<T, N>& a) {
    static_assert(std::is_integral<T>::value, 
        "Vector bitwise NOT requires integral type.");
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = ~a.vec_type[i];
    }
    return result;
}

/*----------------------*/
/* Comparison Operators */
/*----------------------*/

/* Vector logical AND. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator&&(const vector<T, N>& a, const vector<T, N>& b) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = a.vec_type[i] && b.vec_type[i];
    }
    return result;
}

/* Vector logical OR. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator||(const vector<T, N>& a, const vector<T, N>& b) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = a.vec_type[i] || b.vec_type[i];
    }
    return result;
}

/* Vector logical NOT. */
template<typename T, u32 N>
constexpr scl_inline vector<T, N>
operator!(const vector<T, N>& a) {
    vector<T, N> result;
    for (u32 i = 0; i < N; ++i) {
        result.vec_type[i] = !a.vec_type[i];
    }
    return result;
}

/*------------------*/
/* Stream Operators */
/*------------------*/

/* Vector output stream. */
template<typename T, u32 N>
constexpr scl_inline std::ostream&
operator<<(std::ostream& os, const vector<T, N>& v) {
    os << "{";
    for (u32 i = 0; i < N; ++i) {
        os << v.vec_type[i];
        if (i < N - 1) {
            os << ", ";
        }
    }
    os << "}";
    return os;
}

/* Vector input stream. */
template<typename T, u32 N>
constexpr scl_inline std::istream&
operator>>(std::istream& is, vector<T, N>& v) {
    for (u32 i = 0; i < N; ++i) {
        is >> v.vec_type[i];
    }
    return is;
}

/*============================================================================*/
/* Non-Member Mathematical Functions                                          */
/*============================================================================*/

/* Returns the smaller of two vectors. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
min(const vector<T, N>& a, const vector<T, N>& b) {
    return (((a) < (b)) ? (a) : (b));
}

/* Returns the larger of two vectors. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
max(const vector<T, N>& a, const vector<T, N>& b) {
    return (((a) > (b)) ? (a) : (b));
}

/* Absolute value. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
abs(const vector<T, N>& v) {
    return v.abs();
}

/* Square root. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
sqrt(const vector<T, N>& v) {
    return v.sqrt();
}

/* Cube root. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
cbrt(const vector<T, N>& v) {
    return v.cbrt();
}

/* Reciprocal square root. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
rsqrt(const vector<T, N>& v) {
    return v.rsqrt();
}

/* Reciprocal. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
recip(const vector<T, N>& v) {
    return v.recip();
}

/* Exponential. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
exp(const vector<T, N>& v) {
    return v.exp();
}

/* Base 2 exponential. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
exp2(const vector<T, N>& v) {
    return v.exp2();
}

/* Base 10 exponential. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
exp10(const vector<T, N>& v) {
    return v.exp10();
}

/* Natural logarithm. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
log(const vector<T, N>& v) {
    return v.log();
}

/* Base 2 logarithm. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
log2(const vector<T, N>& v) {
    return v.log2();
}

/* Base 10 logarithm. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
log10(const vector<T, N>& v) {
    return v.log10();
}

/* Logarithm to base e. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
logb(const vector<T, N>& v) {
    return v.logb();
}

/* Power. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
pow(const vector<T, N>& v, const vector<T, N>& e) {
    return v.pow(e);
}

/* Sine. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
sin(const vector<T, N>& v) {
    return v.sin();
}

/* Cosine. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
cos(const vector<T, N>& v) {
    return v.cos();
}

/* Tangent. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
tan(const vector<T, N>& v) {
    return v.tan();
}

/* Arcsine. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
asin(const vector<T, N>& v) {
    return v.asin();
}

/* Arccosine. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
acos(const vector<T, N>& v) {
    return v.acos();
}

/* Arctangent. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
atan(const vector<T, N>& v) {
    return v.atan();
}

/* Hyperbolic sine. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
sinh(const vector<T, N>& v) {
    return v.sinh();
}

/* Hyperbolic cosine. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
cosh(const vector<T, N>& v) {
    return v.cosh();
}

/* Hyperbolic tangent. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
tanh(const vector<T, N>& v) {
    return v.tanh();
}

/* Hyperbolic arcsine. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
asinh(const vector<T, N>& v) {
    return v.asinh();
}

/* Hyperbolic arccosine. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
acosh(const vector<T, N>& v) {
    return v.acosh();
}

/* Hyperbolic arctangent. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
atanh(const vector<T, N>& v) {
    return v.atanh();
}

/* Hypotenuse. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
hypot(const vector<T, N>& v, const vector<T, N>& e) {
    return v.hypot(e);
}

/* Floor. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
floor(const vector<T, N>& v) {
    return v.floor();
}

/* Ceiling. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
ceil(const vector<T, N>& v) {
    return v.ceil();
}

/* Truncate. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
trunc(const vector<T, N>& v) {
    return v.trunc();
}

/* Round. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
round(const vector<T, N>& v) {
    return v.round();
}

/* Fractional part. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
fract(const vector<T, N>& v) {
    return v.fract();
}

/* Modulus. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
mod(const vector<T, N>& v, const vector<T, N>& m) {
    return v.mod(m);
}

/* Sign. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
sign(const vector<T, N>& v) {
    return v.sign();
}

/*============================================================================*/
/* Common Vector Functions                                                    */
/*============================================================================*/

/* Dot product. */
template<typename T, u32 N>
constexpr static inline T 
dot_product(const vector<T, N>& a, const vector<T, N>& b) {
    T result = 0;
    for (u32 i = 0; i < N; ++i) {
        result += a[i] * b[i];
    }
    return result;
}

/* Cross product 2D. */
template<typename T>
constexpr static inline vector<T, 2> 
cross_product(const vector<T, 2>& a, const vector<T, 2>& b) {
    return a[0] * b[1] - a[1] * b[0];
}

/* Cross product 3D. */
template<typename T>
constexpr static inline vector<T, 3> 
cross_product(const vector<T, 3>& a, const vector<T, 3>& b) {
    return vector<T, 3>(
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    );
}

/* Cross product 4D. */
template<typename T>
constexpr static inline vector<T, 4> 
cross_product(const vector<T, 4>& a, const vector<T, 4>& b) {
    return vector<T, 4>(
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
        a[3] * b[3] - a[3] * b[3]
    );
}

/* Length. */
template<typename T, u32 N>
constexpr static inline T 
length(const vector<T, N>& a) {
    return std::sqrt(dot_product(a, a));
}

/* Length squared. */
template<typename T, u32 N>
constexpr static inline T
length_squared(const vector<T, N>& a) {
    return dot_product(a, a);
}

/* Normalize. */
template<typename T, u32 N>
constexpr static inline vector<T, N> 
normalize(const vector<T, N>& a) {
    return a / length(a);
}

/* Reflect. */
template<typename T, u32 N>
constexpr static inline vector<T, N> 
reflect(const vector<T, N>& a, const vector<T, N>& b) {
    return a - 2 * dot_product(a, b) * b;
}

/* Refract. */
template<typename T, u32 N>
constexpr static inline vector<T, N> 
refract(const vector<T, N>& a, const vector<T, N>& b, const T& eta) {
    T k = 1 - eta * eta * (1 - dot_product(a, b) * dot_product(a, b));
    if (k < 0) {
        return vector<T, N>(0);
    } else {
        return eta * a - (eta * dot_product(a, b) + std::sqrt(k)) * b;
    }
}

/* Distance. */
template<typename T, u32 N>
constexpr static inline T 
distance(const vector<T, N>& a, const vector<T, N>& b) {
    return length(a - b);
}

/* Face forward. */
template<typename T, u32 N>
constexpr static inline vector<T, N> 
face_forward(const vector<T, N>& a, const vector<T, N>& b) {
    return dot_product(a, b) < 0 ? a : -a;
}

/* Linear Interpolation. */
template<typename T, u32 N>
constexpr static inline vector<T, N> 
lerp(const vector<T, N>& a, const vector<T, N>& b, const T& t) {
    return a + t * (b - a);
}

/* Sperical interpolation. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
slerp(const vector<T, N>& a, const vector<T, N>& b, const T& t) {
    T theta = std::acos(dot_product(a, b));
    return (std::sin((1 - t) * theta) * a + 
            std::sin(t * theta) * b) / std::sin(theta);
}

/* Cosine interpolation. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
cosine_interpolation(const vector<T, N>& a, 
                     const vector<T, N>& b, const T& t) {
    T t2 = (1 - std::cos(t * PI<T>)) * 0.5;
    return a * (1 - t2) + b * t2;
}

/* Cubic interpolation. */
template<typename T, u32 N>
constexpr static inline vector<T, N>
cubic_interpolation(const vector<T, N>& a, const vector<T, N>& b, 
      const vector<T, N>& c, const vector<T, N>& d, T t) {
    T t2 = t * t;
    T t3 = t2 * t;
    return (2 * t3 - 3 * t2 + 1) * a + 
           (t3 - 2 * t2 + t) * c + 
           (-2 * t3 + 3 * t2) * b + 
           (t3 - t2) * d;
}

/* Smoothstep. */
template<typename T, u32 N>
constexpr static inline vector<T, N> 
smoothstep(const vector<T, N>& a, const vector<T, N>& b, const T& t) {
    return lerp(a, b, t * t * (3 - 2 * t));
}

/* Hermite interpolation. */
template<typename T, u32 N>
constexpr static inline vector<T, N> 
hermite(const vector<T, N>& a, const vector<T, N>& b, 
        const vector<T, N>& c, const vector<T, N>& d, const T& t) {
    T t2 = t   * t;
    T t3 = t2  * t;
    return (2  * t3 - 3 * t2 + 1) * a + 
                (t3 - 2 * t2 + t) * c + 
           (-2 * t3 + 3 * t2) * b + (t3 - t2) * d;
}

/* Barycentric interpolation. */
template<typename T, u32 N>
constexpr static inline vector<T, N> 
barycentric(const vector<T, N>& a, const vector<T, N>& b, 
             const vector<T, N>& c, const T& u, const T& v) {
    return a + u * (b - a) + v * (c - a);
}

/* Catmull-Rom interpolation. */
template<typename T, u32 N>
constexpr static inline vector<T, N> 
catmullrom(const vector<T, N>& a, const vector<T, N>& b, 
           const vector<T, N>& c, const vector<T, N>& d, const T& t) {
    return 0.5 * (2 * b + (c - a) * t + 
                 (2 * a - 5 * b + 4 * c - d) * t * t + 
                 (3 * b - a - 3 * c + d) * t * t * t);
}

/* Clamp. */
template<typename T, u32 N>
constexpr static inline vector<T, N> 
clamp(const vector<T, N>& a, 
      const vector<T, N>& b, 
      const vector<T, N>& c) {
    return std::min(std::max(a, b), c);
}

/* Mix. */
template<typename T, u32 N>
constexpr static inline vector<T, N> 
mix(const vector<T, N>& a, const vector<T, N>& b, const T& t) {
    return a * (1 - t) + b * t;
}

/* Step. */
template<typename T, u32 N>
constexpr static inline
vector<T, N> step(const vector<T, N>& a, const vector<T, N>& b) {
    return vector<T, N>(a < b);
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

} // namespace scl
