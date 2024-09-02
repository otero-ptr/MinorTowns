#include "User.h"
#include <gtest\gtest.h>

TEST(UserTest, Create) {
	User user("user1", "127.0.0.1");
	EXPECT_EQ(user.getUsername(), "user1");
	EXPECT_EQ(user.getLocation(), Location::MENU);
	EXPECT_FALSE(user.getUUID().empty());
	EXPECT_TRUE(user.getUUIDLocation().empty());
}

TEST(UserTest, ChangeLocation) {
	User user("user1", "127.0.0.1");
	user.setLocation(Location::LOBBY, "uuid123");
	EXPECT_EQ(user.getLocation(), Location::LOBBY);
	EXPECT_EQ(user.getUUIDLocation(), "uuid123");
	user.setLocation(Location::GAME, "uuid456");
	EXPECT_EQ(user.getLocation(), Location::GAME);
	EXPECT_EQ(user.getUUIDLocation(), "uuid456");
}

TEST(UserTest, Comparison) {
	std::shared_ptr<User> user1 = std::make_shared<User>("user1", "127.0.0.1");
	std::shared_ptr<User> user2 = std::make_shared<User>("user2", "127.0.0.2");
	EXPECT_FALSE(user1 == user2);
	std::shared_ptr<User> user1_copy = user1;
	EXPECT_TRUE(user1_copy == user1);
}

TEST(UserTest, UserMessagePool) {
	User user("user1", "127.0.0.1");
	EXPECT_TRUE(user.message_pool.empty());
	user.message_pool.push("msg");
	EXPECT_FALSE(user.message_pool.empty());
	auto result = user.message_pool.pop();
	EXPECT_EQ(result, "msg");
	EXPECT_TRUE(user.message_pool.empty());
	user.message_pool.push("msg1");
	user.message_pool.push("msg2");
	user.message_pool.clear();
	EXPECT_TRUE(user.message_pool.empty());	
}