#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Bthprops.lib")

#include <winsock2.h>
#include <ws2bth.h>
#include <bluetoothapis.h>
#include <iostream>
#include <cstdint>
#include "httplib.h"

class BluetoothManager
{
private:
	BLUETOOTH_ADDRESS deviceAddress;
	std::string deviceName;
	SOCKET bluetoothSocket;

	httplib::Server localServer;
	std::string serverAddress;
	std::string socketEndpoint;
	int port;

	bool configurationSuccessful = false;

	bool findPairedDeviceAddress(const wchar_t* nameContains, BLUETOOTH_ADDRESS& outAddr);
	bool pairDevice();
	void parseConfigurationFile(std::string fileName);
	bool sendDataToMicrocontroller(std::string data);
public:
	BluetoothManager(std::string configurationFileName);
	~BluetoothManager();
	void startServer();
};

