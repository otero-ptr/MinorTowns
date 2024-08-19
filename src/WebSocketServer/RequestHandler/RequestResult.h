#pragma once
#include "operations.h"
#include "params.h"
#include <memory>

class RequestResult {
public:
	RequestResult(RequestOperation&& operation)
		: operation(operation),
		params(nullptr) {

	}
	RequestResult(RequestOperation&& operation, std::shared_ptr<Params::Params> params)
		: operation(operation),
		params(params) {

	}
	~RequestResult() {

	}		
	RequestResult(RequestResult&& other) noexcept
		: operation(other.operation),
		params(std::move(other.params)) {
	}
	RequestResult(const RequestResult&) = delete;
	RequestResult& operator=(const RequestResult&) = delete;
	RequestResult& operator=(RequestResult&&) = delete;

	const RequestOperation getOperation() const {
		return operation;
	}
	bool isParams() const { return params != nullptr; }
	std::weak_ptr<Params::Params> getParams() const { return params; }
private:
	RequestOperation operation;
	std::shared_ptr<Params::Params> params;
};