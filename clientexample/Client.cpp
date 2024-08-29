#include "Client.h"
#include <iostream>



Client::Client(const std::string server_ip, const unsigned short server_port) : server_ip_(server_ip), server_port_(server_port), 
				socket_(io_context_)
{
	connect();
	io_context_thread_ = std::thread([this]() { io_context_.run(); });
}

Client::~Client()
{
	io_context_.stop();
	if (io_context_thread_.joinable()) {
		io_context_thread_.join();
	}
}

void Client::send(const std::vector<char>& data)
{
	using namespace boost;

	if (socket_.is_open()) {
		//std::cout << "Sending data to server" << "\n";
		asio::async_write(socket_, asio::buffer(data.data(), data.size()),
			[this](std::error_code ec, std::size_t length) {
				if (!ec) {
					//std::cout << "Success: Sent to server  "  << length << " bytes\n";
				}
				else {
					std::cerr << "Failed to send data: " << ec.message() << "\n";
				}
			});
	}
}

void Client::connect()
{
	boost::system::error_code ec;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(server_ip_,ec), server_port_);
	socket_.async_connect(endpoint,
		[this](boost::system::error_code ec) {
			if (!ec) {
				std::cout << "Connected to the server.\n";
				receive_data();
			}
			else {
				std::cerr << "Failed to connect: " << ec.message() << std::endl;
				
			}
		});
	
}

bool Client::disconnect()
{
	return false;
}

void Client::receive_data()
{
	
	auto v_buffer = std::make_shared<std::vector<char>>(1024);

	using namespace boost;
	// Asynchronously read data from the socket
	socket_.async_read_some(asio::buffer(v_buffer->data(), v_buffer->size()),
		[this, v_buffer](const boost::system::error_code& error, std::size_t length) {
			if (!error) {
				std::cout << "Received data from server " << std::string(v_buffer->data(), length) << "\n";
				
				receive_data();
			}
			else if (error == asio::error::eof) {
				
				std::cout << "server disconnected (EOF).\n";
				socket_.close();
			}
			else if (error == asio::error::connection_reset) {
				// The connection was forcibly closed
				std::cout << "server disconnected (connection reset).\n";
				socket_.close();
			}
			else {
				std::cerr << "Failed to read payload: " << error.message() << "\n";
			}
		});


}
