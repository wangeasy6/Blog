#include <iostream>
#include <forward_list>
#include <algorithm> // ���� std::for_each �� std::reverse

int main() {
    // 1. ���캯��
    std::forward_list<int> list1; // Ĭ�Ϲ��죬������
    std::forward_list<int> list2(5, 10);            // 5��Ԫ�أ���ʼֵΪ10
    std::forward_list<int> list3{1, 2, 3, 4, 5};    // ��ʼ���б���
    std::forward_list<int> list4(list3);            // ��������
    std::forward_list<int> list5(std::move(list4)); // �ƶ�����

    // 2. Ԫ�ط���
    std::cout << "list3.front(): " << list3.front() << std::endl; // ���ʵ�һ��Ԫ��

    // 3. ������
    std::cout << "Iterating over list3 using iterators:" << std::endl;
    for (auto it = list3.begin(); it != list3.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Iterating over list3 using range-based for loop:" << std::endl;
    for (const auto& elem : list3) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 4. ����
    std::cout << "list3.empty(): " << (list3.empty() ? "true" : "false") << std::endl; // �Ƿ�Ϊ��
    std::cout << "list3.max_size(): " << list3.max_size() << std::endl; // �������

    // 5. �޸���
    list3.push_front(0); // ������ͷ������Ԫ��
    list3.pop_front(); // �Ƴ�����ͷ��Ԫ��
    // list3.pop_back(); // std::forward_list ��֧�� pop_back

    auto it = list3.begin();
    std::advance(it, 2); // �ƶ���������������Ԫ��
    list3.insert_after(it, 10); // �ڵ�����Ԫ�غ����10
    list3.erase_after(it); // �Ƴ�������Ԫ�غ��Ԫ��

    std::cout << "After modifications:" << std::endl;
    for (const auto& elem : list3) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 6. ����ͷ�ת
    list3.sort(); // ����
    list3.reverse(); // ��ת

    std::cout << "After sort and reverse:" << std::endl;
    for (const auto& elem : list3) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 7. �ǳ�Ա����
    std::forward_list<int> list6 = {5, 4, 3, 2, 1};
    list3.swap(list6); // ��������
    std::swap(list3, list6); // ʹ�� std::swap ��������

    std::cout << "After swap with list6:" << std::endl;
    for (const auto& elem : list3) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. ���
    list3.clear(); // �������Ԫ��
    std::cout << "list3 is empty: " << (list3.empty() ? "true" : "false") << std::endl;

    return 0;
}

/*

*/