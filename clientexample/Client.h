#pragma once
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/signals2.hpp>

#include "../common/common.h"

class Client
{
public:
	Client(const std::string server_ip, const unsigned short server_port);
	~Client();

	void send(const std::vector<char>& data);

	void connect();

	void handle_disconnection(const boost::system::error_code& error);
	

	boost::signals2::connection connect_on_data_received(std::function<void(const std::vector<char>& data, const std::size_t length)> func);

private:

	boost::asio::io_context io_context_;
	void receive_data();
	const std::string server_ip_;
	unsigned short server_port_;
	std::vector<char> buffer_;
	boost::asio::ip::tcp::socket socket_;
	std::thread io_context_thread_;

	boost::signals2::signal<void( const std::vector<char>& data, const std::size_t length)> on_data_received_signal_;

	Colors colors;

};

