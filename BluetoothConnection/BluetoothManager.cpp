#include "BluetoothManager.h"
#include <fstream>
#include <string>

#define EQUALS_CHARACTER "="
#define SERVER_ADDRESS_PROPERTY "socketAddress"
#define SOCKET_ENDPOINT_PROPERTY "socketEndpoint"
#define DEVICE_NAME_PROPERTY "deviceName"
#define PORT_PROPERTY "port"

bool BluetoothManager::findPairedDeviceAddress(const wchar_t* nameContains, BLUETOOTH_ADDRESS& outAddr) {
    BLUETOOTH_DEVICE_SEARCH_PARAMS searchParams = { 0 };
    searchParams.dwSize = sizeof(searchParams);
    searchParams.fReturnAuthenticated = TRUE;
    searchParams.fReturnRemembered = TRUE;
    searchParams.fReturnConnected = TRUE;
    searchParams.fReturnUnknown = FALSE;
    searchParams.fIssueInquiry = FALSE;
    searchParams.cTimeoutMultiplier = 0;

    BLUETOOTH_DEVICE_INFO deviceInfo = { 0 };
    deviceInfo.dwSize = sizeof(deviceInfo);

    HBLUETOOTH_DEVICE_FIND hFind = BluetoothFindFirstDevice(&searchParams, &deviceInfo);
    if (!hFind) return false;

    bool found = false;
    do {
        std::wcout << L"Znaleziono: " << deviceInfo.szName << std::endl;
        if (wcsstr(deviceInfo.szName, nameContains) != nullptr) {
            outAddr = deviceInfo.Address;
            found = true;
            break;
        }
    } while (BluetoothFindNextDevice(hFind, &deviceInfo));

    BluetoothFindDeviceClose(hFind);
    return found;
}

void BluetoothManager::parseConfigurationFile(std::string fileName) {
    std::ifstream file;
    file.open(fileName);
    
    if (!file.good()) {
        throw std::exception("Configuration file parsing error");
    }

    std::string fileLine;
    while (std::getline(file, fileLine)) {
        if (fileLine.length() == 0) {
            continue;
        }

        const size_t equalsCharacterPlacement = fileLine.find(EQUALS_CHARACTER);
        const std::string property = fileLine.substr(0, equalsCharacterPlacement);
        const std::string value = fileLine.substr(equalsCharacterPlacement + 1);
        std::cout << property << " " << value << std::endl;

        if (property == SERVER_ADDRESS_PROPERTY) {
            this->serverAddress = value;
        }
        else if (property == SOCKET_ENDPOINT_PROPERTY) {
            this->socketEndpoint = value;
        }
        else if (property == DEVICE_NAME_PROPERTY) {
            this->deviceName = value;
        }
        else if (property == PORT_PROPERTY) {
            this->port = std::stoi(value);
        }
    }

    file.close();
}

bool BluetoothManager::pairDevice() {
    this->bluetoothSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    if (this->bluetoothSocket == INVALID_SOCKET) {
        return false;
    }

    SOCKADDR_BTH addr = { 0 };
    addr.addressFamily = AF_BTH;
    addr.btAddr = this->deviceAddress.ullLong;
    addr.serviceClassId = SerialPortServiceClass_UUID;
    addr.port = 1;

    if (connect(this->bluetoothSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "connect() failed: " << WSAGetLastError() << std::endl;
        closesocket(this->bluetoothSocket);
        return false;
    }

    return true;
}

bool BluetoothManager::sendDataToMicrocontroller(std::string data) {
    const char* dataToSend = data.c_str();
    int sent = send(this->bluetoothSocket, dataToSend, static_cast<int>(data.size()), 0);
    if (sent == SOCKET_ERROR) {
        std::cerr << "send() failed: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "Wyslano " << sent << " bajtow\n";
    }
    std::cout << "3" << std::endl;
    return true;
}

BluetoothManager::BluetoothManager(std::string configurationFileName) {
    this->parseConfigurationFile(configurationFileName);
    bool deviceFound = this->findPairedDeviceAddress(std::wstring(this->deviceName.begin(), this->deviceName.end()).c_str(), this->deviceAddress);
    if (!deviceFound) {
        std::cout << "Unable to find desired device";
        return;
    }
    bool devicePaired = this->pairDevice();
    if (!devicePaired) {
        std::cout << "Unalbe to pair device" << std::endl;
        return;
    }
    
    this->localServer.WebSocket("/"+this->socketEndpoint, [this](const httplib::Request& req, httplib::ws::WebSocket& ws) {
        std::string msg;
        while (ws.read(msg)) {
            std::cout << msg << std::endl;
            std::cout << "Sending to STM..." << std::endl;
            this->sendDataToMicrocontroller(msg);
        }
    });

    this->configurationSuccessful = true;
}

BluetoothManager::~BluetoothManager() {
    closesocket(this->bluetoothSocket);
}

void BluetoothManager::startServer() {
    if (!this->configurationSuccessful) {
        return;
    }

    std::cout << this->localServer.listen(this->serverAddress, this->port) << std::endl;
}