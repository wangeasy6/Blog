#include <iostream>
#include <array>

using namespace std;

int main() {
    // 1. 构造函数
    array<int, 5> arr1 = {1, 2, 3, 4, 5}; // 初始化列表构造
    array<int, 5> arr2 = arr1;            // 拷贝构造
    array<int, 5> arr3 = move(arr2);      // 移动构造

    // 2. 元素访问
    cout << "arr1[0]: " << arr1[0] << endl;
    cout << "arr1.at(1): " << arr1.at(1) << endl;
    cout << "arr1.front(): " << arr1.front() << endl;
    cout << "arr1.back(): " << arr1.back() << endl;
    cout << "arr1.data(): " << arr1.data() << endl;
    cout << "get<2>(arr1): " << get<2>(arr1) << endl;

    // 3. 容量相关
    cout << "arr1.size(): " << arr1.size() << endl;
    cout << "arr1.max_size(): " << arr1.max_size() << endl;
    cout << "arr1.empty(): " << (arr1.empty() ? "true" : "false") << endl;

    // 4. 填充与交换
    arr1.fill(10); // 将所有元素填充为 10
    cout << "After fill(10): ";
    for (const auto& elem : arr1) {
        cout << elem << " ";
    }
    cout << endl;

    array<int, 5> arr4 = {5, 4, 3, 2, 1};
    arr1.swap(arr4); // 交换 arr1 和 arr4 的内容
    cout << "After swap with arr4: ";
    for (const auto& elem : arr1) {
        cout << elem << " ";
    }
    cout << endl;

    // 5. 迭代器
    cout << "Iterating over arr1 using iterators:" << endl;
    for (auto it = arr1.begin(); it != arr1.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Iterating over arr1 using const iterators:" << endl;
    for (auto it = arr1.cbegin(); it != arr1.cend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Iterating over arr1 using reverse iterators:" << endl;
    for (auto it = arr1.rbegin(); it != arr1.rend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Iterating over arr1 using range-based for loop:" << endl;
    for (const auto& elem : arr1) {
        cout << elem << " ";
    }
    cout << endl;

    return 0;
}

/*
arr1[0]: 1
arr1.at(1): 2
arr1.front(): 1
arr1.back(): 5
arr1.data(): 0x7ffffcb60
get<2>(arr1): 3
arr1.size(): 5
arr1.max_size(): 5
arr1.empty(): false
After fill(10): 10 10 10 10 10
After swap with arr4: 5 4 3 2 1
Iterating over arr1 using iterators:
5 4 3 2 1
Iterating over arr1 using const iterators:
5 4 3 2 1
Iterating over arr1 using reverse iterators:
1 2 3 4 5
Iterating over arr1 using range-based for loop:
5 4 3 2 1
*/