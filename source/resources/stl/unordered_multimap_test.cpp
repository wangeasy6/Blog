#include <iostream>
#include <unordered_map>
#include <string>

int main() {
    // 1. 构造函数
    std::unordered_multimap<std::string, int> ummap1; // 默认构造，空 unordered_multimap
    std::unordered_multimap<std::string, int> ummap2 = {{"apple", 1}, {"banana", 2}, {"apple", 3}}; // 初始化列表构造
    std::unordered_multimap<std::string, int> ummap3(ummap2); // 拷贝构造
    std::unordered_multimap<std::string, int> ummap4(std::move(ummap3)); // 移动构造
    std::unordered_multimap<std::string, int> ummap5(ummap2.begin(), ummap2.end()); // 范围构造

    // 2. 插入元素
    ummap1.insert({"apple", 10});
    ummap1.insert({"banana", 20});
    ummap1.insert({"apple", 15}); // 允许键重复
    ummap1.insert({"orange", 30});

    std::cout << "ummap1 contains:" << std::endl;
    for (const auto& pair : ummap1) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 3. 查找元素
    auto range = ummap1.equal_range("apple"); // 获取键为 "apple" 的所有元素范围
    std::cout << "Elements with key 'apple':" << std::endl;
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    auto it = ummap1.find("banana"); // 查找键为 "banana" 的元素
    if (it != ummap1.end()) {
        std::cout << "Found key 'banana' with value: " << it->second << std::endl;
    }

    std::cout << "Count of key 'apple': " << ummap1.count("apple") << std::endl;

    // 4. 删除元素
    ummap1.erase("apple"); // 删除所有键为 "apple" 的元素
    std::cout << "After erasing 'apple', ummap1 contains:" << std::endl;
    for (const auto& pair : ummap1) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 5. 容量和哈希表操作
    std::cout << "ummap1.size(): " << ummap1.size() << std::endl;
    std::cout << "ummap1.empty(): " << (ummap1.empty() ? "true" : "false") << std::endl;
    std::cout << "ummap1.bucket_count(): " << ummap1.bucket_count() << std::endl; // 哈希桶数量
    std::cout << "ummap1.load_factor(): " << ummap1.load_factor() << std::endl; // 负载因子
    std::cout << "ummap1.max_load_factor(): " << ummap1.max_load_factor() << std::endl;

    ummap1.rehash(10); // 重新哈希，指定桶数量
    ummap1.reserve(20); // 预留空间，调整桶数量以容纳更多元素

    // 6. 清空
    ummap1.clear();
    std::cout << "After clear, ummap1 is empty: " << (ummap1.empty() ? "true" : "false") << std::endl;

    // 7. 自定义哈希函数
    struct custom_hash {
        std::size_t operator()(const std::string& str) const {
            return std::hash<std::string>()(str);
        }
    };

    std::unordered_multimap<std::string, int, custom_hash> ummap6 = {{"apple", 1}, {"banana", 2}};
    std::cout << "ummap6 contains:" << std::endl;
    for (const auto& pair : ummap6) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 8. 交换内容
    std::unordered_multimap<std::string, int, custom_hash> ummap7 = {{"orange", 3}, {"grape", 4}};
    ummap6.swap(ummap7);
    std::cout << "After swap, ummap6 contains:" << std::endl;
    for (const auto& pair : ummap6) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}