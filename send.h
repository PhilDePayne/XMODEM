#pragma once

#include "const.h"
#include "crc.h"
#include <iostream>
#include <fstream>

using namespace std;

int crc;



void sendHeader() {

    WriteFile(hPort, &SOH, toRead, &readSize, NULL);
    sign = (char)pkgNr;
    WriteFile(hPort, &sign, toRead, &readSize, NULL);
    sign = (char)255 - pkgNr;
    WriteFile(hPort, &sign, toRead, &readSize, NULL);

}

void sendBlock() {

    for (int i = 0; i < 128; i++) {

        WriteFile(hPort, &block[i], toRead, &readSize, NULL);

    }

}


void sendCheck(char tab[]) {

    if (crc == 2) {

        char sum = 0;
        for (int i = 0; i < 128; i++)
            sum += block[i] % 256;

        cout << (int)sum << endl;
        WriteFile(hPort, &sum, toRead, &readSize, NULL);

    }

    else {


        calcCrc(tab);

        char toSend = signCrc(binary, 1);
        WriteFile(hPort, &toSend, toRead, &readSize, NULL);

        toSend = signCrc(binary, 2);
        WriteFile(hPort, &toSend, toRead, &readSize, NULL);
    }
}


int checkAns() {

    int ret = 0;

    while (1) {
        char ans = ' ';
        ReadFile(hPort, &ans, toRead, &readSize, NULL);

        cout << ans << '\n';

        switch (ans) {

        case ACK:
            cout << "Packet sent succesfully\n";
            ret = 1;
            break;

        case NAK:
            cout << "Error sending packet\n";
            ret = 2;
            break;

        case CAN:
            cout << "Connection stopped\n";
            ret = 3;
            break;

        }

        if (ret != 0) break;
    }

    return ret;

}

void sendFile() {
    cout << "Send file: ";
    cin >> fileName;

    for (int i = 0; i < 6; i++) {
        
        ReadFile(hPort, &sign, toRead, &readSize, NULL);

        if (sign == 'C') {
            cout << "Received 'C'\n";
            crc = 1;
            break;
        }
        else if (sign == NAK) {
            cout << "Received NAK\n";
            crc = 2;
            break;
        }
    }

    file.open(fileName, ios::binary | ios::in);
    while (!file.eof())
    {

        for (int i = 0; i < 128; i++)
            block[i] = (char)26;

        int w = 0;


        while (w < 128 && !file.eof())
        {
            block[w] = file.get();
            if (file.eof()) block[w] = (char)26;
            w++;
        }
        wasACK = false;

        do
        {
            sendHeader();

            sendBlock();

            sendCheck(block);

            int ans = checkAns();

            switch (ans) {

            case 1:
                wasACK = true;
                break;

            case 2:
                break;

            case 3:
                exit(1);

            }
        } while (!wasACK);

        pkgNr == 255 ? pkgNr = 1 : pkgNr++;

    }
    file.close();

    while (1)
    {
        sign = EOT;
        WriteFile(hPort, &sign, toRead, &readSize, NULL);
        ReadFile(hPort, &sign, toRead, &readSize, NULL);
        if (sign == ACK) break;
    }

    CloseHandle(hPort);
    cout << "Sent succesfully";
}