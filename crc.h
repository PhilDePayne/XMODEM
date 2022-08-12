#pragma once
#include "const.h"
#include "crc.h"
#include "windows.h"
#include <iostream>
#include <fstream>

using namespace std;

int toDec(int base, int val)
{
    if (val == 0) return 1;
    if (val == 1) return base;

    int result = base;

    for (int i = 2; i <= val; i++)
        result = result * base;

    return result;
}

char signCrc(int tab[], int half) {

    int x = 0;

    for (int i = 0; i < 8; i++)
        x = x + toDec(2, i) * tab[(half * 8) - 1 - i];

    return (char)x;

}

void calcCrc(char* tab) {

    int sum = 0;
    int count = 128;
    while (--count >= 0)
    {
        sum = sum ^ (int)*tab++ << 8;
        for (int i = 0; i < 8; ++i)
            if (sum & 0x8000) sum = sum << 1 ^ 0x1021;
            else sum = sum << 1;
    }

    sum = sum & 0xFFFF;

    int x;

    for (int z = 0; z < 16; z++) binary[z] = 0;

    for (int i = 0; i < 16; i++)
    {
        x = sum % 2;
        if (x == 1) sum = (sum - 1) / 2;
        if (x == 0) sum = sum / 2;
        binary[15 - i] = x;
    }

}