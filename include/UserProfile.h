#pragma once // ��ֹͷ�ļ��ظ�����

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// �û���Ϣ����¼�û���ƫ��
class UserProfile
{
public:
    // �û�ID
    string getUserId() const;
    void setUserId(const string &userId);

    // ����û�ƫ����Ȥ����ƫ�ó̶�
    void addInterest(string itemId, double rating);

    // �����û�ƫ����Ȥ����ƫ�ó̶�
    void updateInterest(string itemId, double rating);

    // ɾ���û�ƫ����Ȥ
    void removeInterest(string itemId);

    // ��ȡ�û�ƫ�����ּ�¼��������ƷID��ƫ������
    const unordered_map<string, double> &getItemRatings() const { return itemRatings; };

    // ��ȡ�û�ƫ�õļ��ϱ�ǩ�����ذ����û�ƫ�õļ���
    const unordered_set<string> &getPreferredTags() const { return preferredTags; };

    // �����û�ƽ��ƫ������
    double getAverageRating() const;

    // �����û�ƫ�ñ�ǩ
    void updatePreferences(const std::unordered_set<std::string> &tags);

    void setItemRatings(const unordered_map<string, double> &itemRatings);

    const vector<string> &getFriends() const;

private:
    string userId;                             // �û�ID
    unordered_map<string, double> itemRatings; // �洢��ƷID�����û�����Ʒ��ƫ������
    unordered_set<string> preferredTags;       // �洢�û�ƫ�õļ��ϱ�ǩ
    unordered_set<int> friends;                // �洢�û��ĺ����б�
    double totalRating = 0.0;                  // �û�ƫ�������ܺ�
    int ratingCount = 0;                       // �û�ƫ�����ּ�¼��
};
