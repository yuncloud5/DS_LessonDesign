#pragma once

#include <vector>
#include <string>

typedef struct
{
    int id;
    char name[50];
    int friends[100];
    int friendCount;
} User;

class FriendsSystem
{
public:
    FriendsSystem(int currentUserId, const std::string &userName);
    void showMenu(int currentUserId);
    void addFriend(int userId1, int userId2);
    void displayUserFriends(int userId);
    void deleteFriend(int currentUserId);
    void recommendCommonFriends(int currentUserId);
    // void initializeTestData();
    void initializeUser(int userId, const std::string &userName);
    int findUserIndex(int userId);
    bool isFriend(int userId1, int userId2);
    int generateUserId();

private:
    std::vector<User> users;

    void removeFriendFromUser(int userIndex, int friendIdToRemove);
    void deleteFriendship(int userId1, int userId2);
};