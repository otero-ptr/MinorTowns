#pragma once
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/URI.h"

bool handleConnect(Poco::Net::HTTPServerResponse& response, const Poco::URI::QueryParameters &params) {
	bool status = true;
	if (params.size() != 1) {
		return false;
	}
	if (params.begin()->first != "username" || params.begin()->second.empty()) {
		return false;
	}
	if (!status) {
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		response.setContentLength(0);
		response.send();
	}
	return status;
}
