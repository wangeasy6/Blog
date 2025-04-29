#include <iostream>
#include <set>
#include <algorithm> // 用于 std::for_each

using namespace std;

int main() {
    // 1. 构造函数
    std::multiset<int> multiset1; // 默认构造，空 multiset
    std::multiset<int> multiset2 = {1, 2, 3, 4, 5}; // 初始化列表构造
    std::multiset<int> multiset3(multiset2); // 拷贝构造
    std::multiset<int> multiset4(std::move(multiset3)); // 移动构造
    std::multiset<int> multiset5(multiset2.begin(), multiset2.end()); // 范围构造

    // 2. 插入元素
    multiset1.insert(5);
    multiset1.insert(2);
    multiset1.insert(7);
    multiset1.insert(5); // 允许重复元素
    multiset1.insert(1);

    cout << "multiset1 contains: ";
    for (const auto& elem : multiset1) {
        cout << elem << " ";
    }
    cout << endl;

    // 3. 查找和计数
    cout << "Count of 5 in multiset1: " << multiset1.count(5) << endl;
    auto it = multiset1.find(5);
    if (it != multiset1.end()) {
        cout << "Found element: " << *it << endl;
    }

    // 4. 删除元素
    multiset1.erase(it); // 删除找到的元素
    cout << "After erasing one 5, multiset1 contains: ";
    for (const auto& elem : multiset1) {
        cout << elem << " ";
    }
    cout << endl;

    multiset1.erase(5); // 删除所有值为5的元素
    cout << "After erasing all 5s, multiset1 contains: ";
    for (const auto& elem : multiset1) {
        cout << elem << " ";
    }
    cout << endl;

    // 5. 容量和迭代器
    cout << "multiset1.size(): " << multiset1.size() << endl;
    cout << "multiset1.empty(): " << (multiset1.empty() ? "true" : "false") << endl;

    // 6. 自定义比较函数
    auto comp = [](int a, int b) { return a > b; };
    std::multiset<int, decltype(comp)> multiset6(comp);
    multiset6.insert({1, 2, 3, 4, 5});
    cout << "multiset6 (descending order) contains: ";
    for (const auto& elem : multiset6) {
        cout << elem << " ";
    }
    cout << endl;

    // 7. 交换内容
    // multiset1.swap(multiset6);
    // cout << "After swap, multiset1 contains: ";
    // for (const auto& elem : multiset1) {
    //     cout << elem << " ";
    // }
    // cout << endl;

    // 8. 清空
    multiset1.clear();
    cout << "After clear, multiset1 is empty: " << (multiset1.empty() ? "true" : "false") << endl;

    return 0;
}