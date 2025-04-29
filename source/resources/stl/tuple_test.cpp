#include <iostream>
#include <tuple>
#include <string>
#include <utility> // for std::tie

int main() {
    // 1. ���캯��
    std::tuple<int, std::string, double> t1(42, "Hello", 3.14); // ֱ�ӹ���
    std::tuple<int, std::string, double> t2 = std::make_tuple(10, "World", 2.718); // ʹ�� make_tuple
    std::tuple<int, std::string, double> t3 = t2; // ��������
    std::tuple<int, std::string, double> t4 = std::move(t3); // �ƶ�����

    // 2. ����Ԫ��
    std::cout << "t1 contains: " << std::get<0>(t1) << ", " << std::get<1>(t1) << ", " << std::get<2>(t1) << std::endl;

    // ʹ�� std::tie ��� tuple
    int a;
    std::string b;
    double c;
    std::tie(a, b, c) = t2;
    std::cout << "t2 unpacked: " << a << ", " << b << ", " << c << std::endl;

    // ʹ�ýṹ���󶨣�C++17��
    auto [x, y, z] = t3;
    std::cout << "t3 unpacked (structured binding): " << x << ", " << y << ", " << z << std::endl;

    // 3. Ԫ�����ͺ�����
    std::cout << "Size of t1: " << std::tuple_size<decltype(t1)>::value << std::endl; // ��ȡ tuple �Ĵ�С
    std::cout << "Type of first element in t1: " << typeid(std::tuple_element_t<0, decltype(t1)>).name() << std::endl; // ��ȡ��һ��Ԫ�ص�����

    // 4. �޸�Ԫ��
    std::get<1>(t1) = "Modified"; // �޸ĵڶ���Ԫ��
    std::cout << "Modified t1: " << std::get<0>(t1) << ", " << std::get<1>(t1) << ", " << std::get<2>(t1) << std::endl;

    // 5. �Ƚ� tuple
    std::tuple<int, std::string> t5(1, "apple");
    std::tuple<int, std::string> t6(1, "banana");
    std::cout << "t5 == t6: " << (t5 == t6 ? "true" : "false") << std::endl;
    std::cout << "t5 < t6: " << (t5 < t6 ? "true" : "false") << std::endl;

    // 6. ʹ�� std::apply (C++17) ������ tuple
    auto print_tuple = [](const auto& t) {
        std::cout << "Tuple contains: ";
        std::apply([](const auto&... args) { ((std::cout << args << " "), ...); }, t);
        std::cout << std::endl;
    };

    print_tuple(t1);

    // 7. ʹ�� std::tuple_cat (C++17) ƴ�� tuple
    auto t7 = std::tuple_cat(t1, t2);
    std::cout << "t7 (concatenated): ";
    print_tuple(t7);

    return 0;
}