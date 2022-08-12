#pragma once

#include <fstream>
#include <windows.h>


using namespace std;

fstream file;

char fileName[255];
char sign;
int toRead = 1;
unsigned long readSize = sizeof(sign);
int pkgNr = 1;
char block[128];
bool wasACK;
char sumCRC[2];
int binary[16];

HANDLE hPort; 
DCB dcb;              
COMMTIMEOUTS timeSettings;
USHORT tmpCRC;

const char SOH = (char)1;
const char NAK = (char)15;
const char CAN = (char)18;
const char ACK = (char)6;
const char EOT = (char)4;