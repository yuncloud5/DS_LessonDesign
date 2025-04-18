#include <iostream>
#include "Recommender.h"
#include "UserProfile.h"
#include "FriendsSystem.h"
#include <limits>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

void menu()
{
    cout << "�û��Ƽ�ϵͳչʾ" << endl;
    cout << "=============================" << endl;
    cout << "1. �����û������Ƽ���Ʒ" << endl;
    cout << "2. ����Эͬ�����Ƽ���Ʒ" << endl;
    cout << "3. �л��û����½��û�" << endl;
    cout << "4. ���ѹ���" << endl;
    cout << "5. �˳�" << endl;
    cout << "=============================" << endl;
}

void inputUserPreferences(UserProfile &user)
{
    cout << "���������ϲ�ü��̶ȣ���ʽ����Ʒ�� ����,������0~5֮��,����'done'����):" << endl;
    cout << "ʾ��: ���� 4.5 " << endl;

    string itemId;
    double rating;
    bool hasInput = false;

    while (true)
    {
        cin >> itemId;
        if (itemId == "done")
            break;
        cin >> rating;

        if (cin.fail() || rating < 0.0 || rating > 5.0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "��Ч�����֣�������һ�� 0.0 �� 5.0 �����֡�" << endl;
            continue;
        }

        user.addInterest(itemId, rating);
        hasInput = true;
    }

    if (!hasInput)
    {
        cout << "δ�����κ�ƫ�����ݣ��û�������Ϊ�գ�" << endl;
    }
}

void handleFriendManagement(FriendsSystem &friendsSystem, int currentUserId)
{
    int choice;
    do
    {
        friendsSystem.showMenu(currentUserId);
        cout << "���������ѡ��: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "��Ч���룬������ѡ��" << endl;
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            int friendId;
            cout << "������Ҫ��ӵĺ���ID: ";
            cin >> friendId;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "��Ч�ĺ���ID,���������룡" << endl;
                break;
            }
            friendsSystem.addFriend(currentUserId, friendId);
            break;
        }
        case 2:
            friendsSystem.displayUserFriends(currentUserId);
            break;
        case 3:
            friendsSystem.deleteFriend(currentUserId);
            break;
        case 4:
            friendsSystem.recommendCommonFriends(currentUserId);
            break;
        case 5:
            cout << "�������˵�..." << endl;
            return;
        default:
            cout << "��Чѡ�����������룡" << endl;
        }
    } while (true);
}

