#include <iostream>
#include <stack>
#include <vector>  // �����Զ���ײ�����

int main() {
    // 1. ���캯��
    std::stack<int> stack1; // Ĭ�Ϲ��죬ʹ��Ĭ��������ͨ���� std::deque��

    // ʹ�� std::vector ��Ϊ�ײ�����
    std::stack<int, std::vector<int>> stack2;

    // ��������
    std::stack<int> stack3(stack1);

    // �ƶ�����
    std::stack<int> stack4(std::move(stack3));

    // 2. Ԫ�ط���
    stack1.push(10); // ���Ԫ��
    stack1.push(20);
    stack1.push(30);

    std::cout << "Top element of stack1: " << stack1.top() << std::endl; // ����ջ��Ԫ��

    // 3. �޸���
    stack1.pop(); // �Ƴ�ջ��Ԫ��
    std::cout << "Top element after pop: " << stack1.top() << std::endl;

    // 4. ����
    std::cout << "Size of stack1: " << stack1.size() << std::endl; // ��ȡջ��С
    std::cout << "Is stack1 empty? " << (stack1.empty() ? "Yes" : "No") << std::endl;

    // 5. �ǳ�Ա����
    std::stack<int> stack5;
    stack5.push(1);
    stack5.push(2);

    stack1.swap(stack5); // ʹ�ó�Ա���� swap
    std::swap(stack1, stack5); // ʹ�÷ǳ�Ա���� std::swap

    std::cout << "After swap, stack1 top: " << stack1.top() << std::endl;
    std::cout << "After swap, stack5 top: " << stack5.top() << std::endl;

    // 6. ���ջ
    while (!stack1.empty()) {
        stack1.pop();
    }
    std::cout << "Is stack1 empty after clearing? " << (stack1.empty() ? "Yes" : "No") << std::endl;

    return 0;
}