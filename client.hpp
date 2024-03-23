#pragma once

#include <memory>
#include <string>
#include <string_view>

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



    /// A client for the Android Debug Bridge.
    class client
    {
    public:
        /// Create a client for a specific device.
        /**
         * @param serial serial number of the device.
         * @note If the serial is empty, the unique device will be used. If there
         * are multiple devices, an exception will be thrown.
         */
        static std::shared_ptr<client> create(const std::string_view serial);
        virtual ~client() = default;

        /// Connect to the device.
        /**
         * @return A string of the connection status.
         * @throw std::system_error if the server is not available.
         * @note Equivalent to `adb connect <serial>`.
         */
        virtual std::string connect() = 0;

        /// Disconnect from the device.
        /**
         * @return A string of the disconnection status.
         * @throw std::system_error if the server is not available.
         * @note Equivalent to `adb disconnect <serial>`.
         */
        virtual std::string disconnect() = 0;

        /// Retrieve the version of local adb server.
        /**
         * @return 4-byte string of the version number.
         * @throw std::system_error if the server is not available.
         * @note This function reuses the class member io_context, which is
         * thread-safe for the client.
         */
        virtual std::string version() = 0;

        /// Retrieve the available Android devices.
        /**
         * @return A string of the list of devices.
         * @throw std::system_error if the server is not available.
         * @note Equivalent to `adb devices`.
         * @note This function reuses the class member io_context, which is
         * thread-safe for the client.
         */
        virtual std::string devices() = 0;

        /// Send an one-shot shell command to the device.
        /**
         * @param command Command to execute.
         * @return A string of the command output.
         * @throw std::system_error if the server is not available.
         * @note Equivalent to `adb -s <serial> shell <command>` without stdin.
         */
        virtual std::string shell(const std::string_view command) = 0;

        /// Send an one-shot shell command to the device, using raw PTY.
        /**
         * @param command Command to execute.
         * @return A string of the command output, which is not mangled.
         * @throw std::system_error if the server is not available.
         * @note Equivalent to `adb -s <serial> exec-out <command>` without stdin.
         */
        virtual std::string exec(const std::string_view command) = 0;

        /// Send a file to the device.
        /**
         * @return true if the file is successfully sent.
         * @param src Path to the source file.
         * @param dst Path to the destination file.
         * @param perm Permission of the destination file.
         * @throw std::system_error if the server is not available.
         * @note Equivalent to `adb -s <serial> push <src> <dst>`.
         */
        virtual bool push(const std::string_view src, const std::string_view dst, int perm) = 0;

        /// Set the user of adbd to root on the device.
        /**
         * @throw std::system_error if the server is not available.
         * @note Equivalent to `adb -s <serial> root`.
         * @note The device might be offline after this command. Remember to wait
         * for the restart.
         */
        virtual std::string root() = 0;

        /// Set the user of adbd to non-root on the device.
        /**
         * @throw std::system_error if the server is not available.
         * @note Equivalent to `adb -s <serial> unroot`.
         * @note The device might be offline after this command. Remember to wait
         * for the restart.
         */
        virtual std::string unroot() = 0;

        /// Wait for the device to be available.
        /**
         * @throw std::system_error if the server is not available.
         */
        virtual void wait_for_device() = 0;

    protected:
        client() = default;
    };
}