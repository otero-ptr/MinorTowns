#include "LogManager.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>
#include "log.h"

namespace Logger {
	void LogManager::initialize()
	{
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		auto log_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/logs.log", true);
		console_sink->set_pattern("%^[%l] [%Y-%m-%d %H:%M:%S.%e] [%t] %v%$");
		log_file_sink->set_pattern("%^[%l] [%d-%m-%Y %H:%M:%S.%e] [%t] %v%$");

		std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>(LOGGER_DEFAULT_NAME);
		logger->sinks().push_back(console_sink);
		logger->sinks().push_back(log_file_sink);
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);
		spdlog::register_logger(logger);
	}

	void LogManager::shutdown()
	{
		spdlog::shutdown();
	}
}