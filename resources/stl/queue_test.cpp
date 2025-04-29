#include <iostream>
#include <queue>

int main() {
    // 1. ���캯��
    std::queue<int> q1; // Ĭ�Ϲ��죬�ն���
    std::queue<int> q2(q1); // ��������
    std::queue<int> q3(std::move(q2)); // �ƶ�����

    // 2. Ԫ�ط���
    q3.push(10); // ���Ԫ�ص�����
    q3.push(20);
    q3.push(30);

    std::cout << "Front element: " << q3.front() << std::endl; // ���ʶ���Ԫ��
    std::cout << "Back element: " << q3.back() << std::endl; // ���ʶ�βԪ��

    // 3. �޸���
    std::cout << "Popping elements:" << std::endl;
    while (!q3.empty()) {
        std::cout << q3.front() << " "; // �������Ԫ��
        q3.pop(); // �Ƴ�����Ԫ��
    }
    std::cout << std::endl;

    // 4. ����
    std::cout << "Is q3 empty? " << (q3.empty() ? "Yes" : "No") << std::endl; // �������Ƿ�Ϊ��

    // ����������
    q3.push(10);
    q3.push(20);
    q3.push(30);

    std::cout << "Size of q3: " << q3.size() << std::endl; // ��ȡ���д�С

    // 5. �ǳ�Ա����
    std::queue<int> q4;
    q4.push(40);
    q4.push(50);

    q3.swap(q4); // ʹ�ó�Ա���� swap ������������
    std::swap(q3, q4); // ʹ�÷ǳ�Ա���� std::swap ������������

    std::cout << "After swapping with q4:" << std::endl;
    while (!q3.empty()) {
        std::cout << q3.front() << " ";
        q3.pop();
    }
    std::cout << std::endl;

    // 6. ��ն���
    while (!q4.empty()) {
        q4.pop();
    }
    std::cout << "Is q4 empty? " << (q4.empty() ? "Yes" : "No") << std::endl;

    return 0;
}