int main()
{

    // ��ʼ���û���������
    unordered_map<string, unordered_map<string, double>> userItemRatings = {
        {"user1", {{"����", 4.5}, {"��ʳ", 3.0}, {"��Ӱ", 5.0}, {"�˶�", 2.5}}},
        {"user2", {{"����", 4.0}, {"��ʳ", 4.5}, {"����", 3.5}, {"�Ƽ�", 4.1}}},
        {"user3", {{"��Ӱ", 4.0}, {"����", 4.5}, {"��ʳ", 5.0}, {"�Ƽ�", 3.3}}},
        {"user4", {{"����", 5.0}, {"��Ӱ", 4.5}, {"С˵", 4.0}, {"����", 3.5}}},
        {"user5", {{"��ʳ", 4.0}, {"��Ӱ", 3.5}, {"����", 4.5}, {"����", 4.0}}},
        {"user6", {{"����", 5.0}, {"����", 4.5}, {"��Ӿ", 4.0}, {"����", 4.5}}},
        {"user7", {{"С˵", 4.5}, {"ɢ��", 4.0}, {"ʫ��", 3.5}, {"����", 4.0}}},
        {"user8", {{"�Ƽ�", 5.0}, {"��Ӱ", 4.0}, {"����", 3.5}, {"����", 4.5}}},
        {"user9", {{"��ʳ", 4.5}, {"����", 4.0}, {"����", 3.5}, {"���", 4.5}}},
        {"user10", {{"��Ӱ", 5.0}, {"����", 4.5}, {"���ֻ�", 4.0}, {"����", 4.5}}}};

    UserProfile user;
    string userId;
    Recommender recommender;
    unordered_map<int, string> userIdToName;
    int nextUserId = 1; // �Զ������û� ID

    cout << "����������û�id: ";
    cin >> userId;
    user.setUserId(userId);
    FriendsSystem friendsSystem(stoi(userId), "Ĭ���û�");

    int choice;
    do
    {
        menu();
        cout << "����������Ҫ�Ĺ���: ";
        cin >> choice;

        switch (choice)
        {
        case 1: // �����û������Ƽ���Ʒ
        {
            if (user.getItemRatings().empty())
            {
                cout << "�û�����Ϊ�գ���������ƫ�����ݣ�" << endl;
                inputUserPreferences(user);
                userItemRatings[user.getUserId()] = user.getItemRatings(); // ����ȫ���û���������
            }

            if (user.getItemRatings().empty())
            {
                cout << "�û�������Ϊ�գ��޷������Ƽ���" << endl;
                break;
            }

            std::unordered_map<std::string, std::unordered_set<std::string>> itemitemIds = {
                {"����", {"����", "��ҥ", "R&B"}},
                {"��ʳ", {"����", "����", "���"}},
                {"�˶�", {"����", "����", "��Ӿ"}},
                {"����", {"��Ӱ", "����", "���ֻ�"}},
                {"����", {"С˵", "ɢ��", "ʫ��"}},
                {"����", {"����", "��ʳ", "����"}},
                {"��Ӱ", {"����", "����"}},
                {"����", {"�˶�", "����"}},
            };

            // �����Ƽ�������
            /*
            cout << "����: ���յ����û���������:" << endl;
            for (const auto &entry : user.getItemRatings())
            {
                cout << "��Ʒ: " << entry.first << ", ����: " << entry.second << endl;
            }
            */
            auto recomByUserProfile = recommender.recommendItems(user, itemitemIds);
            if (recomByUserProfile.empty())
            {
                cout << "û���ҵ��Ƽ���Ʒ�������û��������ݣ�" << endl;
            }
            else
            {
                cout << "�Ƽ���Ʒ(�����û�����): " << endl;
                for (const auto &item : recomByUserProfile)
                {
                    cout << item << " ";
                }
                cout << endl;
            }
            break;
        }

        case 2: // ����Эͬ�����Ƽ���Ʒ

        {
            int topN = 5;
            if (userItemRatings.find(user.getUserId()) == userItemRatings.end())
            {
                cout << "��ǰ�û�û���������ݣ���������ƫ�����ݣ�" << endl;
                inputUserPreferences(user);
                userItemRatings[user.getUserId()] = user.getItemRatings();
                break;
            }

            cout << "�������Ƽ�������Ĭ�� 5): ";
            cin >> topN;
            if (cin.fail() || topN <= 0)
            {
                topN = 5;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            auto recommendations = recommender.recommendItemsByCF(user, userItemRatings, topN);
            cout << "�Ƽ���Ʒ(�����û���Эͬ����):" << endl;
            for (const auto &item : recommendations)
            {
                cout << item << " ";
            }
            cout << endl;
            break;
        }

        case 3: // �л��û����½��û�
        {
            string userName;

            cout << "�������û���: ";
            cin >> userName;

            // ��������Ƿ��Ѵ��ڣ����ֿ����ظ���
            auto it = find_if(userIdToName.begin(), userIdToName.end(),
                              [&userName](const pair<int, string> &entry)
                              {
                                  return entry.second == userName;
                              });

            if (it != userIdToName.end())
            {

                int existingUserId = it->first;
                cout << "�û��Ѵ��ڣ������û�����..." << endl;
                user.setUserId(to_string(existingUserId));
                user.setItemRatings(userItemRatings[to_string(existingUserId)]);
            }
            else
            {
                int newUserId = friendsSystem.generateUserId();
                user.setUserId(to_string(newUserId));
                userIdToName[newUserId] = userName;
                cout << "���û������ɹ�,�û�IDΪ: " << newUserId << endl;
                inputUserPreferences(user);
                userItemRatings[user.getUserId()] = user.getItemRatings();
                friendsSystem.initializeUser(newUserId, userName);
            }

            cout << "�û��л����½���ɣ�" << endl;
            break;
        }

        case 4: // ���ѹ���
        {
            handleFriendManagement(friendsSystem, stoi(user.getUserId()));
            break;
        }

        case 5:
        {
            cout << "�˳�ϵͳ" << endl;
            return 0;
        }

        default:
            cout << "��Ч��ѡ�������ѡ��" << endl;
            break;
        }
    } while (true);

    return 0;
}