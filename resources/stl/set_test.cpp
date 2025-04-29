#include <iostream>
#include <set>
#include <algorithm> // 用于 std::for_each

int main() {
    // 1. 构造函数
    std::set<int> set1; // 默认构造，空集合
    std::set<int> set2 = {1, 2, 3, 4, 5}; // 初始化列表构造
    std::set<int> set3(set2); // 拷贝构造
    std::set<int> set4(std::move(set3)); // 移动构造
    std::set<int> set5(set2.begin(), set2.end()); // 范围构造

    // 2. 元素访问
    std::cout << "set2 contains: ";
    for (const auto& elem : set2) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 3. 迭代器
    std::cout << "Iterating over set2 using iterators:" << std::endl;
    for (auto it = set2.begin(); it != set2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 4. 容量
    std::cout << "set2.size(): " << set2.size() << std::endl; // 当前大小
    std::cout << "set2.empty(): " << (set2.empty() ? "true" : "false") << std::endl; // 是否为空
    std::cout << "set2.max_size(): " << set2.max_size() << std::endl; // 最大容量

    // 5. 修改器
    set2.insert(6); // 插入元素
    set2.erase(3); // 移除元素
    set2.erase(set2.begin()); // 移除第一个元素
    set2.clear(); // 清空所有元素
    std::cout << "After modifications, set2 is empty: " << (set2.empty() ? "true" : "false") << std::endl;

    // 6. 查找和比较
    set2 = {1, 2, 3, 4, 5};
    auto it = set2.find(3); // 查找元素
    if (it != set2.end()) {
        std::cout << "Found element: " << *it << std::endl;
    } else {
        std::cout << "Element not found." << std::endl;
    }

    std::cout << "set2.count(3): " << set2.count(3) << std::endl; // 计数（只能是0或1）
    std::cout << "set2.lower_bound(3): " << *set2.lower_bound(3) << std::endl; // 返回第一个不小于3的元素
    std::cout << "set2.upper_bound(3): " << *set2.upper_bound(3) << std::endl; // 返回第一个大于3的元素

    // 7. 非成员函数
    std::set<int> set6 = {5, 4, 3, 2, 1};
    set2.swap(set6); // 交换内容
    std::swap(set2, set6); // 使用 std::swap 交换内容

    std::cout << "After swap with set6, set2 contains: ";
    for (const auto& elem : set2) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. 自定义比较函数
    std::set<int, std::greater<int>> set7 = {1, 2, 3, 4, 5}; // 使用 std::greater 降序排列
    std::cout << "set7 (descending order) contains: ";
    for (const auto& elem : set7) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}