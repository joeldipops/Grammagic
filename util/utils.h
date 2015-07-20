#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <fstream>

namespace Util
{
    struct Util
    {
        /**
         * Puts the executing thread to sleep so that other cpu work can be done.
         * @param Amount of time thread should sleep for.
         */
        static void sleep(int milliseconds)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        };

        /**
         * Writes directly to a file.
         * @param fileName the name of the file.
         * @param data The bytes to write.
         * @return True if successful, false otherwise.
         */
        static bool writeFile(const char* fileName, const std::vector<char>& data)
        {
            std::ofstream file(fileName, std::ofstream::binary);
            file.write(&data[0], int(data.size()));
            file.close();

            return true;
        }

        /**
         * Reads a file and returns an array of bytes.
         * @param fileName the file to read.
         * @return The file's contents as an array of bytes.
         */
        static std::vector<char> readFile(const char* fileName)
        {
            std::vector<char> result ;
            std::streampos size;
            std::ifstream file(fileName, std::ios::in|std::ios::binary|std::ios::ate);

            if (!file.is_open())
                return std::vector<char>();

            size = file.tellg();

            char* data = new char[size];
            file.seekg(0, std::ios::beg);
            file.read(data, size);
            file.close();

            result.assign(data, data+size);
            return result;
        };
    };
}

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
static std::vector<T*> toPointers(std::vector<T>& source)
{
    std::vector<T*> target(source.size());
    std::transform(source.begin(), source.end(), target.begin(), [](T& t) { return &t; });
    return target;
};
#endif
