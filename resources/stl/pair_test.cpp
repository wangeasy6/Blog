#include <iostream>
#include <utility> // 包含 std::pair
#include <vector>
#include <map>

int main() {
    // 1. 构造函数
    std::pair<int, std::string> p1(1, "apple"); // 显式构造
    std::pair<int, std::string> p2 = {2, "banana"}; // 使用初始化列表
    std::pair<int, std::string> p3 = p2; // 拷贝构造
    std::pair<int, std::string> p4 = std::move(p3); // 移动构造

    // 2. 成员访问
    std::cout << "p1: (" << p1.first << ", " << p1.second << ")" << std::endl;
    std::cout << "p2: (" << p2.first << ", " << p2.second << ")" << std::endl;

    // 修改成员
    p1.first = 10;
    p1.second = "orange";
    std::cout << "After modification, p1: (" << p1.first << ", " << p1.second << ")" << std::endl;

    // 3. 比较操作
    std::cout << "p1 == p2: " << (p1 == p2 ? "true" : "false") << std::endl;
    std::cout << "p1 != p2: " << (p1 != p2 ? "true" : "false") << std::endl;
    std::cout << "p1 < p2: " << (p1 < p2 ? "true" : "false") << std::endl;
    std::cout << "p1 <= p2: " << (p1 <= p2 ? "true" : "false") << std::endl;
    std::cout << "p1 > p2: " << (p1 > p2 ? "true" : "false") << std::endl;
    std::cout << "p1 >= p2: " << (p1 >= p2 ? "true" : "false") << std::endl;

    // 4. 使用 std::make_pair
    auto p5 = std::make_pair(3, "grape");
    std::cout << "p5 (using std::make_pair): (" << p5.first << ", " << p5.second << ")" << std::endl;

    // 5. 使用 std::pair 在容器中
    std::vector<std::pair<int, std::string>> vec = {{4, "kiwi"}, {5, "mango"}};
    std::cout << "Vector of pairs:" << std::endl;
    for (const auto& pair : vec) {
        std::cout << "(" << pair.first << ", " << pair.second << ")" << std::endl;
    }

    // 6. 使用 std::pair 在关联容器中
    std::map<int, std::string> myMap = {{6, "pear"}, {7, "peach"}};
    std::cout << "Map of pairs:" << std::endl;
    for (const auto& pair : myMap) {
        std::cout << "(" << pair.first << ", " << pair.second << ")" << std::endl;
    }

    // 7. 交换
    p1.swap(p2);
    std::cout << "After swap, p1: (" << p1.first << ", " << p1.second << ")" << std::endl;
    std::cout << "After swap, p2: (" << p2.first << ", " << p2.second << ")" << std::endl;

    // 8. 使用 std::tie
    int a = 0;
    std::string b = "";
    std::tie(a, b) = p1; // 解构赋值
    std::cout << "After std::tie, a: " << a << ", b: " << b << std::endl;

    return 0;
}