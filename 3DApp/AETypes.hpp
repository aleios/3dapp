#pragma once
#ifndef AETYPES_HPP
#define AETYPES_HPP

#include <limits>

#define AE_PI 3.14159f
#define AE_E 2.71828f

typedef signed char int8;
typedef short int int16;
typedef int int32;

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;

typedef float float32;
typedef double float64;

#define AE_INF std::numeric_limits<float>::infinity();

// Deg - Rad Converters
inline static const float Deg2Rad(float degrees)
{
    return degrees * AE_PI / 180.0f;
}

inline static const float Rad2Deg(float rads)
{
    return rads * 180.0f / AE_PI;
}

#endif