#include <iostream>
#include <vector>
#include <deque>
#include <algorithm> // 用于 std::sort 和 std::reverse

void printDeque(std::string des, const std::deque<int>& deq)
{
    std::cout << des;
    for (const auto& elem : deq) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
    // 1. 构造函数
    std::deque<int> deque1; // 默认构造，空双端队列
    std::deque<int> deque2(5, 10);  // 5个元素，初始值为10
    std::deque<int> deque3(deque2); // 拷贝构造
    std::deque<int> deque4(std::move(deque3));  // 移动构造
    std::deque<int> deque5{1, 2, 3, 4, 5};      // 初始化列表构造

    // 2. assign
    deque1.assign(5, 10);  // 创建一个大小为 5，所有元素值为 10 的 deque
    printDeque("deque.assign(5, 10): ", deque1);
    deque1.assign({1, 2, 3}); // 使用初始化列表赋值
    printDeque("deque.assign({1, 2, 3}): ", deque1);
        // 从数组赋值
    int arr[] = {1, 2, 3, 4};
    deque1.assign(arr, arr + sizeof(arr) / sizeof(arr[0]));
    printDeque("deque.assign(arr, arr+4): ", deque1);
        // 从其他容器赋值
    std::vector<int> vec = {1, 2, 3, 4, 5};
    deque1.assign(vec.begin(), vec.end());
    printDeque("deque.assign(vec.begin(), vec.end()): ", deque1);

    // 3. 元素访问
    std::cout << "deque5[0]: " << deque5[0] << std::endl;           // 通过下标访问
    std::cout << "deque5.at(2): " << deque5.at(2) << std::endl;     // 通过at()访问（带边界检查）
    std::cout << "deque5.front(): " << deque5.front() << std::endl; // 访问第一个元素
    std::cout << "deque5.back(): " << deque5.back() << std::endl;   // 访问最后一个元素

    // 4. 容量
    std::cout << "deque5.size(): " << deque5.size() << std::endl;           // 当前大小
    std::cout << "deque5.max_size(): " << deque5.max_size() << std::endl;   // 可以容纳的最大元素数量
    std::cout << "deque5.empty(): " << (deque5.empty() ? "true" : "false") << std::endl; // 是否为空

    // 5. 迭代器
    std::cout << "Iterating over deque5 using iterators:" << std::endl;
    for (auto it = deque5.begin(); it != deque5.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    printDeque("Iterating over deque5 using range-based for loop:\r\n", deque5);

    // 6. 修改器
    deque5.push_back(6);                            // 在末尾添加元素
    printDeque("deque5.push_back(6): ", deque5);
    deque5.push_front(0);                           // 在开头添加元素
    printDeque("deque5.push_front(0): ", deque5);
    deque5.pop_back();                              // 移除最后一个元素
    printDeque("deque5.pop_back(): ", deque5);
    deque5.pop_front();                             // 移除第一个元素
    printDeque("deque5.pop_front(): ", deque5);
    deque5.emplace_front(10);                       // 在头部插入元素，直接调用的元素类型的构造函数
    printDeque("deque5.emplace_front(10): ", deque5);
    deque5.emplace_back(20);                        // 在尾部插入元素，直接调用的元素类型的构造函数
    printDeque("deque5.emplace_back(20): ", deque5);
    deque5.insert(deque5.begin() + 2, 10);          // 在索引2处插入元素10
    printDeque("deque5.insert(deque5.begin() + 2, 10): ", deque5);
    deque5.erase(deque5.begin() + 1);               // 移除索引1的元素
    printDeque("deque5.erase(deque5.begin() + 1): ", deque5);
    deque5.resize(10, 0);                           // 调整大小，新元素用0填充
    printDeque("deque5.resize(10, 0): ", deque5);
    deque5.shrink_to_fit();

    // 7. 排序和反转
    std::sort(deque5.begin(), deque5.end()); // 排序
    std::reverse(deque5.begin(), deque5.end()); // 反转

    std::cout << "After sort and reverse:" << std::endl;
    for (const auto& elem : deque5) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. 非成员函数
    std::deque<int> deque6 = {5, 4, 3, 2, 1};
    deque5.swap(deque6);        // 交换内容
    std::swap(deque5, deque6);  // 使用std::swap交换内容

    std::cout << "After swap with deque6:" << std::endl;
    for (const auto& elem : deque5) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 9. 清空
    deque5.clear(); // 清空所有元素
    std::cout << "deque5 is empty: " << (deque5.empty() ? "true" : "false") << std::endl;

    // 10. allocate
        // 获取 deque 的分配器
    std::allocator<int> alloc = deque1.get_allocator();
        // 使用分配器分配内存
    int* ptr = alloc.allocate(3); // 分配 3 个 int 类型的内存
        // 使用分配器构造对象
    alloc.construct(ptr + 0, 10);
    alloc.construct(ptr + 1, 20);
    alloc.construct(ptr + 2, 30);
        // 输出分配的内存中的值
    std::cout << "Allocated memory values: ";
    for (int i = 0; i < 3; ++i) {
        std::cout << ptr[i] << " ";
    }
    std::cout << "\n";
        // 使用分配器销毁对象
    alloc.destroy(ptr + 0);
    alloc.destroy(ptr + 1);
    alloc.destroy(ptr + 2);
        // 使用分配器释放内存
    alloc.deallocate(ptr, 3);

    return 0;
}

/*
deque.assign(5, 10): 10 10 10 10 10
deque.assign({1, 2, 3}): 1 2 3
deque.assign(arr, arr+4): 1 2 3 4
deque.assign(vec.begin(), vec.end()): 1 2 3 4 5
deque5[0]: 1
deque5.at(2): 3
deque5.front(): 1
deque5.back(): 5
deque5.size(): 5
deque5.max_size(): 2305843009213693951
deque5.empty(): false
Iterating over deque5 using iterators:
1 2 3 4 5
Iterating over deque5 using range-based for loop:
1 2 3 4 5
deque5.push_back(6): 1 2 3 4 5 6
deque5.push_front(0): 0 1 2 3 4 5 6
deque5.pop_back(): 0 1 2 3 4 5
deque5.pop_front(): 1 2 3 4 5
deque5.emplace_front(10): 10 1 2 3 4 5
deque5.emplace_back(20): 10 1 2 3 4 5 20
deque5.insert(deque5.begin() + 2, 10): 10 1 10 2 3 4 5 20
deque5.erase(deque5.begin() + 1): 10 10 2 3 4 5 20
deque5.resize(10, 0): 10 10 2 3 4 5 20 0 0 0
After sort and reverse:
20 10 10 5 4 3 2 0 0 0
After swap with deque6:
20 10 10 5 4 3 2 0 0 0
deque5 is empty: true
Allocated memory values: 10 20 30
*/