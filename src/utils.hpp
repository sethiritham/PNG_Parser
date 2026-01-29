#ifndef UTILS_H
#define UTILS_H

inline int clamp_value_255(int value)
{
    return ((value>255)?255:((value<0) ? 0 : value));
}

#endif