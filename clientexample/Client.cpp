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
				std::cout << colors.green <<  "Connected to the server.\n" << colors.white;
				receive_data();
			}
			else {
				std::cerr << colors.red << "Failed to connect: " << ec.message() << colors.white << std::endl;
				
			}
		});
	
}
void Client::handle_disconnection(const boost::system::error_code& error) {
	if (error == boost::asio::error::eof) {
		std::cout << "Server " << colors.red << " disconnected (EOF).\n" << colors.white;
	}
	else if (error == boost::asio::error::connection_reset) {
		std::cout << "Server " << colors.red << " disconnected (connection reset).\n" << colors.white;
	}
	else {
		std::cerr << colors.red << "Failed to read payload: " << error.message() << "\n" << colors.white;
	}
	// Close and remove the socket
	socket_.close();
	
	
}



boost::signals2::connection Client::connect_on_data_received(std::function<void(const std::vector<char>& data, const std::size_t length)> func)
{
	return on_data_received_signal_.connect(func);
}

void Client::receive_data()
{
	
	auto v_buffer = std::make_shared<std::vector<char>>(1024);

	using namespace boost;
	// Asynchronously read data from the socket
	socket_.async_read_some(asio::buffer(v_buffer->data(), v_buffer->size()),
		[this, v_buffer](const boost::system::error_code& error, std::size_t length) {
			if (!error) {
				on_data_received_signal_( *v_buffer, length); // Trigger the data received signal
				receive_data();
			}
			else {
				handle_disconnection( error);
			}
		});


}
