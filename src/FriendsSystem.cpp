#include <FriendsSystem.h>
#include <iostream>
#include <cstring>

using namespace std;

FriendsSystem::FriendsSystem(int currentUserId, const std::string &userName)
{
    // 初始化用户
    initializeUser(currentUserId, userName);
}

int FriendsSystem::generateUserId()
{
    static int newUserId = 1;
    while (findUserIndex(newUserId) != -1)
    {
        newUserId++;
    }
    return newUserId++;
}

int FriendsSystem::findUserIndex(int userId)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].id == userId)
        {
            return i;
        }
    }
    return -1;
}
// 判断 二人是否为好友关系
bool FriendsSystem::isFriend(int userId1, int userId2)
{
    int user1Index = findUserIndex(userId1);
    if (user1Index == -1)
        return false;

    for (int i = 0; i < users[user1Index].friendCount; i++)
    {
        if (users[user1Index].friends[i] == userId2)
        {
            return true;
        }
    }
    return false;
}
// 加好友
void FriendsSystem::addFriend(int userId1, int userId2)
{
    int user1Index = findUserIndex(userId1);
    int user2Index = findUserIndex(userId2);
    if (user1Index == -1 || user2Index == -1)
    {
        printf("用户不存在,请检查id\n");
        return;
    }
    if (isFriend(userId1, userId2))
    {
        printf("'%s'已经是您的好友，无需重复添加\n", users[user2Index].name);
        return;
    }
    users[user1Index].friends[users[user1Index].friendCount++] = userId2;
    users[user2Index].friends[users[user2Index].friendCount++] = userId1;
    printf("好友关系建立成功\n");
    printf("已添加'%s'为您的好友\n", users[user2Index].name);
}

// 从用户的好友列表中删除好友
void FriendsSystem::removeFriendFromUser(int userIndex, int friendIdToRemove)
{
    int *friends = users[userIndex].friends;
    int *count = &users[userIndex].friendCount;

    for (int i = 0; i < *count; i++)
    {
        if (friends[i] == friendIdToRemove)
        {
            // 将后面的元素前移
            for (int j = i; j < *count - 1; j++)
            {
                friends[j] = friends[j + 1];
            }
            (*count)--;
            break;
        }
    }
}
// 删除双方的好友关系
void FriendsSystem::deleteFriendship(int userId1, int userId2)
{
    int user1Index = findUserIndex(userId1);
    int user2Index = findUserIndex(userId2);

    if (user1Index == -1 || user2Index == -1)
    {
        printf("错误：用户不存在,请检查ID\n");
        return;
    }

    // 从用户1的好友列表中删除用户2
    removeFriendFromUser(user1Index, userId2);

    // 从用户2的好友列表中删除用户1
    removeFriendFromUser(user2Index, userId1);

    printf("好友关系已解除\n");
}
// 显示用户的好友列表
void FriendsSystem::displayUserFriends(int userId)
{
    int userIndex = findUserIndex(userId);
    if (userIndex == -1)
    {
        printf("用户不存在\n");
        return;
    }

    printf("\n=== 好友列表 ===\n");
    printf("ID\t姓名\n");
    printf("----------------\n");

    for (int i = 0; i < users[userIndex].friendCount; i++)
    {
        int friendId = users[userIndex].friends[i];
        int friendIndex = findUserIndex(friendId);
        if (friendIndex != -1)
        {
            cout << users[friendIndex].id << "\t" << users[friendIndex].name << endl;
            ;
        }
    }
}
// 删除好友的主操作
void FriendsSystem::deleteFriend(int currentUserId)
{
    int currentUserIndex = findUserIndex(currentUserId);
    if (currentUserIndex == -1)
    {
        printf("当前用户不存在\n");
        return;
    }

    if (users[currentUserIndex].friendCount == 0)
    {
        printf("您当前没有好友可以删除\n");
        return;
    }
    displayUserFriends(currentUserId);

    int friendIdToDelete;
    printf("\n请输入要删除的好友ID: ");
    scanf("%d", &friendIdToDelete);
    // 检查是否真的是好友
    if (!isFriend(currentUserId, friendIdToDelete))
    {
        printf("错误：该用户不是您的好友\n");
        return;
    }
    // 获取好友信息
    int friendIndex = findUserIndex(friendIdToDelete);
    if (friendIndex == -1)
    {
        printf("错误：好友不存在\n");
        return;
    }
    deleteFriendship(currentUserId, friendIdToDelete);
    // 显示更新后的好友列表
    printf("\n删除后的好友列表:\n");
    displayUserFriends(currentUserId);
}

// 推荐共同好友相加

void FriendsSystem::recommendCommonFriends(int currentUserId)
{
    int currentUserIndex = findUserIndex(currentUserId);
    if (currentUserIndex == -1)
    {
        printf("用户不存在\n");
        return;
    }
    printf("\n=== 可能认识的人 ===\n");
    int i, j;
    for (i = 0; i < users.size(); i++)
    {
        if (users[i].id == currentUserId || isFriend(currentUserId, users[i].id))
        {
            continue;
        }
        int commonCount = 0;
        for (j = 0; j < users[currentUserIndex].friendCount; j++)
        {
            int friendId = users[currentUserIndex].friends[j];
            if (isFriend(users[i].id, friendId))
            {
                commonCount++;
            }
        }
        if (commonCount > 0)
        {
            printf("ID: %d, 姓名: %s, 共同好友数: %d\n", users[i].id, users[i].name, commonCount);
        }
    }
}

// 主菜单
void FriendsSystem::showMenu(int currentUserId)
{
    printf("\n好友管理系统 (当前用户ID: %d)\n", currentUserId);
    printf("1. 添加好友\n");
    printf("2. 显示所有好友\n");
    printf("3. 删除好友\n");
    printf("4. 推荐可能认识的人\n");
    printf("5. 退出系统\n");
    printf("请选择操作: ");
}
// 初始化测试数据
/*void FriendsSystem::initializeTestData()
{
    users.push_back({1, "张三", {2, 3}, 2});
    users.push_back({2, "李四", {1, 4}, 2});
    users.push_back({3, "王五", {1, 4}, 2});
    users.push_back({4, "赵六", {2, 3, 5}, 3});
    users.push_back({5, "钱七", {4}, 1});
}*/
void FriendsSystem::initializeUser(int userId, const std::string &userName)
{
    if (findUserIndex(userId) != -1)
    {
        printf("用户ID %d 已经存在，无法重复初始化\n", userId);
        return;
    }

    User newUser;
    newUser.id = userId;
    strncpy(newUser.name, userName.c_str(), sizeof(newUser.name) - 1);
    newUser.name[sizeof(newUser.name) - 1] = '\0';
    newUser.friendCount = 0;

    users.push_back(newUser);
    printf("用户ID %d (%s) 初始化成功\n", userId, userName.c_str());
}
