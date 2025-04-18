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
    cout << "用户推荐系统展示" << endl;
    cout << "=============================" << endl;
    cout << "1. 基于用户画像推荐物品" << endl;
    cout << "2. 基于协同过滤推荐物品" << endl;
    cout << "3. 切换用户或新建用户" << endl;
    cout << "4. 好友管理" << endl;
    cout << "5. 退出" << endl;
    cout << "=============================" << endl;
}

void inputUserPreferences(UserProfile &user)
{
    cout << "请输入你的喜好及程度（格式：物品名 评分,评分在0~5之间,输入'done'结束):" << endl;
    cout << "示例: 音乐 4.5 " << endl;

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
            cout << "无效的评分，请输入一个 0.0 到 5.0 的数字。" << endl;
            continue;
        }

        user.addInterest(itemId, rating);
        hasInput = true;
    }

    if (!hasInput)
    {
        cout << "未输入任何偏好数据，用户画像仍为空！" << endl;
    }
}

void handleFriendManagement(FriendsSystem &friendsSystem, int currentUserId)
{
    int choice;
    do
    {
        friendsSystem.showMenu(currentUserId);
        cout << "请输入你的选择: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "无效输入，请重新选择！" << endl;
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            int friendId;
            cout << "请输入要添加的好友ID: ";
            cin >> friendId;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "无效的好友ID,请重新输入！" << endl;
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
            cout << "返回主菜单..." << endl;
            return;
        default:
            cout << "无效选择，请重新输入！" << endl;
        }
    } while (true);
}

int main()
{

    // 初始化用户画像数据
    unordered_map<string, unordered_map<string, double>> userItemRatings = {
        {"user1", {{"音乐", 4.5}, {"美食", 3.0}, {"电影", 5.0}, {"运动", 2.5}}},
        {"user2", {{"音乐", 4.0}, {"美食", 4.5}, {"体育", 3.5}, {"科技", 4.1}}},
        {"user3", {{"电影", 4.0}, {"体育", 4.5}, {"美食", 5.0}, {"科技", 3.3}}},
        {"user4", {{"音乐", 5.0}, {"电影", 4.5}, {"小说", 4.0}, {"健身", 3.5}}},
        {"user5", {{"美食", 4.0}, {"电影", 3.5}, {"综艺", 4.5}, {"心理", 4.0}}},
        {"user6", {{"篮球", 5.0}, {"足球", 4.5}, {"游泳", 4.0}, {"健身", 4.5}}},
        {"user7", {{"小说", 4.5}, {"散文", 4.0}, {"诗歌", 3.5}, {"娱乐", 4.0}}},
        {"user8", {{"科技", 5.0}, {"电影", 4.0}, {"音乐", 3.5}, {"心理", 4.5}}},
        {"user9", {{"美食", 4.5}, {"西餐", 4.0}, {"烤肉", 3.5}, {"甜点", 4.5}}},
        {"user10", {{"电影", 5.0}, {"综艺", 4.5}, {"音乐会", 4.0}, {"娱乐", 4.5}}}};

    UserProfile user;
    string userId;
    Recommender recommender;
    unordered_map<int, string> userIdToName;
    int nextUserId = 1; // 自动生成用户 ID

    cout << "请输入你的用户id: ";
    cin >> userId;
    user.setUserId(userId);
    FriendsSystem friendsSystem(stoi(userId), "默认用户");

    int choice;
    do
    {
        menu();
        cout << "请输入你需要的功能: ";
        cin >> choice;

        switch (choice)
        {
        case 1: // 基于用户画像推荐物品
        {
            if (user.getItemRatings().empty())
            {
                cout << "用户画像为空，请先输入偏好数据！" << endl;
                inputUserPreferences(user);
                userItemRatings[user.getUserId()] = user.getItemRatings(); // 更新全局用户画像数据
            }

            if (user.getItemRatings().empty())
            {
                cout << "用户画像仍为空，无法生成推荐！" << endl;
                break;
            }

            std::unordered_map<std::string, std::unordered_set<std::string>> itemitemIds = {
                {"音乐", {"流行", "民谣", "R&B"}},
                {"美食", {"西餐", "烤肉", "甜点"}},
                {"运动", {"篮球", "足球", "游泳"}},
                {"娱乐", {"电影", "综艺", "音乐会"}},
                {"故事", {"小说", "散文", "诗歌"}},
                {"健康", {"健身", "饮食", "心理"}},
                {"电影", {"娱乐", "故事"}},
                {"体育", {"运动", "健康"}},
            };

            // 调试推荐器输入
            /*
            cout << "调试: 接收到的用户画像数据:" << endl;
            for (const auto &entry : user.getItemRatings())
            {
                cout << "物品: " << entry.first << ", 评分: " << entry.second << endl;
            }
            */
            auto recomByUserProfile = recommender.recommendItems(user, itemitemIds);
            if (recomByUserProfile.empty())
            {
                cout << "没有找到推荐物品，请检查用户画像数据！" << endl;
            }
            else
            {
                cout << "推荐物品(基于用户画像): " << endl;
                for (const auto &item : recomByUserProfile)
                {
                    cout << item << " ";
                }
                cout << endl;
            }
            break;
        }

        case 2: // 基于协同过滤推荐物品

        {
            int topN = 5;
            if (userItemRatings.find(user.getUserId()) == userItemRatings.end())
            {
                cout << "当前用户没有评分数据，请先输入偏好数据！" << endl;
                inputUserPreferences(user);
                userItemRatings[user.getUserId()] = user.getItemRatings();
                break;
            }

            cout << "请输入推荐数量（默认 5): ";
            cin >> topN;
            if (cin.fail() || topN <= 0)
            {
                topN = 5;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            auto recommendations = recommender.recommendItemsByCF(user, userItemRatings, topN);
            cout << "推荐物品(基于用户的协同过滤):" << endl;
            for (const auto &item : recommendations)
            {
                cout << item << " ";
            }
            cout << endl;
            break;
        }

        case 3: // 切换用户或新建用户
        {
            string userName;

            cout << "请输入用户名: ";
            cin >> userName;

            // 检查名字是否已存在（名字可以重复）
            auto it = find_if(userIdToName.begin(), userIdToName.end(),
                              [&userName](const pair<int, string> &entry)
                              {
                                  return entry.second == userName;
                              });

            if (it != userIdToName.end())
            {

                int existingUserId = it->first;
                cout << "用户已存在，加载用户数据..." << endl;
                user.setUserId(to_string(existingUserId));
                user.setItemRatings(userItemRatings[to_string(existingUserId)]);
            }
            else
            {
                int newUserId = friendsSystem.generateUserId();
                user.setUserId(to_string(newUserId));
                userIdToName[newUserId] = userName;
                cout << "新用户创建成功,用户ID为: " << newUserId << endl;
                inputUserPreferences(user);
                userItemRatings[user.getUserId()] = user.getItemRatings();
                friendsSystem.initializeUser(newUserId, userName);
            }

            cout << "用户切换或新建完成！" << endl;
            break;
        }

        case 4: // 好友管理
        {
            handleFriendManagement(friendsSystem, stoi(user.getUserId()));
            break;
        }

        case 5:
        {
            cout << "退出系统" << endl;
            return 0;
        }

        default:
            cout << "无效的选项，请重新选择。" << endl;
            break;
        }
    } while (true);

    return 0;
}