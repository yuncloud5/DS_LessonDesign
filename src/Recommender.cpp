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

    // �����û����ֵ���Ʒ
    for (const auto &entry : userRatings)
    {
        const std::string &item = entry.first;
        double rating = entry.second;
        // cout << "���֣�" << item << " ->" << rating << endl;

        if (itemitemIds.find(item) != itemitemIds.end())
        {
            for (const auto &relatedItem : itemitemIds.at(item))

            {
                scores[relatedItem] += rating; // ���ּ�Ȩ
            }

            /* code */
        }

        // ���ִ��ڵ���4���Ƽ�
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

// �����û���Эͬ����,�����û�֮������ƶ�
double Recommender::calculateSimilarity(
    const unordered_map<string, double> &user1Ratings,
    const unordered_map<string, double> &user2Ratings)
{
    double dotProduct = 0.0, norm1 = 0.0, norm2 = 0.0; // dotProduct:�����norm��������������ƽ��

    // ������
    for (const auto &[item, itemRatings] : user1Ratings) // �����û�1����Ʒ
    {
        if (user2Ratings.find(item) != user2Ratings.end())
        {
            dotProduct += itemRatings * user2Ratings.at(item);
        }
        norm1 += itemRatings * itemRatings;
    }

    for (const auto &[item, itemRatings] : user2Ratings) // �����û�2����Ʒ�����㷶��
    {
        norm2 += itemRatings * itemRatings;
    }

    if (norm1 == 0 || norm2 == 0)
    { // ��������һ���û�û�����ּ�¼
        return 0.0;
    }

    return dotProduct / (sqrt(norm1) * sqrt(norm2));
}

// �����û���Эͬ���������Ƽ�
vector<string> Recommender::recommendItemsByCF(
    const UserProfile &currentUser,
    const unordered_map<string, unordered_map<string, double>> &userItemRatings,
    int topN)
{
    unordered_map<string, double> similarityScores;
    const auto &currentUserRatings = userItemRatings.at(currentUser.getUserId());

    // �����û����ƶ�
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

    // ���������û������ֲ��Ƽ���Ʒ
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

    // �����Ƽ������ǰN����Ʒ
    vector<pair<string, double>> sortedItems(recommendItems.begin(), recommendItems.end());
    selectionSort(sortedItems);

    // cout << "Final sorted recommendations:" << endl;
    /* չʾ�Ƽ���Ʒ������
    for (const auto &[itemId, score] : sortedItems)
    {
        cout << "  Item: " << itemId << ", Final Score: " << score << endl;
    }
    */

    vector<string> recommendations;
    for (int i = 0; i < min(topN, (int)sortedItems.size()); i++)
    { // ��������N����Ʒ��������ȡ���е���Ʒ
        recommendations.push_back(sortedItems[i].first);
    }

    return recommendations;
}

// ѡ������
void Recommender::selectionSort(std::vector<std::pair<std::string, double>> &items)
{ // pair��ϴ洢��ƷID������
    for (int i = 0; i < items.size() - 1; i++)
    {
        int maxIdx = i;
        for (int j = i + 1; j < items.size(); j++)
        {
            if (items[j].second > items[maxIdx].second)
            { // �����ֽ�������
                maxIdx = j;
            }
        }
        std::swap(items[i], items[maxIdx]); // �������ֵ����ǰλ��
    }
}
