#include <iostream>
#include <unordered_set>
#include <string>

int main() {
    // 1. ���캯��
    std::unordered_set<int> uset1; // Ĭ�Ϲ��죬�� unordered_set
    std::unordered_set<int> uset2 = {1, 2, 3, 4, 5}; // ��ʼ���б���
    std::unordered_set<int> uset3(uset2); // ��������
    std::unordered_set<int> uset4(std::move(uset3)); // �ƶ�����
    std::unordered_set<int> uset5(uset2.begin(), uset2.end()); // ��Χ����

    // 2. ����Ԫ��
    uset1.insert(10);
    uset1.insert(20);
    uset1.insert(30);
    uset1.insert(10); // �ظ�Ԫ�ز������

    std::cout << "uset1 contains: ";
    for (const auto& elem : uset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 3. ����Ԫ��
    auto it = uset1.find(20);
    if (it != uset1.end()) {
        std::cout << "Found element: " << *it << std::endl;
    } else {
        std::cout << "Element not found." << std::endl;
    }

    std::cout << "Count of 20 in uset1: " << uset1.count(20) << std::endl;

    // 4. ɾ��Ԫ��
    uset1.erase(20); // ɾ��Ԫ��
    std::cout << "After erasing 20, uset1 contains: ";
    for (const auto& elem : uset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    uset1.erase(it); // ɾ��������ָ���Ԫ��
    std::cout << "After erasing iterator, uset1 contains: ";
    for (const auto& elem : uset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 5. �����͹�ϣ�����
    std::cout << "uset1.size(): " << uset1.size() << std::endl;
    std::cout << "uset1.empty(): " << (uset1.empty() ? "true" : "false") << std::endl;
    std::cout << "uset1.max_size(): " << uset1.max_size() << std::endl;
    std::cout << "uset1.bucket_count(): " << uset1.bucket_count() << std::endl; // ��ϣͰ����
    std::cout << "uset1.load_factor(): " << uset1.load_factor() << std::endl; // ��������
    std::cout << "uset1.max_load_factor(): " << uset1.max_load_factor() << std::endl;

    uset1.rehash(10); // ���¹�ϣ��ָ��Ͱ����
    uset1.reserve(20); // Ԥ���ռ䣬����Ͱ���������ɸ���Ԫ��

    // 6. ���
    uset1.clear();
    std::cout << "After clear, uset1 is empty: " << (uset1.empty() ? "true" : "false") << std::endl;

    // 7. �Զ����ϣ����
    struct custom_hash {
        std::size_t operator()(const std::string& str) const {
            return std::hash<std::string>()(str);
        }
    };

    std::unordered_set<std::string, custom_hash> uset6 = {"apple", "banana", "cherry"};
    std::cout << "uset6 contains: ";
    for (const auto& elem : uset6) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. ��������
    std::unordered_set<std::string, custom_hash> uset7 = {"orange", "grape"};
    uset6.swap(uset7);
    std::cout << "After swap, uset6 contains: ";
    for (const auto& elem : uset6) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}