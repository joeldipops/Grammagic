#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

namespace Util {
    /*
    static void sleep(int milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }*/
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

struct Colour
{
    int R;
    int G;
    int B;
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
        }
        int X;
        int Y;
};
#endif
