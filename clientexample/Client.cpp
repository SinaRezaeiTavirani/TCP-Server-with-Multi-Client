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
	asio::error_code ec;
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address(server_ip_,ec), server_port_);
	socket_.async_connect(endpoint,
		[this](asio::error_code ec) {
			if (!ec) {
				std::cout << colors.green <<  "Connected to the server.\n" << colors.white;
				receive_data();
			}
			else {
				std::cerr << colors.red << "Failed to connect: " << ec.message() << colors.white << std::endl;
				
			}
		});
	
}
void Client::handle_disconnection(const asio::error_code& error) {
	if (error == asio::error::eof) {
		std::cout << "Server " << colors.red << " disconnected (EOF).\n" << colors.white;
	}
	else if (error == asio::error::connection_reset) {
		std::cout << "Server " << colors.red << " disconnected (connection reset).\n" << colors.white;
	}
	else {
		std::cerr << colors.red << "Failed to read payload: " << error.message() << "\n" << colors.white;
	}
	// Close and remove the socket
	socket_.close();
	
	
}





void Client::receive_data()
{
	
	auto v_buffer = std::make_shared<std::vector<char>>(1024);

	using namespace boost;
	// Asynchronously read data from the socket
	socket_.async_read_some(asio::buffer(v_buffer->data(), v_buffer->size()),
		[this, v_buffer](const asio::error_code& error, std::size_t length) {
			if (!error) {
				try {
					data_receiver_handler_(*v_buffer, length); // Trigger the data received signal
				}
				catch (std::exception& ex) {
					std::cout << colors.red << "Exception in data receiver handler, maybe you didn't specify function handler for receiving data : " << ex.what() << "\n" << colors.white;
				}
				receive_data();
			}
			else {
				handle_disconnection( error);
			}
		});


}
void Client::set_data_receiver_handler_funtion(std::function<void(const std::vector<char>& data, const std::size_t length)> data_receiver_handler)
{
	data_receiver_handler_ = (data_receiver_handler);
}
