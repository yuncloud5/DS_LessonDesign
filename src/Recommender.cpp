#include "Recommender.h"
#include "FriendsSystem.h"
#include "UserProfile.h"
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

vector<string> Recommender::recommendItems(
    const UserProfile &user,
    const std::unordered_map<std::string, std::unordered_set<std::string>> &itemitemIds)
{

    vector<string> recommendations;
    unordered_map<string, double> scores;
    auto userRatings = user.getItemRatings();

    // 遍历用户评分的物品
    for (const auto &entry : userRatings)
    {
        const std::string &item = entry.first;
        double rating = entry.second;
        // cout << "评分：" << item << " ->" << rating << endl;

        if (itemitemIds.find(item) != itemitemIds.end())
        {
            for (const auto &relatedItem : itemitemIds.at(item))

            {
                scores[relatedItem] += rating; // 评分加权
            }

            /* code */
        }

        // 评分大于等于4才推荐
        /*
        if (itemitemIds.find(item) != itemitemIds.end())
        {
            for (const auto &relatedItem : itemitemIds.at(item))
            {
                recommendations.push_back(relatedItem);
            }
        }
        */
    }

    vector<pair<string, double>> sortedScores(scores.begin(), scores.end());

    selectionSort(sortedScores);
    for (const auto &entry : sortedScores)
    {
        recommendations.push_back(entry.first);
    }

    return recommendations;
}

// 基于用户的协同过滤,计算用户之间的相似度
double Recommender::calculateSimilarity(
    const unordered_map<string, double> &user1Ratings,
    const unordered_map<string, double> &user2Ratings)
{
    double dotProduct = 0.0, norm1 = 0.0, norm2 = 0.0; // dotProduct:点积，norm：评分向量范数平方

    // 计算点积
    for (const auto &[item, itemRatings] : user1Ratings) // 遍历用户1的物品
    {
        if (user2Ratings.find(item) != user2Ratings.end())
        {
            dotProduct += itemRatings * user2Ratings.at(item);
        }
        norm1 += itemRatings * itemRatings;
    }

    for (const auto &[item, itemRatings] : user2Ratings) // 遍历用户2的物品，计算范数
    {
        norm2 += itemRatings * itemRatings;
    }

    if (norm1 == 0 || norm2 == 0)
    { // 其中至少一个用户没有评分记录
        return 0.0;
    }

    return dotProduct / (sqrt(norm1) * sqrt(norm2));
}

// 基于用户的协同过滤生成推荐
vector<string> Recommender::recommendItemsByCF(
    const UserProfile &currentUser,
    const unordered_map<string, unordered_map<string, double>> &userItemRatings,
    int topN)
{
    unordered_map<string, double> similarityScores;
    const auto &currentUserRatings = userItemRatings.at(currentUser.getUserId());

    // 计算用户相似度
    // cout << "Calculating similarity for user: " << currentUser.getUserId() << endl;
    for (const auto &[otherUser, ratings] : userItemRatings)
    {
        if (otherUser != currentUser.getUserId())
        {
            double similarity = calculateSimilarity(currentUserRatings, ratings);
            similarityScores[otherUser] = similarity;
            // cout << "Similarity with " << otherUser << ": " << similarity << endl;
        }
    }

    // 查找相似用户的评分并推荐物品
    unordered_map<string, double> recommendItems;
    for (const auto &[otherUser, similarity] : similarityScores)
    {
        if (similarity > 0)
        {
            // cout << "Using ratings from user " << otherUser << " (similarity: " << similarity << ") to recommend:" << endl;
            for (const auto &[itemId, rating] : userItemRatings.at(otherUser))
            {
                if (currentUserRatings.find(itemId) == currentUserRatings.end())
                {
                    recommendItems[itemId] += rating * similarity;
                    // cout << "  Item: " << itemId << ", Weighted Rating: " << rating * similarity << endl;
                }
            }
        }
    }

    // 返回推荐排序后前N个物品
    vector<pair<string, double>> sortedItems(recommendItems.begin(), recommendItems.end());
    selectionSort(sortedItems);

    // cout << "Final sorted recommendations:" << endl;
    /* 展示推荐物品和评分
    for (const auto &[itemId, score] : sortedItems)
    {
        cout << "  Item: " << itemId << ", Final Score: " << score << endl;
    }
    */

    vector<string> recommendations;
    for (int i = 0; i < min(topN, (int)sortedItems.size()); i++)
    { // 若不满足N个物品的数量，取已有的物品
        recommendations.push_back(sortedItems[i].first);
    }

    return recommendations;
}

// 选择排序
void Recommender::selectionSort(std::vector<std::pair<std::string, double>> &items)
{ // pair组合存储物品ID和评分
    for (int i = 0; i < items.size() - 1; i++)
    {
        int maxIdx = i;
        for (int j = i + 1; j < items.size(); j++)
        {
            if (items[j].second > items[maxIdx].second)
            { // 按评分降序排序
                maxIdx = j;
            }
        }
        std::swap(items[i], items[maxIdx]); // 交换最大值到当前位置
    }
}
