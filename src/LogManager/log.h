#pragma once
#include "spdlog\spdlog.h"

#define LOGGER_DEFAULT_NAME "logger"

// DEFAULT LOGGER
#define LOGGER_TRACE(...)	if(spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) {spdlog::get(LOGGER_DEFAULT_NAME)->trace(__VA_ARGS__);}
#define LOGGER_DEBUG(...)	if(spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) {spdlog::get(LOGGER_DEFAULT_NAME)->debug(__VA_ARGS__);}
#define LOGGER_INFO(...)	if(spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) {spdlog::get(LOGGER_DEFAULT_NAME)->info(__VA_ARGS__);}
#define LOGGER_WARN(...)	if(spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) {spdlog::get(LOGGER_DEFAULT_NAME)->warn(__VA_ARGS__);}
#define LOGGER_ERROR(...)	if(spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) {spdlog::get(LOGGER_DEFAULT_NAME)->error(__VA_ARGS__);}
#define LOGGER_FATAL(...)	if(spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) {spdlog::get(LOGGER_DEFAULT_NAME)->critical(__VA_ARGS__);}