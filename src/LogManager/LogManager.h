#pragma once

namespace Logger {
	class LogManager {
	public:
		LogManager() = default;
		~LogManager() = default;
		void initialize();
		void shutdown();
	};
}