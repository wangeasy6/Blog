#include <iostream>
#include <set>
#include <algorithm> // ���� std::for_each

using namespace std;

int main() {
    // 1. ���캯��
    std::multiset<int> multiset1; // Ĭ�Ϲ��죬�� multiset
    std::multiset<int> multiset2 = {1, 2, 3, 4, 5}; // ��ʼ���б���
    std::multiset<int> multiset3(multiset2); // ��������
    std::multiset<int> multiset4(std::move(multiset3)); // �ƶ�����
    std::multiset<int> multiset5(multiset2.begin(), multiset2.end()); // ��Χ����

    // 2. ����Ԫ��
    multiset1.insert(5);
    multiset1.insert(2);
    multiset1.insert(7);
    multiset1.insert(5); // �����ظ�Ԫ��
    multiset1.insert(1);

    cout << "multiset1 contains: ";
    for (const auto& elem : multiset1) {
        cout << elem << " ";
    }
    cout << endl;

    // 3. ���Һͼ���
    cout << "Count of 5 in multiset1: " << multiset1.count(5) << endl;
    auto it = multiset1.find(5);
    if (it != multiset1.end()) {
        cout << "Found element: " << *it << endl;
    }

    // 4. ɾ��Ԫ��
    multiset1.erase(it); // ɾ���ҵ���Ԫ��
    cout << "After erasing one 5, multiset1 contains: ";
    for (const auto& elem : multiset1) {
        cout << elem << " ";
    }
    cout << endl;

    multiset1.erase(5); // ɾ������ֵΪ5��Ԫ��
    cout << "After erasing all 5s, multiset1 contains: ";
    for (const auto& elem : multiset1) {
        cout << elem << " ";
    }
    cout << endl;

    // 5. �����͵�����
    cout << "multiset1.size(): " << multiset1.size() << endl;
    cout << "multiset1.empty(): " << (multiset1.empty() ? "true" : "false") << endl;

    // 6. �Զ���ȽϺ���
    auto comp = [](int a, int b) { return a > b; };
    std::multiset<int, decltype(comp)> multiset6(comp);
    multiset6.insert({1, 2, 3, 4, 5});
    cout << "multiset6 (descending order) contains: ";
    for (const auto& elem : multiset6) {
        cout << elem << " ";
    }
    cout << endl;

    // 7. ��������
    // multiset1.swap(multiset6);
    // cout << "After swap, multiset1 contains: ";
    // for (const auto& elem : multiset1) {
    //     cout << elem << " ";
    // }
    // cout << endl;

    // 8. ���
    multiset1.clear();
    cout << "After clear, multiset1 is empty: " << (multiset1.empty() ? "true" : "false") << endl;

    return 0;
}