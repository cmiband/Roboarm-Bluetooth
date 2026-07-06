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

	httplib::Server localServer;
	std::string serverAddress;
	std::string socketEndpoint;
	int port;

	bool findPairedDeviceAddress(const wchar_t* nameContains, BLUETOOTH_ADDRESS& outAddr);
	void parseConfigurationFile(std::string fileName);
public:
	BluetoothManager(std::string configurationFileName);
	void startServer();
};

