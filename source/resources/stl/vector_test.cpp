#include <iostream>
#include <vector>
#include <array>
#include <algorithm> // 用于 std::sort 和 std::reverse

void printVector(std::string des,std::vector<int>& vec);
void printArray(const int* arr, size_t size);

int main() {
    // 1. 构造函数
    std::vector<int> vec1;          // 默认构造
    std::vector<int> vec2(5, 10);   // 5个元素，初始值为10
    std::vector<int> vec3(vec2);    // 拷贝构造
    std::vector<int> vec4(std::move(vec3));  // 移动构造
    std::vector<int> vec5{1, 2, 3, 4, 5}; // 初始化列表构造
    // std::vector<int> vec5 = {1, 2, 3, 4, 5}; 分为 2 步，先列表构造再移动或拷贝。
    // 但由于编译器的 拷贝省略（Copy Elision） 优化（C++17 起强制要求），临时对象通常会被消除，直接调用 std::initializer_list 构造函数。

    // 2. assign
    vec1.assign(5, 10); // 创建一个大小为 5，所有元素值为 10 的 vector
    printVector("vec.assign(5, 10): ", vec1);
    vec1.assign({1, 2, 3, 2, 1}); // 使用初始化列表赋值
    printVector("vec.assign({1, 2, 3, 4, 5}): ", vec1);
        // 可以直接从其他容器（如数组、std::list、std::array 等）赋值。
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    vec1.assign(arr.begin(), arr.end());
    printVector("vec.assign(arr.begin(), arr.end()): ", vec1);

    // 3. 元素访问
    std::cout << "vec[0]: " << vec5[0] << std::endl;           // 通过下标访问
    std::cout << "vec.at(2): " << vec5.at(2) << std::endl;     // 通过 at() 访问（带边界检查）
    std::cout << "vec.front(): " << vec5.front() << std::endl; // 访问第一个元素
    std::cout << "vec.back(): " << vec5.back() << std::endl;   // 访问最后一个元素
    std::cout << "vec.data(): " << vec5.data() << std::endl;   // 返回底层指针
    printArray(vec5.data(), vec5.size());

    // 4. 容量
    std::cout << "vec.size(): " << vec5.size() << std::endl;           // 当前大小
    std::cout << "vec.max_size(): " << vec5.max_size() << std::endl;   // 最大元素数
    std::cout << "vec.capacity(): " << vec5.capacity() << std::endl;   // 当前容量
    std::cout << "vec.empty(): " << (vec5.empty() ? "true" : "false") << std::endl;    // 是否为空

    // 5. 修改器
    vec5.push_back(6);  // 在末尾添加元素
    vec5.pop_back();    // 移除最后一个元素
    vec5.insert(vec5.begin() + 2, 10);  // 在索引2处插入元素10
    vec5.erase(vec5.begin() + 1);       // 移除索引1的元素
    vec5.resize(7, 0);                  // 调整大小，新元素用0填充
    vec5.shrink_to_fit();               // 释放多余内存
    printVector("After modifications:\r\n", vec5);

    // 6. 迭代器
    printVector("Iterating over vec5 using range-based for loop:\r\n", vec5);

    std::cout << "Iterating over vec5 using iterators:" << std::endl;
    for (auto it = vec5.begin(); it != vec5.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 7. 排序和反转
    std::sort(vec5.begin(), vec5.end());    // 排序
    std::reverse(vec5.begin(), vec5.end()); // 反转
    printVector("After sort and reverse:\r\n", vec5);

    // 8. 非成员函数
    std::vector<int> vec6 = {5, 4, 3, 2, 1};
    vec5.swap(vec6);        // 交换内容
    std::swap(vec5, vec6);  // 使用std::swap交换内容
    printVector("After swap with vec6:\r\n", vec5);

    // 9. 清空
    vec5.clear(); // 清空所有元素
    std::cout << "vec5 is empty: " << (vec5.empty() ? "true" : "false") << std::endl;

    return 0;
}


/*
vec.assign(5, 10): 10 10 10 10 10
vec.assign({1, 2, 3, 4, 5}): 1 2 3 2 1
vec.assign(arr.begin(), arr.end()): 1 2 3 4 5
vec[0]: 1
vec.at(2): 3
vec.front(): 1
vec.back(): 5
vec.data(): 0xa00000470
vec.data() to arr[]: 1 2 3 4 5
vec.size(): 5
vec.max_size(): 2305843009213693951
vec.capacity(): 5
vec.empty(): false
After modifications:
1 10 3 4 5 0 0
Iterating over vec5 using range-based for loop:
1 10 3 4 5 0 0
Iterating over vec5 using iterators:
1 10 3 4 5 0 0
After sort and reverse:
10 5 4 3 1 0 0
After swap with vec6:
10 5 4 3 1 0 0
vec5 is empty: true
*/


void printVector(std::string des,std::vector<int>& vec)
{
    std::cout << des;
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

// 假设这是一个需要数组作为参数的函数
void printArray(const int* arr, size_t size)
{
    std::cout << "vec.data() to arr[]: ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}