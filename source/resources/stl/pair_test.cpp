#include <iostream>
#include <utility> // ���� std::pair
#include <vector>
#include <map>

int main() {
    // 1. ���캯��
    std::pair<int, std::string> p1(1, "apple"); // ��ʽ����
    std::pair<int, std::string> p2 = {2, "banana"}; // ʹ�ó�ʼ���б�
    std::pair<int, std::string> p3 = p2; // ��������
    std::pair<int, std::string> p4 = std::move(p3); // �ƶ�����

    // 2. ��Ա����
    std::cout << "p1: (" << p1.first << ", " << p1.second << ")" << std::endl;
    std::cout << "p2: (" << p2.first << ", " << p2.second << ")" << std::endl;

    // �޸ĳ�Ա
    p1.first = 10;
    p1.second = "orange";
    std::cout << "After modification, p1: (" << p1.first << ", " << p1.second << ")" << std::endl;

    // 3. �Ƚϲ���
    std::cout << "p1 == p2: " << (p1 == p2 ? "true" : "false") << std::endl;
    std::cout << "p1 != p2: " << (p1 != p2 ? "true" : "false") << std::endl;
    std::cout << "p1 < p2: " << (p1 < p2 ? "true" : "false") << std::endl;
    std::cout << "p1 <= p2: " << (p1 <= p2 ? "true" : "false") << std::endl;
    std::cout << "p1 > p2: " << (p1 > p2 ? "true" : "false") << std::endl;
    std::cout << "p1 >= p2: " << (p1 >= p2 ? "true" : "false") << std::endl;

    // 4. ʹ�� std::make_pair
    auto p5 = std::make_pair(3, "grape");
    std::cout << "p5 (using std::make_pair): (" << p5.first << ", " << p5.second << ")" << std::endl;

    // 5. ʹ�� std::pair ��������
    std::vector<std::pair<int, std::string>> vec = {{4, "kiwi"}, {5, "mango"}};
    std::cout << "Vector of pairs:" << std::endl;
    for (const auto& pair : vec) {
        std::cout << "(" << pair.first << ", " << pair.second << ")" << std::endl;
    }

    // 6. ʹ�� std::pair �ڹ���������
    std::map<int, std::string> myMap = {{6, "pear"}, {7, "peach"}};
    std::cout << "Map of pairs:" << std::endl;
    for (const auto& pair : myMap) {
        std::cout << "(" << pair.first << ", " << pair.second << ")" << std::endl;
    }

    // 7. ����
    p1.swap(p2);
    std::cout << "After swap, p1: (" << p1.first << ", " << p1.second << ")" << std::endl;
    std::cout << "After swap, p2: (" << p2.first << ", " << p2.second << ")" << std::endl;

    // 8. ʹ�� std::tie
    int a = 0;
    std::string b = "";
    std::tie(a, b) = p1; // �⹹��ֵ
    std::cout << "After std::tie, a: " << a << ", b: " << b << std::endl;

    return 0;
}