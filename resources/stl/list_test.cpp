#include <iostream>
#include <list>
#include <algorithm> // 用于 std::sort 和 std::reverse

int main() {
    // 1. 构造函数
    std::list<int> list1; // 默认构造，空链表
    std::list<int> list2(5, 10); // 5个元素，初始值为10
    std::list<int> list3(list2); // 拷贝构造
    std::list<int> list4(std::move(list3)); // 移动构造
    std::list<int> list5 = {1, 2, 3, 4, 5}; // 初始化列表构造
    std::list<int> list6(list5.begin(), list5.end()); // 范围构造

    // 2. 元素访问
    std::cout << "list5.front(): " << list5.front() << std::endl; // 访问第一个元素
    std::cout << "list5.back(): " << list5.back() << std::endl; // 访问最后一个元素

    // 3. 迭代器
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

    // 4. 容量
    std::cout << "list5.size(): " << list5.size() << std::endl; // 当前大小
    std::cout << "list5.empty(): " << (list5.empty() ? "true" : "false") << std::endl; // 是否为空

    // 5. 修改器
    list5.push_back(6); // 在末尾添加元素
    list5.push_front(0); // 在开头添加元素
    list5.pop_back(); // 移除最后一个元素
    list5.pop_front(); // 移除第一个元素

    auto it = list5.begin();
    std::advance(it, 2); // 移动到第三个元素
    list5.insert(it, 10); // 在第三个位置插入元素10
    list5.erase(it); // 移除第三个位置的元素

    std::cout << "After modifications:" << std::endl;
    for (const auto& elem : list5) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 6. 排序和反转
    list5.sort(); // 排序
    list5.reverse(); // 反转

    std::cout << "After sort and reverse:" << std::endl;
    for (const auto& elem : list5) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 7. 非成员函数
    std::list<int> list7 = {5, 4, 3, 2, 1};
    list5.swap(list7); // 交换内容
    std::swap(list5, list7); // 使用std::swap交换内容

    std::cout << "After swap with list7:" << std::endl;
    for (const auto& elem : list5) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. 清空
    list5.clear(); // 清空所有元素
    std::cout << "list5 is empty: " << (list5.empty() ? "true" : "false") << std::endl;

    return 0;
}