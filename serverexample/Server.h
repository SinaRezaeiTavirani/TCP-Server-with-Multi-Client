#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <list>
#include <unordered_map>

#include <boost/asio.hpp>

#include "common.h"

class Server {
public:

	Server(unsigned short port);
	~Server();

	std::unordered_map<short, std::string> get_ip_map();

	void send(unsigned short clientId, const std::vector<char>& data);
	void broad_cast(const std::vector<char>& data);


private:
	void startListen();

	void stop();
	std::atomic<bool> running_;

	void handleClient(unsigned short clientId);
	unsigned port_;
	std::thread thread_worker_;

	std::unordered_map<short, std::shared_ptr<boost::asio::ip::tcp::socket> > socket_map;
	std::unordered_map<short, std::string> ip_map_;

	std::atomic<unsigned short> id_;
	boost::asio::io_context io_context_;

	Colors colors;

};
