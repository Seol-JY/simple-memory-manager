#pragma once
#define FIRST_FIT 1
#define BEST_FIT 2
#define WORST_FIT 3

struct Node {
    /* 잔여공간 또는 할당된 공간을 가지는 노드 구조체
     잔여 공간으로 사용할 경우 name 변수 사용하지 않음 */
    Node();
    Node(int size, int start);
    Node(int size, int start, char name);
    /* 구조체 포인터는 생성자에서 기본적으로 NULL로 초기화 */
    Node* next;
    Node* prev;
    int size_;
    int start_;
    char name_;
};

class Memory {
public:
    Memory();
    Memory(int capacity, int placement_policy = FIRST_FIT);
    ~Memory();
    int MyMalloc(char alloacte_name, int allocate_size);
    void MyFree(char free_name);
    void setPlacementPolicy(int placement_policy);
    void cleanUp(int resetFlag=0);

    Node* getAvailable();
    Node* getAllocated();
    int getPlacementPolicy();
    int getCapacity();
    int getUsed();

    void resetAvailable();
    void resetAllocated();

private:
    Node* available_;
    // 잔여 공간 노드의 Head
    Node* allocated_;
    // 할당된 공간 노드의 Head
    int placement_policy_;
    // 배치 정책
    int capacity_;
    // 메모리 전체 용량, 불변
    int used_;
    // 사용중인 용량
};

// 아래는 예외 클래스 선언
class MemoryException {
public:
    virtual void What() = 0;
};
class MemorySetException : public MemoryException {
private:
    int req;
public:
    MemorySetException(int e);
    void What();
};
class MemoryAllocException : public MemoryException {
private:
    int req;
public:
    MemoryAllocException(int e);
    void What();
};
class MemoryFreeException : public MemoryException {
private:
    char req;
public:
    MemoryFreeException(char e);
    void What();
};