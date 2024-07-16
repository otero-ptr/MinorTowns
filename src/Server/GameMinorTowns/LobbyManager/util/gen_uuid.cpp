#include "gen_uuid.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"

std::string GenerateUUID()
{
	Poco::UUIDGenerator generator;
	Poco::UUID uuid = generator.create();
	return uuid.toString();
}
