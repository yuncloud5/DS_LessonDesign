#include "UserProfile.h"

// ��ȡ�û�ID
string UserProfile::getUserId() const
{
    return userId;
}

// �����û�ID
void UserProfile::setUserId(const string &userId)
{
    this->userId = userId;
}

// ����û�ƫ����Ȥ����ƫ�ó̶�
void UserProfile::addInterest(std::string itemId, double rating)
{
    // �����Ʒ�Ѵ��ڣ���������
    if (itemRatings.find(itemId) != itemRatings.end())
    {
        totalRating -= itemRatings[itemId];
    }
    else
    { // �����Ʒ�����ڣ��������
        ratingCount++;
    }
    itemRatings[itemId] = rating;
    totalRating += rating;
}

// ������Ȥ����
void UserProfile::updateInterest(std::string itemId, double rating)
{
    addInterest(itemId, rating);
}

// ɾ����Ȥ����
void UserProfile::removeInterest(std::string itemId)
{
    // ��Ʒ����
    if (itemRatings.find(itemId) != itemRatings.end())
    {
        totalRating -= itemRatings[itemId];
        itemRatings.erase(itemId); // ɾ������
        ratingCount--;             // �������ּ�¼��
    }
}

// �����û�ƫ�ñ�ǩ
void UserProfile::updatePreferences(const std::unordered_set<std::string> &tags)
{
    preferredTags.insert(tags.begin(), tags.end());
}

double UserProfile::getAverageRating() const
{
    return ratingCount > 0 ? totalRating / ratingCount : 0.0;
}

// �����û����ּ�¼
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
