#include "UserProfile.h"

// 获取用户ID
string UserProfile::getUserId() const
{
    return userId;
}

// 设置用户ID
void UserProfile::setUserId(const string &userId)
{
    this->userId = userId;
}

// 添加用户偏好兴趣，及偏好程度
void UserProfile::addInterest(std::string itemId, double rating)
{
    // 如果物品已存在，更新评分
    if (itemRatings.find(itemId) != itemRatings.end())
    {
        totalRating -= itemRatings[itemId];
    }
    else
    { // 如果物品不存在，添加评分
        ratingCount++;
    }
    itemRatings[itemId] = rating;
    totalRating += rating;
}

// 更新兴趣评分
void UserProfile::updateInterest(std::string itemId, double rating)
{
    addInterest(itemId, rating);
}

// 删除兴趣评分
void UserProfile::removeInterest(std::string itemId)
{
    // 物品存在
    if (itemRatings.find(itemId) != itemRatings.end())
    {
        totalRating -= itemRatings[itemId];
        itemRatings.erase(itemId); // 删除评分
        ratingCount--;             // 更新评分记录数
    }
}

// 更新用户偏好标签
void UserProfile::updatePreferences(const std::unordered_set<std::string> &tags)
{
    preferredTags.insert(tags.begin(), tags.end());
}

double UserProfile::getAverageRating() const
{
    return ratingCount > 0 ? totalRating / ratingCount : 0.0;
}

// 设置用户评分记录
void UserProfile::setItemRatings(const unordered_map<string, double> &ratings)
{
    itemRatings = ratings;
    totalRating = 0;
    ratingCount = 0;

    for (const auto &pair : itemRatings)
    {
        totalRating += pair.second;
        ratingCount++;
    }
}
