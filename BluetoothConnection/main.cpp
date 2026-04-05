#include <iostream>
#include "httplib.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT

int main() {	
	httplib::Server server;

    server.Get("/handshake", [](const httplib::Request&, httplib::Response& res) {
        res.status = 200;
    });

    server.WebSocket("/ws", [](const httplib::Request& req, httplib::ws::WebSocket& ws) {
        std::string msg;
        while (ws.read(msg)) {
            std::cout << msg << std::endl;
        }
    });

	server.listen("0.0.0.0", 8080);

	return 0;
}