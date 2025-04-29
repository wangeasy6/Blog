#include <iostream>
#include <list>
#include <algorithm> // ���� std::sort �� std::reverse

int main() {
    // 1. ���캯��
    std::list<int> list1; // Ĭ�Ϲ��죬������
    std::list<int> list2(5, 10); // 5��Ԫ�أ���ʼֵΪ10
    std::list<int> list3(list2); // ��������
    std::list<int> list4(std::move(list3)); // �ƶ�����
    std::list<int> list5 = {1, 2, 3, 4, 5}; // ��ʼ���б���
    std::list<int> list6(list5.begin(), list5.end()); // ��Χ����

    // 2. Ԫ�ط���
    std::cout << "list5.front(): " << list5.front() << std::endl; // ���ʵ�һ��Ԫ��
    std::cout << "list5.back(): " << list5.back() << std::endl; // �������һ��Ԫ��

    // 3. ������
    std::cout << "Iterating over list5 using iterators:" << std::endl;
    for (auto it = list5.begin(); it != list5.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Iterating over list5 using range-based for loop:" << std::endl;
    for (const auto& elem : list5) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 4. ����
    std::cout << "list5.size(): " << list5.size() << std::endl; // ��ǰ��С
    std::cout << "list5.empty(): " << (list5.empty() ? "true" : "false") << std::endl; // �Ƿ�Ϊ��

    // 5. �޸���
    list5.push_back(6); // ��ĩβ���Ԫ��
    list5.push_front(0); // �ڿ�ͷ���Ԫ��
    list5.pop_back(); // �Ƴ����һ��Ԫ��
    list5.pop_front(); // �Ƴ���һ��Ԫ��

    auto it = list5.begin();
    std::advance(it, 2); // �ƶ���������Ԫ��
    list5.insert(it, 10); // �ڵ�����λ�ò���Ԫ��10
    list5.erase(it); // �Ƴ�������λ�õ�Ԫ��

    std::cout << "After modifications:" << std::endl;
    for (const auto& elem : list5) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 6. ����ͷ�ת
    list5.sort(); // ����
    list5.reverse(); // ��ת

    std::cout << "After sort and reverse:" << std::endl;
    for (const auto& elem : list5) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 7. �ǳ�Ա����
    std::list<int> list7 = {5, 4, 3, 2, 1};
    list5.swap(list7); // ��������
    std::swap(list5, list7); // ʹ��std::swap��������

    std::cout << "After swap with list7:" << std::endl;
    for (const auto& elem : list5) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. ���
    list5.clear(); // �������Ԫ��
    std::cout << "list5 is empty: " << (list5.empty() ? "true" : "false") << std::endl;

    return 0;
}