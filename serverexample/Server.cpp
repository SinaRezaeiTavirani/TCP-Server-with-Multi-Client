#include "Server.h"


#include <boost/exception/all.hpp>



Server::Server( unsigned short port) : port_(port)
{
	running_ = true;
	id_ = 1000;
	std::thread listen_thread([this]() { startListen(); });
	thread_worker_ = std::move(listen_thread);
}

Server::~Server()
{
	stop();
	if (thread_worker_.joinable()) {
		thread_worker_.join();
	}
}	

void Server::startListen() {
    std::cout << colors.yellow << "Server is now listening for connections.\n" << colors.white;

	using namespace boost;
	try {
		

		asio::ip::tcp::acceptor acceptor(io_context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_)); // Port 8080

		std::cout << "Server is running on port: " << colors.yellow << port_ << colors.white <<  " ...\n";


		std::function<void()> accept_handler = [&]() {
			
			if (!running_) return; // Exit if server is stopping

				
			// Create a new socket to handle the incoming connection
			socket_map[id_] = std::make_shared<asio::ip::tcp::socket>(io_context_);

			// Asynchronously accept a new connection
			acceptor.async_accept(*socket_map[id_],
				[&, this](const std::error_code& error) {
					if (!error) {
						std::cout << colors.green <<  "New client connected.\n" << colors.white;
						ip_map_[id_] = socket_map[id_]->remote_endpoint().address().to_string();
						handleClient(id_);
					}
					else {
						std::cerr << "Failed to accept connection: " << error.message() << "\n";
					}

					// Continue accepting new connections
					id_++;
					accept_handler();
				});
			};
		accept_handler();

		// Run the I/O context to process asynchronous events
		io_context_.run();
	}

	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

}

void Server::stop() {
	if (running_) {
		running_ = false;
		io_context_.stop(); // Stop the io_context loop
		std::cout << "Server is stopping.\n";
	}
}

void Server::handleClient(unsigned short clientId) {
    

	auto v_buffer = std::make_shared<std::vector<char>>(1024);

	using namespace boost;
	// Asynchronously read data from the socket
	socket_map[clientId]->async_read_some(asio::buffer(v_buffer->data(), v_buffer->size()),
		[this, v_buffer, clientId](const boost::system::error_code& error, std::size_t length) {
			if (!error) 
			{
				std::cout << "Received data from ID :  " << colors.bright_cyan << clientId << colors.white << " " << std::string(v_buffer->data(), length) << "\n" << colors.white;
				handleClient(clientId);
			}
			else if (error == asio::error::eof) 
			{
				std::cout << "Client ID " << colors.bright_cyan << clientId << colors.red << " disconnected (EOF).\n" << colors.white;
				socket_map[clientId]->close();
			}
			else if (error == asio::error::connection_reset) 
			{
				std::cout << "Client ID " << colors.bright_cyan << clientId << colors.red  << " disconnected (connection reset).\n" << colors.white;
				socket_map[clientId]->close();
			}
			else 
			{
				std::cerr << colors.red << "Failed to read payload: " << error.message() << "\n" << colors.white;
			}
		});

}

std::unordered_map<short, std::string> Server::get_ip_map()
{
	return ip_map_;
}

void Server::send(unsigned short clientId, const std::vector<char>& data) {
    // Send data to client

	using namespace boost;

	if (socket_map[clientId]->is_open()) {
		//std::cout << "Sending data to client with ID: " << clientId << "\n";
		asio::async_write(*socket_map[clientId], asio::buffer(data.data(), data.size()),
			[this, clientId](std::error_code ec, std::size_t length) {
				if (!ec) {
					//std::cout << "Success: Sent to ID  " << clientId << " " << length << " bytes\n";
				}
				else {
					std::cerr << colors.red << "Failed to send data: " << ec.message() << "\n" << colors.white;
				}
			});
	}
}

void Server::broad_cast(const std::vector<char>& data)
{
	for (auto& ip : ip_map_)
	{
		send(ip.first, data);
	}
}


