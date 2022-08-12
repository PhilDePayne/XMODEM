#include <fstream>
#include <string.h>
#include <windows.h>
#include <atlstr.h>
#include "iostream"
#include "const.h"
#include "send.h"
#include "receive.h"

using namespace std;

void openPort(int portNr) {

    CString port;

    if (portNr == 2) port = "COM2";
    else port = "COM3";

    hPort = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hPort != INVALID_HANDLE_VALUE)
    {
        dcb.DCBlength = sizeof(dcb);
        GetCommState(hPort, &dcb);
        dcb.BaudRate = CBR_9600;
        dcb.Parity = NOPARITY;
        dcb.StopBits = ONESTOPBIT;
        dcb.ByteSize = 8;

        timeSettings.ReadIntervalTimeout = 10000;
        timeSettings.ReadTotalTimeoutMultiplier = 10000;
        timeSettings.ReadTotalTimeoutConstant = 10000;
        timeSettings.WriteTotalTimeoutMultiplier = 100;
        timeSettings.WriteTotalTimeoutConstant = 100;
    }

}

int main()
{
    cout << "What do you want to do:\n[1] Send file\n[2] Receive file\n";

    switch (getchar()) {
    case '1':

        openPort(3);
        sendFile();
        break;

    case '2':

        openPort(2);
        receiveFile();
        break;

    }
 
    return 0;

}
