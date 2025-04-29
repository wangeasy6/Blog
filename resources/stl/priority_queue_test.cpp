#include <iostream>
#include <queue>
#include <vector>
#include <functional> // ���� std::greater

int main() {
    // 1. ���캯��
    // Ĭ�Ϲ��죺����
    std::priority_queue<int> pq1; // Ĭ������
    std::cout << "Default priority_queue (max heap):" << std::endl;

    // ʹ���Զ���ȽϺ�������С��
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq2;
    std::cout << "Custom priority_queue (min heap):" << std::endl;

    // 2. ����Ԫ��
    pq1.push(10);
    pq1.push(5);
    pq1.push(20);
    pq1.push(15);

    pq2.push(10);
    pq2.push(5);
    pq2.push(20);
    pq2.push(15);

    // 3. ����Ԫ��
    std::cout << "Top element in pq1 (max heap): " << pq1.top() << std::endl; // 20
    std::cout << "Top element in pq2 (min heap): " << pq2.top() << std::endl; // 5

    // 4. ����
    std::cout << "Size of pq1: " << pq1.size() << std::endl; // 4
    std::cout << "Size of pq2: " << pq2.size() << std::endl; // 4
    std::cout << "pq1 is empty: " << (pq1.empty() ? "true" : "false") << std::endl; // false

    // 5. �Ƴ�Ԫ��
    std::cout << "Removing elements from pq1 (max heap):" << std::endl;
    while (!pq1.empty()) {
        std::cout << pq1.top() << " ";
        pq1.pop();
    }
    std::cout << std::endl;

    std::cout << "Removing elements from pq2 (min heap):" << std::endl;
    while (!pq2.empty()) {
        std::cout << pq2.top() << " ";
        pq2.pop();
    }
    std::cout << std::endl;

    // 6. �Զ���ȽϺ���
    // ����һ���Զ���ȽϺ������������ȶ���
    auto customCompare = [](int a, int b) { return a > b; }; // ��С��
    std::priority_queue<int, std::vector<int>, decltype(customCompare)> pq3(customCompare);

    pq3.push(10);
    pq3.push(5);
    pq3.push(20);
    pq3.push(15);

    std::cout << "Custom priority_queue with lambda function (min heap):" << std::endl;
    while (!pq3.empty()) {
        std::cout << pq3.top() << " ";
        pq3.pop();
    }
    std::cout << std::endl;

    // 7. ʹ�ó�ʼ���б���
    // std::priority_queue<int> pq4({10, 5, 20, 15}); // Ĭ������
    // std::cout << "Priority_queue constructed with initializer list (max heap):" << std::endl;
    // while (!pq4.empty()) {
    //     std::cout << pq4.top() << " ";
    //     pq4.pop();
    // }
    // std::cout << std::endl;

    // 8. ʹ���Զ���ȽϺ����ͳ�ʼ���б�
    // std::priority_queue<int, std::vector<int>, std::greater<int>> pq5({10, 5, 20, 15}); // ��С��
    // std::cout << "Priority_queue constructed with initializer list and custom compare (min heap):" << std::endl;
    // while (!pq5.empty()) {
    //     std::cout << pq5.top() << " ";
    //     pq5.pop();
    // }
    // std::cout << std::endl;

    return 0;
}