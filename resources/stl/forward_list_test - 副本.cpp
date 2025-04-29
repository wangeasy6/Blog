#include <iostream>
#include <forward_list>
#include <algorithm> // 用于 std::for_each 和 std::reverse

int main() {
    // 1. 构造函数
    std::forward_list<int> list1; // 默认构造，空链表
    std::forward_list<int> list2(5, 10);            // 5个元素，初始值为10
    std::forward_list<int> list3{1, 2, 3, 4, 5};    // 初始化列表构造
    std::forward_list<int> list4(list3);            // 拷贝构造
    std::forward_list<int> list5(std::move(list4)); // 移动构造

    // 2. 元素访问
    std::cout << "list3.front(): " << list3.front() << std::endl; // 访问第一个元素

    // 3. 迭代器
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

    // 4. 容量
    std::cout << "list3.empty(): " << (list3.empty() ? "true" : "false") << std::endl; // 是否为空
    std::cout << "list3.max_size(): " << list3.max_size() << std::endl; // 最大容量

    // 5. 修改器
    list3.push_front(0); // 在链表头部插入元素
    list3.pop_front(); // 移除链表头部元素
    // list3.pop_back(); // std::forward_list 不支持 pop_back

    auto it = list3.begin();
    std::advance(it, 2); // 移动迭代器到第三个元素
    list3.insert_after(it, 10); // 在第三个元素后插入10
    list3.erase_after(it); // 移除第三个元素后的元素

    std::cout << "After modifications:" << std::endl;
    for (const auto& elem : list3) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 6. 排序和反转
    list3.sort(); // 排序
    list3.reverse(); // 反转

    std::cout << "After sort and reverse:" << std::endl;
    for (const auto& elem : list3) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 7. 非成员函数
    std::forward_list<int> list6 = {5, 4, 3, 2, 1};
    list3.swap(list6); // 交换内容
    std::swap(list3, list6); // 使用 std::swap 交换内容

    std::cout << "After swap with list6:" << std::endl;
    for (const auto& elem : list3) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. 清空
    list3.clear(); // 清空所有元素
    std::cout << "list3 is empty: " << (list3.empty() ? "true" : "false") << std::endl;

    return 0;
}

/*

*/