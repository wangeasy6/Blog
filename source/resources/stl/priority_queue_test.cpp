#include <iostream>
#include <queue>
#include <vector>
#include <functional> // 用于 std::greater

int main() {
    // 1. 构造函数
    // 默认构造：最大堆
    std::priority_queue<int> pq1; // 默认最大堆
    std::cout << "Default priority_queue (max heap):" << std::endl;

    // 使用自定义比较函数：最小堆
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq2;
    std::cout << "Custom priority_queue (min heap):" << std::endl;

    // 2. 插入元素
    pq1.push(10);
    pq1.push(5);
    pq1.push(20);
    pq1.push(15);

    pq2.push(10);
    pq2.push(5);
    pq2.push(20);
    pq2.push(15);

    // 3. 访问元素
    std::cout << "Top element in pq1 (max heap): " << pq1.top() << std::endl; // 20
    std::cout << "Top element in pq2 (min heap): " << pq2.top() << std::endl; // 5

    // 4. 容量
    std::cout << "Size of pq1: " << pq1.size() << std::endl; // 4
    std::cout << "Size of pq2: " << pq2.size() << std::endl; // 4
    std::cout << "pq1 is empty: " << (pq1.empty() ? "true" : "false") << std::endl; // false

    // 5. 移除元素
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

    // 6. 自定义比较函数
    // 定义一个自定义比较函数，用于优先队列
    auto customCompare = [](int a, int b) { return a > b; }; // 最小堆
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

    // 7. 使用初始化列表构造
    // std::priority_queue<int> pq4({10, 5, 20, 15}); // 默认最大堆
    // std::cout << "Priority_queue constructed with initializer list (max heap):" << std::endl;
    // while (!pq4.empty()) {
    //     std::cout << pq4.top() << " ";
    //     pq4.pop();
    // }
    // std::cout << std::endl;

    // 8. 使用自定义比较函数和初始化列表
    // std::priority_queue<int, std::vector<int>, std::greater<int>> pq5({10, 5, 20, 15}); // 最小堆
    // std::cout << "Priority_queue constructed with initializer list and custom compare (min heap):" << std::endl;
    // while (!pq5.empty()) {
    //     std::cout << pq5.top() << " ";
    //     pq5.pop();
    // }
    // std::cout << std::endl;

    return 0;
}