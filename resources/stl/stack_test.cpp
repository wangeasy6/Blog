#include <iostream>
#include <stack>
#include <vector>  // 用于自定义底层容器

int main() {
    // 1. 构造函数
    std::stack<int> stack1; // 默认构造，使用默认容器（通常是 std::deque）

    // 使用 std::vector 作为底层容器
    std::stack<int, std::vector<int>> stack2;

    // 拷贝构造
    std::stack<int> stack3(stack1);

    // 移动构造
    std::stack<int> stack4(std::move(stack3));

    // 2. 元素访问
    stack1.push(10); // 添加元素
    stack1.push(20);
    stack1.push(30);

    std::cout << "Top element of stack1: " << stack1.top() << std::endl; // 访问栈顶元素

    // 3. 修改器
    stack1.pop(); // 移除栈顶元素
    std::cout << "Top element after pop: " << stack1.top() << std::endl;

    // 4. 容量
    std::cout << "Size of stack1: " << stack1.size() << std::endl; // 获取栈大小
    std::cout << "Is stack1 empty? " << (stack1.empty() ? "Yes" : "No") << std::endl;

    // 5. 非成员函数
    std::stack<int> stack5;
    stack5.push(1);
    stack5.push(2);

    stack1.swap(stack5); // 使用成员函数 swap
    std::swap(stack1, stack5); // 使用非成员函数 std::swap

    std::cout << "After swap, stack1 top: " << stack1.top() << std::endl;
    std::cout << "After swap, stack5 top: " << stack5.top() << std::endl;

    // 6. 清空栈
    while (!stack1.empty()) {
        stack1.pop();
    }
    std::cout << "Is stack1 empty after clearing? " << (stack1.empty() ? "Yes" : "No") << std::endl;

    return 0;
}