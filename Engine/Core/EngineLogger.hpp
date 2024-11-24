#pragma once
#include "Platform/Platform.hpp"
#include "Core/Console.hpp"

template<typename ... Args>
char* AppendLogMessage(const char* format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format, args ...) + 2; // Extra space for '\0'
	if (size_s > 0) {
		size_t size = static_cast<size_t>(size_s);
		char* buf = new char[size];
		std::snprintf(buf, size, format, args ...);
		buf[size - 2] = '\n';
        buf[size - 1] = '\0';
		return buf;
	}

	return nullptr;
}

class DAPI EngineLogger{
public:
    EngineLogger();
};

// Logger
#ifdef LEVEL_DEBUG
#define ASSERT(expr) \
    if(!(expr)) { \
        UL_DEBUG(#expr " is null!"); \
        exit(-1); \
}

#ifndef LOG_DEBUG
#define LOG_DEBUG(format, ...) \
    {char* str = AppendLogMessage(format, ##__VA_ARGS__);\
    Platform::PlatformConsoleWrite(str, 3);  \
    Console::WriteLine(Log::Logger::DEBUG, str);  \
    delete[] str;\
    UL_DEBUG(format, ##__VA_ARGS__);}
#endif

#ifndef LOG_INFO
#define LOG_INFO(format, ...) \
    {char* str = AppendLogMessage(format, ##__VA_ARGS__);\
    Platform::PlatformConsoleWrite(str, 5);  \
    Console::WriteLine(Log::Logger::INFO, str);  \
    delete[] str;\
    UL_INFO(format, ##__VA_ARGS__);}
#endif

#ifndef LOG_WARN
#define LOG_WARN(format, ...) \
    {char* str = AppendLogMessage(format, ##__VA_ARGS__);\
    Platform::PlatformConsoleWrite(str, 2);  \
    Console::WriteLine(Log::Logger::WARN, str);  \
    delete[] str;\
    UL_WARN(format, ##__VA_ARGS__);}
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(format, ...) \
    {char* str = AppendLogMessage(format, ##__VA_ARGS__);\
    Platform::PlatformConsoleWrite(str, 1);  \
    Console::WriteLine(Log::Logger::ERROR, str);  \
    delete[] str;\
    UL_ERROR(format, ##__VA_ARGS__);}
#endif

#ifndef LOG_FATAL
#define LOG_FATAL(format, ...) \
    {char* str = AppendLogMessage(format, ##__VA_ARGS__);\
    Platform::PlatformConsoleWrite(str, 0);  \
    Console::WriteLine(Log::Logger::FATAL, str);  \
    delete[] str;\
    UL_FATAL(format, ##__VA_ARGS__);}
#endif

#else

#ifndef LOG_DEBUG
#define LOG_DEBUG(format, ...) 
#endif

#ifndef LOG_INFO
#define LOG_INFO(format, ...) \
    {char* str = AppendLogMessage(format, ##__VA_ARGS__);\
    Console::WriteLine(Log::Logger::INFO, str);  \
    delete[] str;\
    UL_INFO(format, ##__VA_ARGS__);}
#endif

#ifndef LOG_WARN
#define LOG_WARN(format, ...) \
    {char* str = AppendLogMessage(format, ##__VA_ARGS__);\
    Console::WriteLine(Log::Logger::WARN, str);  \
    delete[] str;\
    UL_WARN(format, ##__VA_ARGS__);}
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(format, ...) \
    {char* str = AppendLogMessage(format, ##__VA_ARGS__);\
    Console::WriteLine(Log::Logger::ERROR, str);  \
    delete[] str;\
    UL_ERROR(format, ##__VA_ARGS__);}
#endif

#ifndef LOG_FATAL
#define LOG_FATAL(format, ...) \
    {char* str = AppendLogMessage(format, ##__VA_ARGS__);\
    Console::WriteLine(Log::Logger::FATAL, str);  \
    delete[] str;\
    UL_FATAL(format, ##__VA_ARGS__);}
#endif

#define ASSERT(expr) {}
#endif //ifdef DEBUG
