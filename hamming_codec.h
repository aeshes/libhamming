#ifndef HAMMING_CODEC_H
#define HAMMING_CODEC_H

#include <cstdint>
#include <iostream>
#include <map>

namespace hamming
{
    typedef uint8_t u8;
    typedef uint16_t u16;

    enum error_type
    {
        NO_ERROR,
        SINGLE_BIT_ERROR,
        DOUBLE_BIT_ERROR
    };

    inline u8 sum_bits(u16 n)
    {
        uint8_t sum = 0;
        while (n > 0)
        {
            if (n & 1)
                sum ^= 1;
            n >>= 1;
        }
        return sum;
    }

    void make_error(u16& n, std::size_t position)
    {
        n ^= 1 << position;
    }

    u16 hamming_encode(u16 i)
    {
        const std::size_t matrix_cols = 15;
        u16 result = 0;

        u16 g[matrix_cols] = { 0b10000000000,
                               0b01000000000,
                               0b00100000000,
                               0b00010000000,
                               0b00001000000,
                               0b00000100000,
                               0b00000010000,
                               0b00000001000,
                               0b00000000100,
                               0b00000000010,
                               0b00000000001,
                               0b11110101100,
                               0b01111010110,
                               0b00111101011,
                               0b11101011001 };

        for (std::size_t col = 0; col < matrix_cols; ++col)
        {
            result <<= 1;
            result |= sum_bits(i & g[col]); // Умножение двоичных векторов и сумма бит по модулю 2
        }

        return result;
    }

    u8 syndrome(u16 codeword)
    {
        u8 syndrome = 0;

        const u16 h[4] = { 0b111101011001000,
                           0b011110101100100,
                           0b001111010110010,
                           0b111010110010001 };

        for (std::size_t row = 0; row < 4; ++row)
        {
            syndrome <<= 1;
            syndrome |= sum_bits(codeword & h[row]);
        }

        return syndrome;
    }

    error_type hamming_error_correct(u16 & i)
    {
        const u8 syndrome_to_bit[16] = {UINT8_MAX, 0, 1, 4, 2, 8, 5, 10, 8, 14, 9, 7,  6, 13, 11, 12  };

        u8 s = syndrome(i);

        if (s == 0)
        {
            return NO_ERROR;
        }

        i ^= 1 << syndrome_to_bit[s];

        return SINGLE_BIT_ERROR;
    }

    error_type hamming_decode(u16 & i)
    {
        error_type error = hamming_error_correct(i);

        i = i >> 4 & 0x7FF;

        return error;
    }
}

#endif // HAMMING_CODEC_H
