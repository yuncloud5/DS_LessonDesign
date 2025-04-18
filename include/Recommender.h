#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "UserProfile.h"

class Recommender
{
public:
    // 根据用户画像和物品生成推荐结果
    std::vector<std::string> recommendItems(
        const UserProfile &user,
        const std::unordered_map<std::string, std::unordered_set<std::string>> &itemTags);

    // 利用协同过滤算法生成，基于用户的协同过滤（collaborative filtering）
    std::vector<std::string> recommendItemsByCF(
        const UserProfile &user,
        const std::unordered_map<std::string, std::unordered_map<std::string, double>> &userItemRatings,
        int topN = 5 // 默认返回排名前五的物品
    );

private:
    // 用户相似度计算
    double calculateSimilarity(
        const std::unordered_map<std::string, double> &user1Ratings,
        const std::unordered_map<std::string, double> &user2Ratings);

    void selectionSort(std::vector<std::pair<std::string, double>> &items);
};