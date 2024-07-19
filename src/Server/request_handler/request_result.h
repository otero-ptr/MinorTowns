#pragma once
#include "operations.h"
#include "params.h"
#include "error_info.h"

struct RequestResult {
private:
	Params::Params* params = nullptr;
	ErrorInfo* err_info = nullptr;
public:
	RequestOperation operation;

	RequestResult() = default;
	~RequestResult() {
		if (params) {
			delete params;
		}
		if (err_info) {
			delete err_info;
		}
	}		
	RequestResult(RequestResult&& other)
		: operation(other.operation),
		params(other.params),
		err_info(other.err_info)
	{
		other.params = nullptr;
		other.err_info = nullptr;
	}
	RequestResult(const RequestResult&) = delete;
	RequestResult& operator=(const RequestResult&) = delete;
	RequestResult& operator=(RequestResult&&) = delete;

	bool isError() const { return err_info != nullptr; }
	bool isParams() const { return params != nullptr; }
	const Params::Params* getParams() const { return params; }
	const ErrorInfo* getErrorInfo() const { return err_info; }
	void setParams(Params::Params* params) { this->params = params; }
	void setErrorInfo(ErrorInfo* err_info) {
		this->operation = RequestOperation::ERROR_OPERATION;
		this->err_info = err_info;
	}
};