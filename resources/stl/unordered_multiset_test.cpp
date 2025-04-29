#include <iostream>
#include <unordered_set>
#include <string>

int main() {
    // 1. 构造函数
    std::unordered_multiset<int> umset1; // 默认构造，空 unordered_multiset
    std::unordered_multiset<int> umset2 = {1, 2, 3, 4, 5}; // 初始化列表构造
    std::unordered_multiset<int> umset3(umset2); // 拷贝构造
    std::unordered_multiset<int> umset4(std::move(umset3)); // 移动构造
    std::unordered_multiset<int> umset5(umset2.begin(), umset2.end()); // 范围构造

    // 2. 插入元素
    umset1.insert(10);
    umset1.insert(20);
    umset1.insert(10); // 允许重复元素
    umset1.insert(30);

    std::cout << "umset1 contains: ";
    for (const auto& elem : umset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 3. 查找元素
    auto it = umset1.find(10);
    if (it != umset1.end()) {
        std::cout << "Found element: " << *it << std::endl;
    } else {
        std::cout << "Element not found." << std::endl;
    }

    std::cout << "Count of 10 in umset1: " << umset1.count(10) << std::endl;

    // 4. 删除元素
    umset1.erase(it); // 删除迭代器指向的元素
    std::cout << "After erasing iterator, umset1 contains: ";
    for (const auto& elem : umset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    umset1.erase(10); // 删除所有值为10的元素
    std::cout << "After erasing all 10s, umset1 contains: ";
    for (const auto& elem : umset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 5. 容量和哈希表操作
    std::cout << "umset1.size(): " << umset1.size() << std::endl;
    std::cout << "umset1.empty(): " << (umset1.empty() ? "true" : "false") << std::endl;
    std::cout << "umset1.bucket_count(): " << umset1.bucket_count() << std::endl; // 哈希桶数量
    std::cout << "umset1.load_factor(): " << umset1.load_factor() << std::endl; // 负载因子

    umset1.rehash(10); // 重新哈希，指定桶数量
    umset1.reserve(20); // 预留空间，调整桶数量以容纳更多元素

    // 6. 清空
    umset1.clear();
    std::cout << "After clear, umset1 is empty: " << (umset1.empty() ? "true" : "false") << std::endl;

    // 7. 自定义哈希函数
    struct custom_hash {
        std::size_t operator()(const std::string& str) const {
            return std::hash<std::string>()(str);
        }
    };

    std::unordered_multiset<std::string, custom_hash> umset6 = {"apple", "banana", "apple"};
    std::cout << "umset6 contains: ";
    for (const auto& elem : umset6) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. 交换内容
    std::unordered_multiset<std::string, custom_hash> umset7 = {"orange", "grape"};
    umset6.swap(umset7);
    std::cout << "After swap, umset6 contains: ";
    for (const auto& elem : umset6) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}