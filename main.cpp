#include <iostream>
#include <bitset>

#include "hamming_codec.h"

int main()
{
    typedef uint16_t word;

    word source = 0b00000001111;

    word encoded = hamming::hamming_encode(source);
    std::cout << std::bitset<15>(encoded) << std::endl;

    hamming::make_error(encoded, 10);
    std::cout << std::bitset<15>(encoded) << std::endl;

    hamming::hamming_decode(encoded);
    std::cout << std::bitset<15>(encoded) << std::endl;
}
