#pragma once
#include "operations.h"
#include "params.h"
#include "ErrorInfo.h"
#include <memory>

class RequestResult {
public:
	RequestResult() 
		: operation(RequestOperation::NONE), 
		params(nullptr), err_info(nullptr) {

	}
	~RequestResult() {

	}		
	RequestResult(RequestResult&& other) noexcept
		: operation(other.operation),
		params(std::move(other.params)),
		err_info(std::move(other.err_info)) {
	}
	RequestResult(const RequestResult&) = delete;
	RequestResult& operator=(const RequestResult&) = delete;
	RequestResult& operator=(RequestResult&&) = delete;

	bool isError() const { return this->operation == RequestOperation::ERROR_OPERATION; }
	bool isParams() const { return params != nullptr; }

	const RequestOperation getOperation() const {
		return this->operation;
	}
	const Params::Params* getParams() const { return params.get(); }
	const ErrorInfo* getErrorInfo() const { return err_info.get(); }

	void setOperation(RequestOperation&& operation) {
		this->operation = operation;
	}
	void setParams(std::unique_ptr<Params::Params> params) { this->params = std::move(params); }
	void setErrorInfo(std::unique_ptr <ErrorInfo> err_info) {
		this->operation = RequestOperation::ERROR_OPERATION;
		this->err_info = std::move(err_info);
	}
private:
	RequestOperation operation;
	std::unique_ptr<Params::Params> params;
	std::unique_ptr<ErrorInfo> err_info;
};