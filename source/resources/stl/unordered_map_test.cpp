#include <iostream>
#include <unordered_map>
#include <string>

int main() {
    // 1. ���캯��
    std::unordered_map<int, std::string> umap1; // Ĭ�Ϲ��죬�� unordered_map
    std::unordered_map<int, std::string> umap2 = {{1, "one"}, {2, "two"}, {3, "three"}}; // ��ʼ���б���
    std::unordered_map<int, std::string> umap3(umap2); // ��������
    std::unordered_map<int, std::string> umap4(std::move(umap3)); // �ƶ�����
    std::unordered_map<int, std::string> umap5(umap2.begin(), umap2.end()); // ��Χ����

    // 2. ������޸�Ԫ��
    umap1[1] = "apple"; // ������޸ļ�Ϊ1��ֵ
    umap1.insert({2, "banana"}); // �����ֵ��
    umap1.insert_or_assign(3, "cherry"); // C++17: ������޸ļ�Ϊ3��ֵ
    umap1.emplace(4, "date"); // ԭ�ع����ֵ��

    std::cout << "umap1 contains:" << std::endl;
    for (const auto& pair : umap1) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 3. ����Ԫ��
    auto it = umap1.find(2);
    if (it != umap1.end()) {
        std::cout << "Found element: " << it->first << " -> " << it->second << std::endl;
    } else {
        std::cout << "Element not found." << std::endl;
    }

    std::cout << "Count of key 2: " << umap1.count(2) << std::endl;

    // 4. ɾ��Ԫ��
    umap1.erase(2); // ɾ����Ϊ2��Ԫ��
    umap1.erase(it); // ɾ��������ָ���Ԫ��
    std::cout << "After erasing key 2, umap1 contains:" << std::endl;
    for (const auto& pair : umap1) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 5. �����͹�ϣ�����
    std::cout << "umap1.size(): " << umap1.size() << std::endl;
    std::cout << "umap1.empty(): " << (umap1.empty() ? "true" : "false") << std::endl;
    std::cout << "umap1.bucket_count(): " << umap1.bucket_count() << std::endl; // ��ϣͰ����
    std::cout << "umap1.load_factor(): " << umap1.load_factor() << std::endl; // ��������
    std::cout << "umap1.max_load_factor(): " << umap1.max_load_factor() << std::endl;

    umap1.rehash(10); // ���¹�ϣ��ָ��Ͱ����
    umap1.reserve(20); // Ԥ���ռ䣬����Ͱ���������ɸ���Ԫ��

    // 6. ���
    umap1.clear();
    std::cout << "After clear, umap1 is empty: " << (umap1.empty() ? "true" : "false") << std::endl;

    // 7. �Զ����ϣ����
    struct custom_hash {
        std::size_t operator()(const std::string& str) const {
            return std::hash<std::string>()(str);
        }
    };

    std::unordered_map<std::string, int, custom_hash> umap6 = {{"apple", 1}, {"banana", 2}};
    std::cout << "umap6 contains:" << std::endl;
    for (const auto& pair : umap6) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 8. ��������
    std::unordered_map<std::string, int, custom_hash> umap7 = {{"orange", 3}, {"grape", 4}};
    umap6.swap(umap7);
    std::cout << "After swap, umap6 contains:" << std::endl;
    for (const auto& pair : umap6) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}