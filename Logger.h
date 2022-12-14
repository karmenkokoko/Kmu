/*
 * 懒汉单例模式
*/
#pragma once
#include "noncopyable.h"
#include <string>
#define MUDEBUG
#define LOG_INFO(logmsgFormat, ...)                       \
	do                                                    \
	{                                                     \
		Logger &logger = Logger::instance();              \
		logger.setLogLevel(INFO);                         \
		char buf[1024] = {0};                             \
		snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
		logger.log(buf);                                  \
	} while (0)

#define LOG_ERROR(logmsgFormat, ...)                      \
	do                                                    \
	{                                                     \
		Logger &logger = Logger::instance();              \
		logger.setLogLevel(ERROR);                        \
		char buf[1024] = {0};                             \
		snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
		logger.log(buf);                                  \
	} while (0)

#define LOG_FATAL(logmsgFormat, ...)                      \
	do                                                    \
	{                                                     \
		Logger &logger = Logger::instance();              \
		logger.setLogLevel(FATAL);                        \
		char buf[1024] = {0};                             \
		snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
		logger.log(buf);                                  \
		exit(-1);                                         \
	} while (0)
#ifdef MUDEBUG
#define LOG_DEBUG(logmsgFormat, ...)                          \
	do                                                    \
	{                                                     \
		Logger &logger = Logger::instance();              \
		logger.setLogLevel(DEBUG);                        \
		char buf[1024] = {0};                             \
		snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
		logger.log(buf);                                  \
	} while (0)
#else
#define LOG_DEBUG(logmsgFormat, ...)
#endif

enum LogLevel
{
	INFO,  //普通讯息
	ERROR, //错误讯息
	FATAL, //严重错误
	DEBUG, //调试讯息
};


class Logger : noncopyable
{
	public:
		static Logger& instance(); // 静态的 只用有一个，与实例无关
		void setLogLevel(int level);
		void log(std::string msg);
	private:
		Logger(){}; // 私有化构造函数
		int logLevel_;
};
