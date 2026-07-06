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

BluetoothManager::BluetoothManager(std::string configurationFileName) {
    this->parseConfigurationFile(configurationFileName);
    this->findPairedDeviceAddress(std::wstring(this->deviceName.begin(), this->deviceName.end()).c_str(), this->deviceAddress);
    
    this->localServer.WebSocket("/"+this->socketEndpoint, [](const httplib::Request& req, httplib::ws::WebSocket& ws) {
        std::string msg;
        while (ws.read(msg)) {
            std::cout << msg << std::endl;
            
        }
    });
}

void BluetoothManager::startServer() {
    std::cout << this->localServer.listen(this->serverAddress, this->port) << std::endl;
}