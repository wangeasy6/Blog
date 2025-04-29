#include <iostream>
#include <map>
#include <string>

int main() {
    // 1. ���캯��
    std::map<int, std::string> map1; // Ĭ�Ϲ��죬�� map
    std::map<int, std::string> map2 = {{1, "one"}, {2, "two"}, {3, "three"}}; // ��ʼ���б���
    std::map<int, std::string> map3(map2); // ��������
    std::map<int, std::string> map4(std::move(map3)); // �ƶ�����
    std::map<int, std::string> map5(map2.begin(), map2.end()); // ��Χ����

    // 2. Ԫ�ط���
    std::cout << "map2[1]: " << map2[1] << std::endl; // ͨ���±����
    std::cout << "map2.at(2): " << map2.at(2) << std::endl; // ͨ�� at() ���ʣ����߽��飩
    std::cout << "map2.begin()->first: " << map2.begin()->first << ", value: " << map2.begin()->second << std::endl; // ���ʵ�һ��Ԫ��

    // 3. ������
    std::cout << "Iterating over map2 using iterators:" << std::endl;
    for (auto it = map2.begin(); it != map2.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    std::cout << "Iterating over map2 using range-based for loop:" << std::endl;
    for (const auto& pair : map2) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 4. ����
    std::cout << "map2.size(): " << map2.size() << std::endl; // ��ǰ��С
    std::cout << "map2.empty(): " << (map2.empty() ? "true" : "false") << std::endl; // �Ƿ�Ϊ��
    std::cout << "map2.max_size(): " << map2.max_size() << std::endl; // �������

    // 5. �޸���
    map2[4] = "four"; // ������޸�Ԫ��
    map2.insert({5, "five"}); // �����ֵ��
    map2.erase(3); // �Ƴ���Ϊ3��Ԫ��
    map2.erase(map2.begin()); // �Ƴ���һ��Ԫ��
    map2.clear(); // �������Ԫ��
    std::cout << "After modifications, map2 is empty: " << (map2.empty() ? "true" : "false") << std::endl;

    // 6. ���ҺͱȽ�
    map2 = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto it = map2.find(2); // ���Ҽ�Ϊ2��Ԫ��
    if (it != map2.end()) {
        std::cout << "Found element: " << it->first << " -> " << it->second << std::endl;
    } else {
        std::cout << "Element not found." << std::endl;
    }

    std::cout << "map2.count(2): " << map2.count(2) << std::endl; // ������ֻ����0��1��
    std::cout << "map2.lower_bound(2): " << map2.lower_bound(2)->first << " -> " << map2.lower_bound(2)->second << std::endl; // ���ص�һ����С��2��Ԫ��
    std::cout << "map2.upper_bound(2): " << map2.upper_bound(2)->first << " -> " << map2.upper_bound(2)->second << std::endl; // ���ص�һ������2��Ԫ��

    // 7. �ǳ�Ա����
    std::map<int, std::string> map6 = {{3, "three"}, {4, "four"}};
    map2.swap(map6); // ��������
    std::swap(map2, map6); // ʹ�� std::swap ��������

    std::cout << "After swap with map6, map2 contains:" << std::endl;
    for (const auto& pair : map2) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 8. �Զ���ȽϺ���
    std::map<int, std::string, std::greater<int>> map7 = {{1, "one"}, {2, "two"}, {3, "three"}}; // ʹ�� std::greater ��������
    std::cout << "map7 (descending order) contains:" << std::endl;
    for (const auto& pair : map7) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}