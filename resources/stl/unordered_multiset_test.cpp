#include <iostream>
#include <unordered_set>
#include <string>

int main() {
    // 1. ���캯��
    std::unordered_multiset<int> umset1; // Ĭ�Ϲ��죬�� unordered_multiset
    std::unordered_multiset<int> umset2 = {1, 2, 3, 4, 5}; // ��ʼ���б���
    std::unordered_multiset<int> umset3(umset2); // ��������
    std::unordered_multiset<int> umset4(std::move(umset3)); // �ƶ�����
    std::unordered_multiset<int> umset5(umset2.begin(), umset2.end()); // ��Χ����

    // 2. ����Ԫ��
    umset1.insert(10);
    umset1.insert(20);
    umset1.insert(10); // �����ظ�Ԫ��
    umset1.insert(30);

    std::cout << "umset1 contains: ";
    for (const auto& elem : umset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 3. ����Ԫ��
    auto it = umset1.find(10);
    if (it != umset1.end()) {
        std::cout << "Found element: " << *it << std::endl;
    } else {
        std::cout << "Element not found." << std::endl;
    }

    std::cout << "Count of 10 in umset1: " << umset1.count(10) << std::endl;

    // 4. ɾ��Ԫ��
    umset1.erase(it); // ɾ��������ָ���Ԫ��
    std::cout << "After erasing iterator, umset1 contains: ";
    for (const auto& elem : umset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    umset1.erase(10); // ɾ������ֵΪ10��Ԫ��
    std::cout << "After erasing all 10s, umset1 contains: ";
    for (const auto& elem : umset1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 5. �����͹�ϣ�����
    std::cout << "umset1.size(): " << umset1.size() << std::endl;
    std::cout << "umset1.empty(): " << (umset1.empty() ? "true" : "false") << std::endl;
    std::cout << "umset1.bucket_count(): " << umset1.bucket_count() << std::endl; // ��ϣͰ����
    std::cout << "umset1.load_factor(): " << umset1.load_factor() << std::endl; // ��������

    umset1.rehash(10); // ���¹�ϣ��ָ��Ͱ����
    umset1.reserve(20); // Ԥ���ռ䣬����Ͱ���������ɸ���Ԫ��

    // 6. ���
    umset1.clear();
    std::cout << "After clear, umset1 is empty: " << (umset1.empty() ? "true" : "false") << std::endl;

    // 7. �Զ����ϣ����
    struct custom_hash {
        std::size_t operator()(const std::string& str) const {
            return std::hash<std::string>()(str);
        }
    };

    std::unordered_multiset<std::string, custom_hash> umset6 = {"apple", "banana", "apple"};
    std::cout << "umset6 contains: ";
    for (const auto& elem : umset6) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 8. ��������
    std::unordered_multiset<std::string, custom_hash> umset7 = {"orange", "grape"};
    umset6.swap(umset7);
    std::cout << "After swap, umset6 contains: ";
    for (const auto& elem : umset6) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}