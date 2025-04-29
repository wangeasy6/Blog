#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    // 1. 构造函数
    std::multimap<int, std::string> mmap1; // 默认构造，空 multimap
    std::multimap<int, std::string> mmap2 = {{1, "one"}, {2, "two"}, {1, "uno"}, {2, "dos"}}; // 初始化列表构造
    std::multimap<int, std::string> mmap3(mmap2); // 拷贝构造
    std::multimap<int, std::string> mmap4(std::move(mmap3)); // 移动构造
    std::multimap<int, std::string> mmap5(mmap2.begin(), mmap2.end()); // 范围构造

    // 2. 插入元素
    mmap1.insert({1, "apple"});
    mmap1.insert({2, "banana"});
    mmap1.insert({1, "apricot"});
    mmap1.insert({3, "cherry"});

    std::cout << "mmap1 contains:" << std::endl;
    for (const auto& pair : mmap1) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 3. 查找和计数
    auto range = mmap1.equal_range(1); // 获取键为1的所有元素范围
    std::cout << "Elements with key 1:" << std::endl;
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    std::cout << "Count of key 1: " << mmap1.count(1) << std::endl;

    // 4. 删除元素
    mmap1.erase(2); // 删除所有键为2的元素
    std::cout << "After erasing key 2, mmap1 contains:" << std::endl;
    for (const auto& pair : mmap1) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 5. 容量
    std::cout << "mmap1.size(): " << mmap1.size() << std::endl; // 当前大小
    std::cout << "mmap1.empty(): " << (mmap1.empty() ? "true" : "false") << std::endl; // 是否为空
    std::cout << "mmap1.max_size(): " << mmap1.max_size() << std::endl; // 最大容量

    // 6. 迭代器
    std::cout << "Iterating over mmap1 using iterators:" << std::endl;
    for (auto it = mmap1.begin(); it != mmap1.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    // 7. 清空
    mmap1.clear();
    std::cout << "After clear, mmap1 is empty: " << (mmap1.empty() ? "true" : "false") << std::endl;

    // 8. 自定义比较函数
    auto comp = [](int a, int b) { return a > b; };
    std::multimap<int, std::string, decltype(comp)> mmap6(comp);
    mmap6.insert({1, "one"});
    mmap6.insert({2, "two"});
    mmap6.insert({1, "uno"});

    std::cout << "mmap6 (descending order) contains:" << std::endl;
    for (const auto& pair : mmap6) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 9. 交换内容
    // mmap2.swap(mmap6);
    // std::cout << "After swap with mmap6, mmap2 contains:" << std::endl;
    // for (const auto& pair : mmap2) {
    //     std::cout << pair.first << ": " << pair.second << std::endl;
    // }

    return 0;
}