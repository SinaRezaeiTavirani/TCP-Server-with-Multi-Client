#include <memory>
#include <vector>

#include "Server.h"


std::vector<char> get_vec(std::string info)
{
	return std::vector<char>(info.begin(), info.end());
}

int main(int argc, char* argv[])
{
	unsigned short port = 8080;
	if (argc != 2)
		std::cout << "using default config IP : 127.0.0.1 and Port 8080\n";
	else
		port = std::stoul(argv[1]);

	std::shared_ptr<Server> server = std::make_shared<Server>(port);

	server->set_data_receiver_handler_funtion([](unsigned short clientId, const std::vector<char>& data, std::size_t length) {
		if (clientId == 1001)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		}
		std::cout << "Custom handler: Received data from client " << clientId << ": " << std::string(data.begin(), data.end()) << "\n";
		});

	bool key[2] = { false, false };
	bool old_key[2] = { false, false };
	bool quit = false;

	while (!quit)
	{
		auto ip_map = server->get_ip_map();
		key[0] = GetAsyncKeyState('1') & 0x8000;
		key[1] = GetAsyncKeyState(VK_ESCAPE) & 0x8000;

		if (key[0] && !old_key[0]) server->broad_cast(get_vec("testing clients\n"));
		if (key[1] && !old_key[1]) quit = true;

		for (int i = 0; i < 2; i++) old_key[i] = key[i];
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return 0;
}
























