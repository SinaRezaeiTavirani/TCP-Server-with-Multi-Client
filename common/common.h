#pragma once

#include <string>

struct Colors {
	const std::string black = "\033[30m";
	const std::string red = "\033[31m";
	const std::string green = "\033[32m";
	const std::string yellow = "\033[33m";
	const std::string blue = "\033[34m";
	const std::string magenta = "\033[35m";
	const std::string cyan = "\033[36m";
	const std::string white = "\033[37m";
	const std::string bright_black = "\033[90m";
	const std::string bright_red = "\033[91m";
	const std::string bright_green = "\033[92m";
	const std::string bright_yellow = "\033[93m";
	const std::string bright_blue = "\033[94m";
	const std::string bright_magenta = "\033[95m";
	const std::string bright_cyan = "\033[96m";
	const std::string bright_white = "\033[97m";
	const std::string reset = "\033[0m";
};