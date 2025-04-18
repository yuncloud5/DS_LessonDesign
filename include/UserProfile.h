#pragma once // 防止头文件重复包含

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// 用户信息，记录用户的偏好
class UserProfile
{
public:
    // 用户ID
    string getUserId() const;
    void setUserId(const string &userId);

    // 添加用户偏好兴趣，及偏好程度
    void addInterest(string itemId, double rating);

    // 更新用户偏好兴趣，及偏好程度
    void updateInterest(string itemId, double rating);

    // 删除用户偏好兴趣
    void removeInterest(string itemId);

    // 获取用户偏好评分记录，返回物品ID及偏好评分
    const unordered_map<string, double> &getItemRatings() const { return itemRatings; };

    // 获取用户偏好的集合标签，返回包含用户偏好的集合
    const unordered_set<string> &getPreferredTags() const { return preferredTags; };

    // 返回用户平均偏好评分
    double getAverageRating() const;

    // 更新用户偏好标签
    void updatePreferences(const std::unordered_set<std::string> &tags);

    void setItemRatings(const unordered_map<string, double> &itemRatings);

    const vector<string> &getFriends() const;

private:
    string userId;                             // 用户ID
    unordered_map<string, double> itemRatings; // 存储物品ID，及用户对物品的偏好评分
    unordered_set<string> preferredTags;       // 存储用户偏好的集合标签
    unordered_set<int> friends;                // 存储用户的好友列表
    double totalRating = 0.0;                  // 用户偏好评分总和
    int ratingCount = 0;                       // 用户偏好评分记录数
};
