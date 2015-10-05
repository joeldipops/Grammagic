#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../core.h"

namespace Util
{
    /**
    * Puts the executing thread to sleep so that other cpu work can be done.
    * @param Amount of time thread should sleep for.
    */
    inline void sleep(int milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    };

    // TODO Learn to do operator overloading to make this prettier
    template<typename N>
    struct Nullable
    {
        public:

            const N get(void) const
            {
                if (!_isNull)
                    return _value;

                throw;
            };

            void set(void)
            {
                _isNull = true;
            };

            N set(N value_)
            {
                _value = value_;
                _isNull = false;
                return _value;
            };

            const bool isNull(void) const
            {
                return _isNull;
            };

        private:
            N _value;
            bool _isNull = true;
    };

    struct Location
    {
        public:
            Location(int* x = nullptr, int* y = nullptr)
            {
                X = (x != nullptr) ? *x : 0;
                Y = (y != nullptr) ? *y : 0;
            };
            Location(int x, int y)
            {
                X = x;
                Y = y;
            };
            int X;
            int Y;
    };

    template <typename T>
    std::vector<T*> toPointers(std::vector<T>& source)
    {
        std::vector<T*> result(source.size());
        std::transform(source.begin(), source.end(), result.begin(), [](T& t) { return &t; });
        return result;
    };

    template <typename T>
    std::vector<const T> toConst(const std::vector<T> source)
    {
        std::vector<const T> result(source.size());
        std::transform(source.begin(), source.end(), result.begin(), [](T& t) { const T r = t; return r; });
        return result;
    }

    template <typename T>
    std::vector<const T*> toPointers(const std::vector<T>& source)
    {
        std::vector<const T*> result = std::vector<const T*>(0);
        result.reserve(source.size());
        for (natural i = 0; i < source.size(); i++)
        {
            result.push_back(&source.at(i));
        }

        return result;
    };

    template <typename I, typename O>
    std::vector<O> castVector(std::vector<I>& source)
    {
        std::vector<O> result(source.size());
        std::transform(source.begin(), source.end(), result.begin(), [](I i) { return (O) i; });

        return result;
    }

    template <typename I, typename O>
    std::vector<O> castVector(const std::vector<I>& source)
    {
        std::vector<O> result(source.size());
        std::transform(source.begin(), source.end(), result.begin(), [](I i) { return (O) i; });

        return result;
    }

    bool writeFile(const std::string& fileName, const std::vector<byte>& data);

    std::vector<byte> readFile(const std::string& fileName);

    std::vector<std::string> split(const std::string &s, char token);

    std::vector<byte> splitShort(unsigned short);

    unsigned short fuseShort(byte high, byte low);

}

#endif
