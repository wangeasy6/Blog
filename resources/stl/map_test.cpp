#include <iostream>
#include <map>
#include <string>

int main() {
    // 1. 构造函数
    std::map<int, std::string> map1; // 默认构造，空 map
    std::map<int, std::string> map2 = {{1, "one"}, {2, "two"}, {3, "three"}}; // 初始化列表构造
    std::map<int, std::string> map3(map2); // 拷贝构造
    std::map<int, std::string> map4(std::move(map3)); // 移动构造
    std::map<int, std::string> map5(map2.begin(), map2.end()); // 范围构造

    // 2. 元素访问
    std::cout << "map2[1]: " << map2[1] << std::endl; // 通过下标访问
    std::cout << "map2.at(2): " << map2.at(2) << std::endl; // 通过 at() 访问（带边界检查）
    std::cout << "map2.begin()->first: " << map2.begin()->first << ", value: " << map2.begin()->second << std::endl; // 访问第一个元素

    // 3. 迭代器
    std::cout << "Iterating over map2 using iterators:" << std::endl;
    for (auto it = map2.begin(); it != map2.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    std::cout << "Iterating over map2 using range-based for loop:" << std::endl;
    for (const auto& pair : map2) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 4. 容量
    std::cout << "map2.size(): " << map2.size() << std::endl; // 当前大小
    std::cout << "map2.empty(): " << (map2.empty() ? "true" : "false") << std::endl; // 是否为空
    std::cout << "map2.max_size(): " << map2.max_size() << std::endl; // 最大容量

    // 5. 修改器
    map2[4] = "four"; // 插入或修改元素
    map2.insert({5, "five"}); // 插入键值对
    map2.erase(3); // 移除键为3的元素
    map2.erase(map2.begin()); // 移除第一个元素
    map2.clear(); // 清空所有元素
    std::cout << "After modifications, map2 is empty: " << (map2.empty() ? "true" : "false") << std::endl;

    // 6. 查找和比较
    map2 = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto it = map2.find(2); // 查找键为2的元素
    if (it != map2.end()) {
        std::cout << "Found element: " << it->first << " -> " << it->second << std::endl;
    } else {
        std::cout << "Element not found." << std::endl;
    }

    std::cout << "map2.count(2): " << map2.count(2) << std::endl; // 计数（只能是0或1）
    std::cout << "map2.lower_bound(2): " << map2.lower_bound(2)->first << " -> " << map2.lower_bound(2)->second << std::endl; // 返回第一个不小于2的元素
    std::cout << "map2.upper_bound(2): " << map2.upper_bound(2)->first << " -> " << map2.upper_bound(2)->second << std::endl; // 返回第一个大于2的元素

    // 7. 非成员函数
    std::map<int, std::string> map6 = {{3, "three"}, {4, "four"}};
    map2.swap(map6); // 交换内容
    std::swap(map2, map6); // 使用 std::swap 交换内容

    std::cout << "After swap with map6, map2 contains:" << std::endl;
    for (const auto& pair : map2) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 8. 自定义比较函数
    std::map<int, std::string, std::greater<int>> map7 = {{1, "one"}, {2, "two"}, {3, "three"}}; // 使用 std::greater 降序排列
    std::cout << "map7 (descending order) contains:" << std::endl;
    for (const auto& pair : map7) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}