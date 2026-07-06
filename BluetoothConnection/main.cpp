#include <iostream>
#include "httplib.h"
#include "BluetoothManager.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_WEBSOCKET_SUPPORT

int main() {
    BluetoothManager bluetoothManager("config.txt");
    bluetoothManager.startServer();
    /*
    BluetoothManager manager;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    BLUETOOTH_ADDRESS btAddr;
    std::wcout << "Address bluetooth: " << btAddr.rgBytes << " " << btAddr.ullLong << std::endl;
    //if (!manager.findPairedDeviceAddress(L"LOGI", btAddr)) {
    //    std::cerr << "Nie znaleziono sparowanego urzadzenia LOGI\n";
    //    WSACleanup();
    //    return 1;
    //}

    /*
    SOCKET s = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    if (s == INVALID_SOCKET) {
        std::cerr << "socket() failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    SOCKADDR_BTH addr = { 0 };
    addr.addressFamily = AF_BTH;
    addr.btAddr = btAddr.ullLong;
    addr.serviceClassId = SerialPortServiceClass_UUID; // SDP sam znajdzie kanal SPP
    addr.port = 0;

    if (connect(s, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "connect() failed: " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    std::cout << "Polaczono!\n";

    uint8_t data[7] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    int sent = send(s, reinterpret_cast<char*>(data), sizeof(data), 0);
    if (sent == SOCKET_ERROR) {
        std::cerr << "send() failed: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "Wyslano " << sent << " bajtow\n";
    }

    closesocket(s);
    WSACleanup();*/

	return 0;
}