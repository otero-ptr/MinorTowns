#include "LogManager.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>
#include "log.h"

namespace Logger {
	void LogManager::initialize()
	{
		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		auto logFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs.log", true);
		consoleSink->set_pattern("%^[%l] [%Y-%m-%d %H:%M:%S.%e] [%t] %v%$");
		logFileSink->set_pattern("%^[%l] [%d-%m-%Y %H:%M:%S.%e] [%t] %v%$");

		std::shared_ptr<spdlog::logger> myLogger = std::make_shared<spdlog::logger>(LOGGER_DEFAULT_NAME);
		myLogger->sinks().push_back(consoleSink);
		myLogger->sinks().push_back(logFileSink);
		myLogger->set_level(spdlog::level::trace);
		myLogger->flush_on(spdlog::level::trace);
		spdlog::register_logger(myLogger);
	}

	void LogManager::shutdown()
	{
		spdlog::shutdown();
	}
}