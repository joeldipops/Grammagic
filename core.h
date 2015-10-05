#ifndef CORE_INCLUDED
#define CORE_INCLUDED

typedef unsigned char byte;
typedef unsigned int natural;

template<typename T>
bool in(T value)
{
    return false;
}

template<typename T, typename... t>
bool in(T value, T candidate, t... rest)
{
    if (value == candidate)
        return true;

    return in(value, rest...);
};

template<typename T>
void deletePtr(T*& pointer)
{
    delete pointer;
    pointer = nullptr;
};
#endif
