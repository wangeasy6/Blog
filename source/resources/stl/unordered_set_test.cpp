#include <iostream>
#include <unordered_set>
#include <string>

int main() {
    // 1. 构造函数
    std::unordered_set<int> uset1; // 默认构造，空 unordered_set
    std::unordered_set<int> uset2 = {1, 2, 3, 4, 5}; // 初始化列表构造
    std::unordered_set<int> uset3(uset2); // 拷贝构造
    std::unordered_set<int> uset4(std::move(uset3)); // 移动构造
    std::unordered_set<int> uset5(uset2.begin(), uset2.end()); // 范围构造

    // 2. 插入元素
    uset1.insert(10);
    uset1.insert(20);
    uset1.insert(30);
    uset1.insert(10); // 重复元素不会插入

    std::cout << "uset1 contains: ";
    for (const auto& elem : uset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 3. 查找元素
    auto it = uset1.find(20);
    if (it != uset1.end()) {
        std::cout << "Found element: " << *it << std::endl;
    } else {
        std::cout << "Element not found." << std::endl;
    }

    std::cout << "Count of 20 in uset1: " << uset1.count(20) << std::endl;

    // 4. 删除元素
    uset1.erase(20); // 删除元素
    std::cout << "After erasing 20, uset1 contains: ";
    for (const auto& elem : uset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    uset1.erase(it); // 删除迭代器指向的元素
    std::cout << "After erasing iterator, uset1 contains: ";
    for (const auto& elem : uset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 5. 容量和哈希表操作
    std::cout << "uset1.size(): " << uset1.size() << std::endl;
    std::cout << "uset1.empty(): " << (uset1.empty() ? "true" : "false") << std::endl;
    std::cout << "uset1.max_size(): " << uset1.max_size() << std::endl;
    std::cout << "uset1.bucket_count(): " << uset1.bucket_count() << std::endl; // 哈希桶数量
    std::cout << "uset1.load_factor(): " << uset1.load_factor() << std::endl; // 负载因子
    std::cout << "uset1.max_load_factor(): " << uset1.max_load_factor() << std::endl;

    uset1.rehash(10); // 重新哈希，指定桶数量
    uset1.reserve(20); // 预留空间，调整桶数量以容纳更多元素

    // 6. 清空
    uset1.clear();
    std::cout << "After clear, uset1 is empty: " << (uset1.empty() ? "true" : "false") << std::endl;

    // 7. 自定义哈希函数
    struct custom_hash {
        std::size_t operator()(const std::string& str) const {
            return std::hash<std::string>()(str);
        }
    };

    std::unordered_set<std::string, custom_hash> uset6 = {"apple", "banana", "cherry"};
    std::cout << "uset6 contains: ";
    for (const auto& elem : uset6) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. 交换内容
    std::unordered_set<std::string, custom_hash> uset7 = {"orange", "grape"};
    uset6.swap(uset7);
    std::cout << "After swap, uset6 contains: ";
    for (const auto& elem : uset6) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}