#include <gtest\gtest.h>
#include "Params.h"
#include "ParamsValidator.h"

TEST(ParamsValidatorTest, CorrectParamsCreateLobby) {
	std::shared_ptr params = std::make_shared<Params::CreateLobby>(); 
	
	{
		params->max_users = 2;
		EXPECT_TRUE(ParamsValidator::validate(params));
	}

	{
		params->max_users = 3;
		EXPECT_TRUE(ParamsValidator::validate(params));
	}

	{
		params->max_users = 4;
		EXPECT_TRUE(ParamsValidator::validate(params));
	}
}

TEST(ParamsValidatorTest, InorrectParamsCreateLobby) {
	std::shared_ptr params = std::make_shared<Params::CreateLobby>();

	{
		params->max_users = 0;
		EXPECT_FALSE(ParamsValidator::validate(params));
	}

	{
		params->max_users = 5;
		EXPECT_FALSE(ParamsValidator::validate(params));
	}
}

TEST(ParamsValidatorTest, CorrectParamsJoinLobby) {
	std::shared_ptr params = std::make_shared<Params::JoinLobby>();

	{
		params->uuid_lobby = "501f3428-f90e-11ee-a422-0250cf951982";
		EXPECT_TRUE(ParamsValidator::validate(params));
	}
}

TEST(ParamsValidatorTest, IncorrectParamsJoinLobby) {
	std::shared_ptr params = std::make_shared<Params::JoinLobby>();

	{
		params->uuid_lobby = "501f3428-f90e-11ee-0250cf951982";
		EXPECT_FALSE(ParamsValidator::validate(params));
	}

	{
		params->uuid_lobby = "501f3428-f90e-11ee-a422-0250cf951982-";
		EXPECT_FALSE(ParamsValidator::validate(params));
	}

	{
		params->uuid_lobby = "501f3428_f90e_11ee_a422_0250cf951982";
		EXPECT_FALSE(ParamsValidator::validate(params));
	}

	{
		params->uuid_lobby = "5%1f3428-f90e-!!ee-a422-0250cf951982";
		EXPECT_FALSE(ParamsValidator::validate(params));
	}
}

TEST(ParamsValidatorTest, CorrectParamsRaiseArmy) {
	std::shared_ptr params = std::make_shared<Params::RaiseArmy>();

	{
		params->soldiers = 1;
		EXPECT_TRUE(ParamsValidator::validate(params));
	}

	{
		params->soldiers = 100'000;
		EXPECT_TRUE(ParamsValidator::validate(params));
	}
}

TEST(ParamsValidatorTest, IncorrectParamsRaiseArmy) {
	std::shared_ptr params = std::make_shared<Params::RaiseArmy>();

	{
		params->soldiers = 0;
		EXPECT_FALSE(ParamsValidator::validate(params));
	}

	{
		params->soldiers = 100'001;
		EXPECT_FALSE(ParamsValidator::validate(params));
	}

	{
		params->soldiers = -5;
		EXPECT_FALSE(ParamsValidator::validate(params));
	}

	{
		params->soldiers = -100'000;
		EXPECT_FALSE(ParamsValidator::validate(params));
	}
}

TEST(ParamsValidatorTest, CorrectParamsDisbandArmy) {
	std::shared_ptr params = std::make_shared<Params::DisbandArmy>();

	{
		params->soldiers = 1;
		EXPECT_TRUE(ParamsValidator::validate(params));
	}

	{
		params->soldiers = 100'000;
		EXPECT_TRUE(ParamsValidator::validate(params));
	}
}

TEST(ParamsValidatorTest, IncorrectParamsDisbandArmy) {
	std::shared_ptr params = std::make_shared<Params::DisbandArmy>();

	{
		params->soldiers = 0;
		EXPECT_FALSE(ParamsValidator::validate(params));
	}

	{
		params->soldiers = 100'001;
		EXPECT_FALSE(ParamsValidator::validate(params));
	}

	{
		params->soldiers = -5;
		EXPECT_FALSE(ParamsValidator::validate(params));
	}

	{
		params->soldiers = -100'000;
		EXPECT_FALSE(ParamsValidator::validate(params));
	}
}

TEST(ParamsValidatorTest, CorrectParamsMoveArmy) {
	std::shared_ptr params = std::make_shared<Params::MoveArmy>();

	{
		params->node = 0;
		EXPECT_TRUE(ParamsValidator::validate(params));
	}

	{
		params->node = 128;
		EXPECT_TRUE(ParamsValidator::validate(params));
	}

	{
		params->node = 255;
		EXPECT_TRUE(ParamsValidator::validate(params));
	}
}