#include "utils.h"

using namespace Util;

/**
 * Puts the executing thread to sleep so that other cpu work can be done.
 * @param Amount of time thread should sleep for.
 */
void Util::sleep(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

/**
 * Writes directly to a file.
 * @param fileName the name of the file.
 * @param data The bytes to write.
 * @return True if successful, false otherwise.
 */
bool Util::writeFile(const char* fileName, const std::vector<char>& data)
{
    std::ofstream file(fileName, std::ofstream::binary);
    file.write(data.data(), int(data.size()));
    file.close();

    return true;
}

/**
 * Reads a file and returns an array of bytes.
 * @param fileName the file to read.
 * @return The file's contents as an array of bytes.
 */
 std::vector<char> Util::readFile(const char* fileName)
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
}
