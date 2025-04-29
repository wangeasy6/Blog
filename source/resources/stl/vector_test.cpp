#include <iostream>
#include <vector>
#include <array>
#include <algorithm> // ���� std::sort �� std::reverse

void printVector(std::string des,std::vector<int>& vec);
void printArray(const int* arr, size_t size);

int main() {
    // 1. ���캯��
    std::vector<int> vec1;          // Ĭ�Ϲ���
    std::vector<int> vec2(5, 10);   // 5��Ԫ�أ���ʼֵΪ10
    std::vector<int> vec3(vec2);    // ��������
    std::vector<int> vec4(std::move(vec3));  // �ƶ�����
    std::vector<int> vec5{1, 2, 3, 4, 5}; // ��ʼ���б���
    // std::vector<int> vec5 = {1, 2, 3, 4, 5}; ��Ϊ 2 �������б������ƶ��򿽱���
    // �����ڱ������� ����ʡ�ԣ�Copy Elision�� �Ż���C++17 ��ǿ��Ҫ�󣩣���ʱ����ͨ���ᱻ������ֱ�ӵ��� std::initializer_list ���캯����

    // 2. assign
    vec1.assign(5, 10); // ����һ����СΪ 5������Ԫ��ֵΪ 10 �� vector
    printVector("vec.assign(5, 10): ", vec1);
    vec1.assign({1, 2, 3, 2, 1}); // ʹ�ó�ʼ���б�ֵ
    printVector("vec.assign({1, 2, 3, 4, 5}): ", vec1);
        // ����ֱ�Ӵ����������������顢std::list��std::array �ȣ���ֵ��
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    vec1.assign(arr.begin(), arr.end());
    printVector("vec.assign(arr.begin(), arr.end()): ", vec1);

    // 3. Ԫ�ط���
    std::cout << "vec[0]: " << vec5[0] << std::endl;           // ͨ���±����
    std::cout << "vec.at(2): " << vec5.at(2) << std::endl;     // ͨ�� at() ���ʣ����߽��飩
    std::cout << "vec.front(): " << vec5.front() << std::endl; // ���ʵ�һ��Ԫ��
    std::cout << "vec.back(): " << vec5.back() << std::endl;   // �������һ��Ԫ��
    std::cout << "vec.data(): " << vec5.data() << std::endl;   // ���صײ�ָ��
    printArray(vec5.data(), vec5.size());

    // 4. ����
    std::cout << "vec.size(): " << vec5.size() << std::endl;           // ��ǰ��С
    std::cout << "vec.max_size(): " << vec5.max_size() << std::endl;   // ���Ԫ����
    std::cout << "vec.capacity(): " << vec5.capacity() << std::endl;   // ��ǰ����
    std::cout << "vec.empty(): " << (vec5.empty() ? "true" : "false") << std::endl;    // �Ƿ�Ϊ��

    // 5. �޸���
    vec5.push_back(6);  // ��ĩβ���Ԫ��
    vec5.pop_back();    // �Ƴ����һ��Ԫ��
    vec5.insert(vec5.begin() + 2, 10);  // ������2������Ԫ��10
    vec5.erase(vec5.begin() + 1);       // �Ƴ�����1��Ԫ��
    vec5.resize(7, 0);                  // ������С����Ԫ����0���
    vec5.shrink_to_fit();               // �ͷŶ����ڴ�
    printVector("After modifications:\r\n", vec5);

    // 6. ������
    printVector("Iterating over vec5 using range-based for loop:\r\n", vec5);

    std::cout << "Iterating over vec5 using iterators:" << std::endl;
    for (auto it = vec5.begin(); it != vec5.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 7. ����ͷ�ת
    std::sort(vec5.begin(), vec5.end());    // ����
    std::reverse(vec5.begin(), vec5.end()); // ��ת
    printVector("After sort and reverse:\r\n", vec5);

    // 8. �ǳ�Ա����
    std::vector<int> vec6 = {5, 4, 3, 2, 1};
    vec5.swap(vec6);        // ��������
    std::swap(vec5, vec6);  // ʹ��std::swap��������
    printVector("After swap with vec6:\r\n", vec5);

    // 9. ���
    vec5.clear(); // �������Ԫ��
    std::cout << "vec5 is empty: " << (vec5.empty() ? "true" : "false") << std::endl;

    return 0;
}


/*
vec.assign(5, 10): 10 10 10 10 10
vec.assign({1, 2, 3, 4, 5}): 1 2 3 2 1
vec.assign(arr.begin(), arr.end()): 1 2 3 4 5
vec[0]: 1
vec.at(2): 3
vec.front(): 1
vec.back(): 5
vec.data(): 0xa00000470
vec.data() to arr[]: 1 2 3 4 5
vec.size(): 5
vec.max_size(): 2305843009213693951
vec.capacity(): 5
vec.empty(): false
After modifications:
1 10 3 4 5 0 0
Iterating over vec5 using range-based for loop:
1 10 3 4 5 0 0
Iterating over vec5 using iterators:
1 10 3 4 5 0 0
After sort and reverse:
10 5 4 3 1 0 0
After swap with vec6:
10 5 4 3 1 0 0
vec5 is empty: true
*/


void printVector(std::string des,std::vector<int>& vec)
{
    std::cout << des;
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

// ��������һ����Ҫ������Ϊ�����ĺ���
void printArray(const int* arr, size_t size)
{
    std::cout << "vec.data() to arr[]: ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}