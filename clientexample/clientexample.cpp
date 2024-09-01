#include <iostream>
#include <memory>
#include <vector>

#include "Client.h"


std::vector<char> get_vec(std::string info)
{
	return std::vector<char>(info.begin(), info.end());
}

int main(int argc, char* argv[])
{
	std::string ip_address = "127.0.0.1";
	unsigned short port = 8080;
	if (argc != 3) {
		std::cout << "using default config IP : 127.0.0.1 and Port 8080\n";
	}
	else
	{
		ip_address = argv[1];
		port = std::stoul(argv[2]);
	}

	std::shared_ptr<Client> client = std::make_shared<Client>(ip_address, port);

	client->connect_on_data_received([](const std::vector<char>& data, std::size_t length) {
		std::cout << "Custom handler: Received data from Server : " << std::string(data.begin(), data.end()) << "\n";
		});

	bool key[2] = { false, false };
	bool old_key[2] = { false, false };
	bool quit = false;

	while (!quit)
	{

		key[0] = GetAsyncKeyState('2') & 0x8000;
		key[1] = GetAsyncKeyState(VK_ESCAPE) & 0x8000;

		if (key[0] && !old_key[0]) client->send(get_vec("testing server\n"));
		if (key[1] && !old_key[1]) quit = true;

		for (int i = 0; i < 2; i++) old_key[i] = key[i];
	}

	return 0;
}
