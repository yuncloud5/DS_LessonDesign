#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "UserProfile.h"

class Recommender
{
public:
    // �����û��������Ʒ�����Ƽ����
    std::vector<std::string> recommendItems(
        const UserProfile &user,
        const std::unordered_map<std::string, std::unordered_set<std::string>> &itemTags);

    // ����Эͬ�����㷨���ɣ������û���Эͬ���ˣ�collaborative filtering��
    std::vector<std::string> recommendItemsByCF(
        const UserProfile &user,
        const std::unordered_map<std::string, std::unordered_map<std::string, double>> &userItemRatings,
        int topN = 5 // Ĭ�Ϸ�������ǰ�����Ʒ
    );

private:
    // �û����ƶȼ���
    double calculateSimilarity(
        const std::unordered_map<std::string, double> &user1Ratings,
        const std::unordered_map<std::string, double> &user2Ratings);

    void selectionSort(std::vector<std::pair<std::string, double>> &items);
};