#pragma once

#include <memory>
#include <string>

namespace adb
{
	/// Retrieve the version of local adb server.
   /**
	* @return 4-byte string of the version number.
	* @throw std::system_error if the server is not available.
	*/
	std::string version();

	std::string devices();

	void kill_server();
}