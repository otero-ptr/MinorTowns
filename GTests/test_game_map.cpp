#include <gtest/gtest.h>
#include "GameMap/GameMap.h"
#include <iostream>

TEST(GameMapTest, SizeMap) {
	using namespace DimensionMap;

	SizeMap size_map(4, 2);
	ASSERT_EQ(size_map.x, 4);
	ASSERT_EQ(size_map.y, 2);
}

TEST(GameMapTest, DimensionMapDetect) {
	using namespace DimensionMap;
	const SizeMap exp_two_players(5, 3);
	const SizeMap exp_three_players(5, 3);
	const SizeMap exp_four_players(5, 5);
	const SizeMap exp_error_players(0, 0);

	{
		const int players = 2;
		SizeMap result = detect(players);
		EXPECT_EQ(result.x, exp_two_players.x);
		EXPECT_EQ(result.y, exp_two_players.y);
	}

	{
		const int players = 3;
		SizeMap result = detect(players);
		EXPECT_EQ(result.x, exp_three_players.x);
		EXPECT_EQ(result.y, exp_three_players.y);
	}

	{
		const int players = 4;
		SizeMap result = detect(players);
		EXPECT_EQ(result.x, exp_four_players.x);
		EXPECT_EQ(result.y, exp_four_players.y);
	}

	{
		const int players = 0;
		SizeMap result = detect(players);
		EXPECT_EQ(result.x, exp_error_players.x);
		EXPECT_EQ(result.y, exp_error_players.y);
	}

	{
		const int players = 5;
		SizeMap result = detect(players);
		EXPECT_EQ(result.x, exp_error_players.x);
		EXPECT_EQ(result.y, exp_error_players.y);
	}
}

TEST(GameMapTest, DimensionMapPlaceTowns) {
	using namespace DimensionMap;
	{
		const int players = 2;
		const SizeMap size(5, 3);

		const std::vector<SizeMap> expected_towns = {
		{0, 1},
		{4, 1}
		};

		auto towns = placeTowns(players, size);
		ASSERT_EQ(towns.size(), players);

		for (size_t i = 0; i < towns.size(); ++i) {
			EXPECT_EQ(towns[i].x, expected_towns[i].x);
			EXPECT_EQ(towns[i].y, expected_towns[i].y);
		}
	}
}

TEST(GameMapTest, PlacementTownsOnMap) {
	{
		const int players = 2;
		DimensionMap::SizeMap size = DimensionMap::detect(players);
		GameMap map(size);
		ASSERT_EQ(map.getRow(), size.x);
		ASSERT_EQ(map.getColumn(), size.y);
		EXPECT_TRUE(map.getMapJson().empty());
		auto towns = DimensionMap::placeTowns(players, size);
		ASSERT_EQ(towns.size(), players);
		auto ids = map.placeTowns(towns);
		const std::vector<int> exp_ids{ 1, 13 };
		ASSERT_EQ(ids.size(), exp_ids.size());
		for (size_t i = 0; i < towns.size(); ++i) {
			EXPECT_EQ(ids[i], exp_ids[i]);
		}
	}

	{
		const int players = 3;
		DimensionMap::SizeMap size = DimensionMap::detect(players);
		GameMap map(size);
		ASSERT_EQ(map.getRow(), size.x);
		ASSERT_EQ(map.getColumn(), size.y);
		EXPECT_TRUE(map.getMapJson().empty());
		auto towns = DimensionMap::placeTowns(players, size);
		ASSERT_EQ(towns.size(), players);
		auto ids = map.placeTowns(towns);
		const std::vector<int> exp_ids{ 0, 12, 8};
		ASSERT_EQ(ids.size(), exp_ids.size());
		for (size_t i = 0; i < towns.size(); ++i) {
			EXPECT_EQ(ids[i], exp_ids[i]);
		}
	}

	{
		const int players = 4;
		DimensionMap::SizeMap size = DimensionMap::detect(players);
		GameMap map(size);
		ASSERT_EQ(map.getRow(), size.x);
		ASSERT_EQ(map.getColumn(), size.y);
		EXPECT_TRUE(map.getMapJson().empty());
		auto towns = DimensionMap::placeTowns(players, size);
		ASSERT_EQ(towns.size(), players);
		auto ids = map.placeTowns(towns);
		const std::vector<int> exp_ids{ 0, 20, 4 , 24};
		ASSERT_EQ(ids.size(), exp_ids.size());
		for (size_t i = 0; i < towns.size(); ++i) {
			EXPECT_EQ(ids[i], exp_ids[i]);
		}
	}
}

TEST(GameMapTest, BuildWay) {
	const int players = 3;
	DimensionMap::SizeMap size = DimensionMap::detect(players);
	GameMap map(size);
	ASSERT_EQ(map.getRow(), size.x);
	ASSERT_EQ(map.getColumn(), size.y);
	EXPECT_TRUE(map.getMapJson().empty());
	auto towns = DimensionMap::placeTowns(players, size);
	ASSERT_EQ(towns.size(), players);
	auto ids = map.placeTowns(towns);
	const std::vector<int> exp_ids{ 0, 12, 8 };
	ASSERT_EQ(ids.size(), exp_ids.size());
	for (size_t i = 0; i < towns.size(); ++i) {
		EXPECT_EQ(ids[i], exp_ids[i]);
	}
	auto way = map.buildWay(11,6);
	std::cout << "way:" << std::endl;
	for (auto& node : way) {
		std::cout << node << std::endl;
	}
}