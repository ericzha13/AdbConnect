// AdbConnect.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define GLOG_NO_ABBREVIATED_SEVERITIES 0
#include <iostream>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include "client.hpp"
#include <regex>

int main()
{
	google::InitGoogleLogging("AABB");

	//FLAGS_v = 1;// no used
	FLAGS_log_dir = R"(C:\Users\lsp\Downloads\LLOG)";
	FLAGS_logtostdout = true;
	//FLAGS_alsologtostderr = false;
	FLAGS_logtostderr = true;
	//FLAGS_stderrthreshold = google::GLOG_WARNING;
	FLAGS_colorlogtostdout = true;
	FLAGS_colorlogtostderr = true;
	//FLAGS_max_log_size = 3;
	FLAGS_stop_logging_if_full_disk = true;
	{
		try
		{
			//LOG(INFO) << "adb version ->"<< adb::version();
			//adb::kill_server();
			//std::string ts = "";
			//std::regex regex_as(R"(^.+ rst$)");
			//bool ret = std::regex_match("a     rst", regex_as);
			//if (ret) puts("true");

			int i = 1;
			while (i--)
			{
				LOG(INFO) << "time = " << i;
				LOG(WARNING) << "adb devices ->" << adb::devices();
				LOG(INFO) << "adb version2 ->" << adb::version();
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "adb Connection failed: " << e.what() << std::endl;
		}
		
		//host_request("host:version");
	}


    LOG(INFO) << "Hello World!\n";
	google::ShutdownGoogleLogging();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
