#include <iostream>
#include <set>
#include <algorithm> // ���� std::for_each

int main() {
    // 1. ���캯��
    std::set<int> set1; // Ĭ�Ϲ��죬�ռ���
    std::set<int> set2 = {1, 2, 3, 4, 5}; // ��ʼ���б���
    std::set<int> set3(set2); // ��������
    std::set<int> set4(std::move(set3)); // �ƶ�����
    std::set<int> set5(set2.begin(), set2.end()); // ��Χ����

    // 2. Ԫ�ط���
    std::cout << "set2 contains: ";
    for (const auto& elem : set2) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 3. ������
    std::cout << "Iterating over set2 using iterators:" << std::endl;
    for (auto it = set2.begin(); it != set2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 4. ����
    std::cout << "set2.size(): " << set2.size() << std::endl; // ��ǰ��С
    std::cout << "set2.empty(): " << (set2.empty() ? "true" : "false") << std::endl; // �Ƿ�Ϊ��
    std::cout << "set2.max_size(): " << set2.max_size() << std::endl; // �������

    // 5. �޸���
    set2.insert(6); // ����Ԫ��
    set2.erase(3); // �Ƴ�Ԫ��
    set2.erase(set2.begin()); // �Ƴ���һ��Ԫ��
    set2.clear(); // �������Ԫ��
    std::cout << "After modifications, set2 is empty: " << (set2.empty() ? "true" : "false") << std::endl;

    // 6. ���ҺͱȽ�
    set2 = {1, 2, 3, 4, 5};
    auto it = set2.find(3); // ����Ԫ��
    if (it != set2.end()) {
        std::cout << "Found element: " << *it << std::endl;
    } else {
        std::cout << "Element not found." << std::endl;
    }

    std::cout << "set2.count(3): " << set2.count(3) << std::endl; // ������ֻ����0��1��
    std::cout << "set2.lower_bound(3): " << *set2.lower_bound(3) << std::endl; // ���ص�һ����С��3��Ԫ��
    std::cout << "set2.upper_bound(3): " << *set2.upper_bound(3) << std::endl; // ���ص�һ������3��Ԫ��

    // 7. �ǳ�Ա����
    std::set<int> set6 = {5, 4, 3, 2, 1};
    set2.swap(set6); // ��������
    std::swap(set2, set6); // ʹ�� std::swap ��������

    std::cout << "After swap with set6, set2 contains: ";
    for (const auto& elem : set2) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. �Զ���ȽϺ���
    std::set<int, std::greater<int>> set7 = {1, 2, 3, 4, 5}; // ʹ�� std::greater ��������
    std::cout << "set7 (descending order) contains: ";
    for (const auto& elem : set7) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}