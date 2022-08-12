#pragma once

#include "const.h"
#include "crc.h"
#include "windows.h"
#include <iostream>
#include <fstream>

using namespace std;

char comp255;

void checkCRC(int mode, char tab[], char checkTab[]) {

    if (mode == 1) {

        calcCrc(tab);

        char toCheck = signCrc(binary, 1);
        if (toCheck != checkTab[0]) {
            WriteFile(hPort, &NAK, toRead, &readSize, NULL);
            wasACK = false;
        }

        toCheck = signCrc(binary, 2);
        if (toCheck != checkTab[1]) {
            WriteFile(hPort, &NAK, toRead, &readSize, NULL);
            wasACK = false;
        }

    }

    else {

        char sum = 0;

        for (int i = 0; i < 128; i++) sum += block[i] % 256;

        cout << "sum " << (int)sum << endl;

        if (sum != checkTab[0]) {

            WriteFile(hPort, &NAK, toRead, &readSize, NULL);
            wasACK = false;

        }

    }


}

void readBlock(int mode) {

    ReadFile(hPort, &sign, toRead, &readSize, NULL);
    pkgNr = (int)sign;

    ReadFile(hPort, &sign, toRead, &readSize, NULL);
    comp255 = sign;

    for (int i = 0; i < 128; i++) {
        ReadFile(hPort, &sign, toRead, &readSize, NULL);
        block[i] = sign;
    }

    if (mode == 1) {

        ReadFile(hPort, &sign, toRead, &readSize, NULL);
        sumCRC[0] = sign;

        ReadFile(hPort, &sign, toRead, &readSize, NULL);
        sumCRC[1] = sign;

    }
    else {

        cout << "NAK ";
        ReadFile(hPort, &sign, toRead, &readSize, NULL);
        sumCRC[0] = sign;
        cout << (int)sign << endl;

    }
    
    wasACK = true;

    if ((char)(255 - pkgNr) != comp255) {

        cout << "Wrong packet number.\n";

        WriteFile(hPort, &NAK, toRead, &readSize, NULL);

        wasACK = false;

    }
    else {

        checkCRC(mode, block, sumCRC);
        
    }

    if (wasACK) {

        for (int i = 0; i < 128; i++) {

            file << block[i];

        }

        WriteFile(hPort, &ACK, toRead, &readSize, NULL);

    }

}

void receiveFile() {

    int mode;

    cin.ignore();

    cout << "Choose sign to send\n[1] C\n[2] NAK\n";

    switch (getchar()) {

    case '1': {

        sign = 'C';
        mode = 1;
        break;

    }

    case '2': {

        sign = NAK;
        mode = 2;
        break;

    }

    default: {

        cout << "Default choice: C.\n";
        sign = 'C';

    }
    }



    for (int i = 0; i < 1000; i++) {
        
        WriteFile(hPort, &sign, toRead, &readSize, NULL);

        cout << "Waiting for SOH...\n";

        ReadFile(hPort, &sign, toRead, &readSize, NULL);

        cout << (int)sign << endl;

        if (sign == SOH) {

            cout << "Connected!\n";
            break;

        }
    }


    cout << "Received file name: ";

    cin >> fileName;

    file.open(fileName, ios::binary | ios::out);

    readBlock(mode);

    while (1) {

        ReadFile(hPort, &sign, toRead, &readSize, NULL);
        if (sign == EOT || sign == CAN) break;

        readBlock(mode);

    }
    WriteFile(hPort, &ACK, toRead, &readSize, NULL);

    file.close();
    CloseHandle(hPort);
}