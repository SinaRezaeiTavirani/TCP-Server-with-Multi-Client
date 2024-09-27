#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <list>
#include <unordered_map>

#include <asio.hpp>

#include "../common/common.h"

class Server {
public:

	Server(unsigned short port);
	~Server();

	std::unordered_map<short, std::string> get_ip_map();

	void send(unsigned short clientId, const std::vector<char>& data);
	void broad_cast(const std::vector<char>& data);
	
	void set_data_receiver_handler_funtion(std::function<void(const unsigned short client_id, const std::vector<char>& data, const std::size_t length)> data_receiver_handler);


private:
	void startListen();

	void stop();
	std::atomic<bool> running_;

	void handle_client(unsigned short clientId);
	void handle_disconnection(unsigned short clientId, const asio::error_code& error);
	unsigned port_;
	std::thread thread_worker_;

	std::unordered_map<short, std::shared_ptr<asio::ip::tcp::socket> > socket_map;
	std::unordered_map<short, std::string> ip_map_;

	std::atomic<unsigned short> id_;
	asio::io_context io_context_;

	std::function<void(const unsigned short client_id, const std::vector<char>& data, const std::size_t length)> data_receiver_handler_;

	Colors colors;

};
