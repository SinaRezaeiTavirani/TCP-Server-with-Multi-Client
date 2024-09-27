#pragma once
#include <string>
#include <vector>
#include <asio.hpp>

#include "../common/common.h"

class Client
{
public:
	Client(const std::string server_ip, const unsigned short server_port);
	~Client();

	void send(const std::vector<char>& data);

	void connect();

	void handle_disconnection(const asio::error_code& error);
	
	void set_data_receiver_handler_funtion(std::function<void(const std::vector<char>& data, const std::size_t length)> data_receiver_handler);

	

private:

	asio::io_context io_context_;
	void receive_data();
	const std::string server_ip_;
	unsigned short server_port_;
	std::vector<char> buffer_;
	asio::ip::tcp::socket socket_;
	std::thread io_context_thread_;


	std::function<void(const std::vector<char>& data, const std::size_t length)> data_receiver_handler_;

	Colors colors;

};

