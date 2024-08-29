#pragma once
#include <string>
#include <vector>
#include <boost/asio.hpp>

class Client
{
public:
	Client(const std::string server_ip, const unsigned short server_port);
	~Client();

	void send(const std::vector<char>& data);

	void connect();
	bool disconnect();



private:

	boost::asio::io_context io_context_;
	void receive_data();
	const std::string server_ip_;
	unsigned short server_port_;
	std::vector<char> buffer_;
	boost::asio::ip::tcp::socket socket_;
	std::thread io_context_thread_;

};

