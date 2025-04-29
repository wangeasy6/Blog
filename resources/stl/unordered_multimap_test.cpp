#include <iostream>
#include <unordered_map>
#include <string>

int main() {
    // 1. ���캯��
    std::unordered_multimap<std::string, int> ummap1; // Ĭ�Ϲ��죬�� unordered_multimap
    std::unordered_multimap<std::string, int> ummap2 = {{"apple", 1}, {"banana", 2}, {"apple", 3}}; // ��ʼ���б���
    std::unordered_multimap<std::string, int> ummap3(ummap2); // ��������
    std::unordered_multimap<std::string, int> ummap4(std::move(ummap3)); // �ƶ�����
    std::unordered_multimap<std::string, int> ummap5(ummap2.begin(), ummap2.end()); // ��Χ����

    // 2. ����Ԫ��
    ummap1.insert({"apple", 10});
    ummap1.insert({"banana", 20});
    ummap1.insert({"apple", 15}); // ������ظ�
    ummap1.insert({"orange", 30});

    std::cout << "ummap1 contains:" << std::endl;
    for (const auto& pair : ummap1) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 3. ����Ԫ��
    auto range = ummap1.equal_range("apple"); // ��ȡ��Ϊ "apple" ������Ԫ�ط�Χ
    std::cout << "Elements with key 'apple':" << std::endl;
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    auto it = ummap1.find("banana"); // ���Ҽ�Ϊ "banana" ��Ԫ��
    if (it != ummap1.end()) {
        std::cout << "Found key 'banana' with value: " << it->second << std::endl;
    }

    std::cout << "Count of key 'apple': " << ummap1.count("apple") << std::endl;

    // 4. ɾ��Ԫ��
    ummap1.erase("apple"); // ɾ�����м�Ϊ "apple" ��Ԫ��
    std::cout << "After erasing 'apple', ummap1 contains:" << std::endl;
    for (const auto& pair : ummap1) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 5. �����͹�ϣ�����
    std::cout << "ummap1.size(): " << ummap1.size() << std::endl;
    std::cout << "ummap1.empty(): " << (ummap1.empty() ? "true" : "false") << std::endl;
    std::cout << "ummap1.bucket_count(): " << ummap1.bucket_count() << std::endl; // ��ϣͰ����
    std::cout << "ummap1.load_factor(): " << ummap1.load_factor() << std::endl; // ��������
    std::cout << "ummap1.max_load_factor(): " << ummap1.max_load_factor() << std::endl;

    ummap1.rehash(10); // ���¹�ϣ��ָ��Ͱ����
    ummap1.reserve(20); // Ԥ���ռ䣬����Ͱ���������ɸ���Ԫ��

    // 6. ���
    ummap1.clear();
    std::cout << "After clear, ummap1 is empty: " << (ummap1.empty() ? "true" : "false") << std::endl;

    // 7. �Զ����ϣ����
    struct custom_hash {
        std::size_t operator()(const std::string& str) const {
            return std::hash<std::string>()(str);
        }
    };

    std::unordered_multimap<std::string, int, custom_hash> ummap6 = {{"apple", 1}, {"banana", 2}};
    std::cout << "ummap6 contains:" << std::endl;
    for (const auto& pair : ummap6) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 8. ��������
    std::unordered_multimap<std::string, int, custom_hash> ummap7 = {{"orange", 3}, {"grape", 4}};
    ummap6.swap(ummap7);
    std::cout << "After swap, ummap6 contains:" << std::endl;
    for (const auto& pair : ummap6) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}