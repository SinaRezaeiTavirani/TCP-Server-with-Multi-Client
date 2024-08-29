#include <iostream>
#include <memory>
#include <vector>

#include "Client.h"


std::vector<char> get_vec(std::string info)
{
	return std::vector<char>(info.begin(), info.end());
}

int main()
{
	std::shared_ptr<Client> client = std::make_shared<Client>("127.0.0.1", 8080);

	bool key[2] = { false, false };
	bool old_key[2] = { false, false };
	bool quit = false;

	while (!quit)
	{
		
		key[0] = GetAsyncKeyState('2') & 0x8000;
		key[1] = GetAsyncKeyState(VK_ESCAPE) & 0x8000;
		
		if (key[0] && !old_key[0]) client->send(get_vec("testing server\n"));
		if (key[1] && !old_key[2]) quit = true;

		for (int i = 0; i < 2; i++) old_key[i] = key[i];
	}

	return 0;
}
