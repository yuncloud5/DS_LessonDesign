#include <FriendsSystem.h>
#include <iostream>
#include <cstring>

using namespace std;

FriendsSystem::FriendsSystem(int currentUserId, const std::string &userName)
{
    // ��ʼ���û�
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
// �ж� �����Ƿ�Ϊ���ѹ�ϵ
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
// �Ӻ���
void FriendsSystem::addFriend(int userId1, int userId2)
{
    int user1Index = findUserIndex(userId1);
    int user2Index = findUserIndex(userId2);
    if (user1Index == -1 || user2Index == -1)
    {
        printf("�û�������,����id\n");
        return;
    }
    if (isFriend(userId1, userId2))
    {
        printf("'%s'�Ѿ������ĺ��ѣ������ظ����\n", users[user2Index].name);
        return;
    }
    users[user1Index].friends[users[user1Index].friendCount++] = userId2;
    users[user2Index].friends[users[user2Index].friendCount++] = userId1;
    printf("���ѹ�ϵ�����ɹ�\n");
    printf("�����'%s'Ϊ���ĺ���\n", users[user2Index].name);
}

// ���û��ĺ����б���ɾ������
void FriendsSystem::removeFriendFromUser(int userIndex, int friendIdToRemove)
{
    int *friends = users[userIndex].friends;
    int *count = &users[userIndex].friendCount;

    for (int i = 0; i < *count; i++)
    {
        if (friends[i] == friendIdToRemove)
        {
            // �������Ԫ��ǰ��
            for (int j = i; j < *count - 1; j++)
            {
                friends[j] = friends[j + 1];
            }
            (*count)--;
            break;
        }
    }
}
// ɾ��˫���ĺ��ѹ�ϵ
void FriendsSystem::deleteFriendship(int userId1, int userId2)
{
    int user1Index = findUserIndex(userId1);
    int user2Index = findUserIndex(userId2);

    if (user1Index == -1 || user2Index == -1)
    {
        printf("�����û�������,����ID\n");
        return;
    }

    // ���û�1�ĺ����б���ɾ���û�2
    removeFriendFromUser(user1Index, userId2);

    // ���û�2�ĺ����б���ɾ���û�1
    removeFriendFromUser(user2Index, userId1);

    printf("���ѹ�ϵ�ѽ��\n");
}
// ��ʾ�û��ĺ����б�
void FriendsSystem::displayUserFriends(int userId)
{
    int userIndex = findUserIndex(userId);
    if (userIndex == -1)
    {
        printf("�û�������\n");
        return;
    }

    printf("\n=== �����б� ===\n");
    printf("ID\t����\n");
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
// ɾ�����ѵ�������
void FriendsSystem::deleteFriend(int currentUserId)
{
    int currentUserIndex = findUserIndex(currentUserId);
    if (currentUserIndex == -1)
    {
        printf("��ǰ�û�������\n");
        return;
    }

    if (users[currentUserIndex].friendCount == 0)
    {
        printf("����ǰû�к��ѿ���ɾ��\n");
        return;
    }
    displayUserFriends(currentUserId);

    int friendIdToDelete;
    printf("\n������Ҫɾ���ĺ���ID: ");
    scanf("%d", &friendIdToDelete);
    // ����Ƿ�����Ǻ���
    if (!isFriend(currentUserId, friendIdToDelete))
    {
        printf("���󣺸��û��������ĺ���\n");
        return;
    }
    // ��ȡ������Ϣ
    int friendIndex = findUserIndex(friendIdToDelete);
    if (friendIndex == -1)
    {
        printf("���󣺺��Ѳ�����\n");
        return;
    }
    deleteFriendship(currentUserId, friendIdToDelete);
    // ��ʾ���º�ĺ����б�
    printf("\nɾ����ĺ����б�:\n");
    displayUserFriends(currentUserId);
}

// �Ƽ���ͬ�������

void FriendsSystem::recommendCommonFriends(int currentUserId)
{
    int currentUserIndex = findUserIndex(currentUserId);
    if (currentUserIndex == -1)
    {
        printf("�û�������\n");
        return;
    }
    printf("\n=== ������ʶ���� ===\n");
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
            printf("ID: %d, ����: %s, ��ͬ������: %d\n", users[i].id, users[i].name, commonCount);
        }
    }
}

// ���˵�
void FriendsSystem::showMenu(int currentUserId)
{
    printf("\n���ѹ���ϵͳ (��ǰ�û�ID: %d)\n", currentUserId);
    printf("1. ��Ӻ���\n");
    printf("2. ��ʾ���к���\n");
    printf("3. ɾ������\n");
    printf("4. �Ƽ�������ʶ����\n");
    printf("5. �˳�ϵͳ\n");
    printf("��ѡ�����: ");
}
// ��ʼ����������
/*void FriendsSystem::initializeTestData()
{
    users.push_back({1, "����", {2, 3}, 2});
    users.push_back({2, "����", {1, 4}, 2});
    users.push_back({3, "����", {1, 4}, 2});
    users.push_back({4, "����", {2, 3, 5}, 3});
    users.push_back({5, "Ǯ��", {4}, 1});
}*/
void FriendsSystem::initializeUser(int userId, const std::string &userName)
{
    if (findUserIndex(userId) != -1)
    {
        printf("�û�ID %d �Ѿ����ڣ��޷��ظ���ʼ��\n", userId);
        return;
    }

    User newUser;
    newUser.id = userId;
    strncpy(newUser.name, userName.c_str(), sizeof(newUser.name) - 1);
    newUser.name[sizeof(newUser.name) - 1] = '\0';
    newUser.friendCount = 0;

    users.push_back(newUser);
    printf("�û�ID %d (%s) ��ʼ���ɹ�\n", userId, userName.c_str());
}
