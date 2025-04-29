#include <iostream>
#include <unordered_map>
#include <string>

int main() {
    // 1. 构造函数
    std::unordered_map<int, std::string> umap1; // 默认构造，空 unordered_map
    std::unordered_map<int, std::string> umap2 = {{1, "one"}, {2, "two"}, {3, "three"}}; // 初始化列表构造
    std::unordered_map<int, std::string> umap3(umap2); // 拷贝构造
    std::unordered_map<int, std::string> umap4(std::move(umap3)); // 移动构造
    std::unordered_map<int, std::string> umap5(umap2.begin(), umap2.end()); // 范围构造

    // 2. 插入和修改元素
    umap1[1] = "apple"; // 插入或修改键为1的值
    umap1.insert({2, "banana"}); // 插入键值对
    umap1.insert_or_assign(3, "cherry"); // C++17: 插入或修改键为3的值
    umap1.emplace(4, "date"); // 原地构造键值对

    std::cout << "umap1 contains:" << std::endl;
    for (const auto& pair : umap1) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 3. 查找元素
    auto it = umap1.find(2);
    if (it != umap1.end()) {
        std::cout << "Found element: " << it->first << " -> " << it->second << std::endl;
    } else {
        std::cout << "Element not found." << std::endl;
    }

    std::cout << "Count of key 2: " << umap1.count(2) << std::endl;

    // 4. 删除元素
    umap1.erase(2); // 删除键为2的元素
    umap1.erase(it); // 删除迭代器指向的元素
    std::cout << "After erasing key 2, umap1 contains:" << std::endl;
    for (const auto& pair : umap1) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 5. 容量和哈希表操作
    std::cout << "umap1.size(): " << umap1.size() << std::endl;
    std::cout << "umap1.empty(): " << (umap1.empty() ? "true" : "false") << std::endl;
    std::cout << "umap1.bucket_count(): " << umap1.bucket_count() << std::endl; // 哈希桶数量
    std::cout << "umap1.load_factor(): " << umap1.load_factor() << std::endl; // 负载因子
    std::cout << "umap1.max_load_factor(): " << umap1.max_load_factor() << std::endl;

    umap1.rehash(10); // 重新哈希，指定桶数量
    umap1.reserve(20); // 预留空间，调整桶数量以容纳更多元素

    // 6. 清空
    umap1.clear();
    std::cout << "After clear, umap1 is empty: " << (umap1.empty() ? "true" : "false") << std::endl;

    // 7. 自定义哈希函数
    struct custom_hash {
        std::size_t operator()(const std::string& str) const {
            return std::hash<std::string>()(str);
        }
    };

    std::unordered_map<std::string, int, custom_hash> umap6 = {{"apple", 1}, {"banana", 2}};
    std::cout << "umap6 contains:" << std::endl;
    for (const auto& pair : umap6) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 8. 交换内容
    std::unordered_map<std::string, int, custom_hash> umap7 = {{"orange", 3}, {"grape", 4}};
    umap6.swap(umap7);
    std::cout << "After swap, umap6 contains:" << std::endl;
    for (const auto& pair : umap6) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}