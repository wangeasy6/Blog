#include <iostream>
#include <queue>

int main() {
    // 1. 构造函数
    std::queue<int> q1; // 默认构造，空队列
    std::queue<int> q2(q1); // 拷贝构造
    std::queue<int> q3(std::move(q2)); // 移动构造

    // 2. 元素访问
    q3.push(10); // 添加元素到队列
    q3.push(20);
    q3.push(30);

    std::cout << "Front element: " << q3.front() << std::endl; // 访问队首元素
    std::cout << "Back element: " << q3.back() << std::endl; // 访问队尾元素

    // 3. 修改器
    std::cout << "Popping elements:" << std::endl;
    while (!q3.empty()) {
        std::cout << q3.front() << " "; // 输出队首元素
        q3.pop(); // 移除队首元素
    }
    std::cout << std::endl;

    // 4. 容量
    std::cout << "Is q3 empty? " << (q3.empty() ? "Yes" : "No") << std::endl; // 检查队列是否为空

    // 重新填充队列
    q3.push(10);
    q3.push(20);
    q3.push(30);

    std::cout << "Size of q3: " << q3.size() << std::endl; // 获取队列大小

    // 5. 非成员函数
    std::queue<int> q4;
    q4.push(40);
    q4.push(50);

    q3.swap(q4); // 使用成员函数 swap 交换队列内容
    std::swap(q3, q4); // 使用非成员函数 std::swap 交换队列内容

    std::cout << "After swapping with q4:" << std::endl;
    while (!q3.empty()) {
        std::cout << q3.front() << " ";
        q3.pop();
    }
    std::cout << std::endl;

    // 6. 清空队列
    while (!q4.empty()) {
        q4.pop();
    }
    std::cout << "Is q4 empty? " << (q4.empty() ? "Yes" : "No") << std::endl;

    return 0;
}