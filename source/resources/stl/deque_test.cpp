#include <iostream>
#include <vector>
#include <deque>
#include <algorithm> // ���� std::sort �� std::reverse

void printDeque(std::string des, const std::deque<int>& deq)
{
    std::cout << des;
    for (const auto& elem : deq) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
    // 1. ���캯��
    std::deque<int> deque1; // Ĭ�Ϲ��죬��˫�˶���
    std::deque<int> deque2(5, 10);  // 5��Ԫ�أ���ʼֵΪ10
    std::deque<int> deque3(deque2); // ��������
    std::deque<int> deque4(std::move(deque3));  // �ƶ�����
    std::deque<int> deque5{1, 2, 3, 4, 5};      // ��ʼ���б���

    // 2. assign
    deque1.assign(5, 10);  // ����һ����СΪ 5������Ԫ��ֵΪ 10 �� deque
    printDeque("deque.assign(5, 10): ", deque1);
    deque1.assign({1, 2, 3}); // ʹ�ó�ʼ���б�ֵ
    printDeque("deque.assign({1, 2, 3}): ", deque1);
        // �����鸳ֵ
    int arr[] = {1, 2, 3, 4};
    deque1.assign(arr, arr + sizeof(arr) / sizeof(arr[0]));
    printDeque("deque.assign(arr, arr+4): ", deque1);
        // ������������ֵ
    std::vector<int> vec = {1, 2, 3, 4, 5};
    deque1.assign(vec.begin(), vec.end());
    printDeque("deque.assign(vec.begin(), vec.end()): ", deque1);

    // 3. Ԫ�ط���
    std::cout << "deque5[0]: " << deque5[0] << std::endl;           // ͨ���±����
    std::cout << "deque5.at(2): " << deque5.at(2) << std::endl;     // ͨ��at()���ʣ����߽��飩
    std::cout << "deque5.front(): " << deque5.front() << std::endl; // ���ʵ�һ��Ԫ��
    std::cout << "deque5.back(): " << deque5.back() << std::endl;   // �������һ��Ԫ��

    // 4. ����
    std::cout << "deque5.size(): " << deque5.size() << std::endl;           // ��ǰ��С
    std::cout << "deque5.max_size(): " << deque5.max_size() << std::endl;   // �������ɵ����Ԫ������
    std::cout << "deque5.empty(): " << (deque5.empty() ? "true" : "false") << std::endl; // �Ƿ�Ϊ��

    // 5. ������
    std::cout << "Iterating over deque5 using iterators:" << std::endl;
    for (auto it = deque5.begin(); it != deque5.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    printDeque("Iterating over deque5 using range-based for loop:\r\n", deque5);

    // 6. �޸���
    deque5.push_back(6);                            // ��ĩβ���Ԫ��
    printDeque("deque5.push_back(6): ", deque5);
    deque5.push_front(0);                           // �ڿ�ͷ���Ԫ��
    printDeque("deque5.push_front(0): ", deque5);
    deque5.pop_back();                              // �Ƴ����һ��Ԫ��
    printDeque("deque5.pop_back(): ", deque5);
    deque5.pop_front();                             // �Ƴ���һ��Ԫ��
    printDeque("deque5.pop_front(): ", deque5);
    deque5.emplace_front(10);                       // ��ͷ������Ԫ�أ�ֱ�ӵ��õ�Ԫ�����͵Ĺ��캯��
    printDeque("deque5.emplace_front(10): ", deque5);
    deque5.emplace_back(20);                        // ��β������Ԫ�أ�ֱ�ӵ��õ�Ԫ�����͵Ĺ��캯��
    printDeque("deque5.emplace_back(20): ", deque5);
    deque5.insert(deque5.begin() + 2, 10);          // ������2������Ԫ��10
    printDeque("deque5.insert(deque5.begin() + 2, 10): ", deque5);
    deque5.erase(deque5.begin() + 1);               // �Ƴ�����1��Ԫ��
    printDeque("deque5.erase(deque5.begin() + 1): ", deque5);
    deque5.resize(10, 0);                           // ������С����Ԫ����0���
    printDeque("deque5.resize(10, 0): ", deque5);
    deque5.shrink_to_fit();

    // 7. ����ͷ�ת
    std::sort(deque5.begin(), deque5.end()); // ����
    std::reverse(deque5.begin(), deque5.end()); // ��ת

    std::cout << "After sort and reverse:" << std::endl;
    for (const auto& elem : deque5) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. �ǳ�Ա����
    std::deque<int> deque6 = {5, 4, 3, 2, 1};
    deque5.swap(deque6);        // ��������
    std::swap(deque5, deque6);  // ʹ��std::swap��������

    std::cout << "After swap with deque6:" << std::endl;
    for (const auto& elem : deque5) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 9. ���
    deque5.clear(); // �������Ԫ��
    std::cout << "deque5 is empty: " << (deque5.empty() ? "true" : "false") << std::endl;

    // 10. allocate
        // ��ȡ deque �ķ�����
    std::allocator<int> alloc = deque1.get_allocator();
        // ʹ�÷����������ڴ�
    int* ptr = alloc.allocate(3); // ���� 3 �� int ���͵��ڴ�
        // ʹ�÷������������
    alloc.construct(ptr + 0, 10);
    alloc.construct(ptr + 1, 20);
    alloc.construct(ptr + 2, 30);
        // ���������ڴ��е�ֵ
    std::cout << "Allocated memory values: ";
    for (int i = 0; i < 3; ++i) {
        std::cout << ptr[i] << " ";
    }
    std::cout << "\n";
        // ʹ�÷��������ٶ���
    alloc.destroy(ptr + 0);
    alloc.destroy(ptr + 1);
    alloc.destroy(ptr + 2);
        // ʹ�÷������ͷ��ڴ�
    alloc.deallocate(ptr, 3);

    return 0;
}

/*
deque.assign(5, 10): 10 10 10 10 10
deque.assign({1, 2, 3}): 1 2 3
deque.assign(arr, arr+4): 1 2 3 4
deque.assign(vec.begin(), vec.end()): 1 2 3 4 5
deque5[0]: 1
deque5.at(2): 3
deque5.front(): 1
deque5.back(): 5
deque5.size(): 5
deque5.max_size(): 2305843009213693951
deque5.empty(): false
Iterating over deque5 using iterators:
1 2 3 4 5
Iterating over deque5 using range-based for loop:
1 2 3 4 5
deque5.push_back(6): 1 2 3 4 5 6
deque5.push_front(0): 0 1 2 3 4 5 6
deque5.pop_back(): 0 1 2 3 4 5
deque5.pop_front(): 1 2 3 4 5
deque5.emplace_front(10): 10 1 2 3 4 5
deque5.emplace_back(20): 10 1 2 3 4 5 20
deque5.insert(deque5.begin() + 2, 10): 10 1 10 2 3 4 5 20
deque5.erase(deque5.begin() + 1): 10 10 2 3 4 5 20
deque5.resize(10, 0): 10 10 2 3 4 5 20 0 0 0
After sort and reverse:
20 10 10 5 4 3 2 0 0 0
After swap with deque6:
20 10 10 5 4 3 2 0 0 0
deque5 is empty: true
Allocated memory values: 10 20 30
*/