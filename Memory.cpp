#include <iostream>
#include <string>
#include "Memory.h"
using namespace std;

// 구조체 생성자 정의
Node::Node() : next(NULL), prev(NULL) {};
Node::Node(int size, int start) : next(NULL), prev(NULL), size_(size), start_(start), name_(NULL) {};
Node::Node(int size, int start, char name) : next(NULL), prev(NULL), size_(size), start_(start), name_(name) {};

//클래스
// allocated는 NULL일 수 있지만, available에는 무조건 첫 노드가 존재해야 함
Memory::Memory(int capacity, int placement_policy) : used_(0), allocated_(NULL) {
    setPlacementPolicy(placement_policy);
    if (capacity <= 0) { throw MemorySetException(capacity); }
    this->capacity_ = capacity;
    // available_의 첫 노드를 생성 및 초기화
    this->available_ = new Node(this->capacity_, 0);
}
Memory::Memory() { Memory(100); }
Memory::~Memory() { cleanUp(); } // todo: 소멸자 구현해야함

int Memory::MyMalloc(char allocate_name, int allocate_size) {
    if (allocate_size <= 0) { throw MemoryAllocException(allocate_size); }
    Node* target_node = NULL;  // 할당 할 대상 hole을 가지는 변수
    Node* current = available_;
    switch (this->placement_policy_) {
    case FIRST_FIT:
        while (current != NULL) {
            // 할당할 할 적절한 hole이 존재하는 경우
            if (current->size_ >= allocate_size) {
                target_node = current;
                break;
            }
            current = current->next;
        }
        break;

    case BEST_FIT:
        while (current != NULL) {
            if (current->size_ >= allocate_size) {
                // 할당할 할 hole이 존재하는 경우
                if (target_node) {
                    // target_node가 존재하는 경우, target노드를 갱신할지에 대해
                    if (current->size_ < target_node->size_) {
                        target_node = current;
                    }
                }
                else { target_node = current; }
            }
            current = current->next;
        }
        break;

    case WORST_FIT:
        while (current != NULL) {
            if (current->size_ >= allocate_size) {
                // 할당할 할 hole이 존재하는 경우
                if (target_node) {
                    // target_node가 존재하는 경우, target노드를 갱신할지에 대해
                    if (current->size_ > target_node->size_) {
                        target_node = current;
                    }
                }
                else { target_node = current; }
            }
            current = current->next;
        }
        break;

    default:
        break;
    }
    if (!target_node) { throw MemoryAllocException(allocate_size); } // 할당할 hole이 없는 경우에 메모리 할당 예외 throw

    // 아래는 새로운 메모리 청크를 생성하고 반영하는 부분
    Node* new_chunk = new Node(allocate_size, target_node->start_, allocate_name);

    if (!allocated_) {
        allocated_ = new_chunk;
    }
    else {
        new_chunk->next = allocated_;
        allocated_->prev = new_chunk;
        allocated_ = new_chunk;
    }
    int start_point = target_node->start_;

    target_node->start_ += allocate_size;  // target hole의 사이즈와 시작주소를 재갱신
    target_node->size_ -= allocate_size;

    this->used_ += allocate_size; // 사용량 반영
    return start_point; // 청크 시작주소 반환
}

void Memory::MyFree(char free_name) {
    if (!allocated_) { throw MemoryFreeException(free_name); } // 할당된 메모리가 존재하지 않을 때
    Node* target_node = NULL;  // 해제 할 대상 Chunk를 임시 저장

    if (allocated_->name_ == free_name && allocated_->next == NULL) {
        // 할당된 메모리가 하나일 경우 그 해당 메모리 해제할 시
        target_node = allocated_; // 대상 chunk로 지정
        allocated_ = NULL;
    }
    else {
        // 할당된 메모리가 여러개일 경우
        Node* current = allocated_;
        while (current != NULL) {
            if (current->name_ == free_name) {
                // 해제할 메모리를 찾았을 때
                if (current->prev == NULL) {
                    // 첫번째 메모리 해제할 경우
                    allocated_ = current->next;
                    current->next->prev = NULL;
                }
                else if (!current->next) {
                    // 마지막 메모리 해제할 경우
                    current->prev->next = NULL;
                }
                else {
                    // 중간 메모리 해제할 경우
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }

                target_node = current; // 대상 chunk로 지정
                break;
            }
            current = current->next;
        }
    }

    if (!target_node) { throw MemoryFreeException(free_name); } // 해제할 chunk가 없는 경우에 메모리 해제 예외 throw

    Node* current = available_;
    Node* rear_of_target = NULL;
    Node* front_of_target = NULL;
    int opt = 0;
    while (current != NULL) {
        /*
         기존 남은 공간이 앞쪽으로 확장되는 경우 -> 1로 세트
         기존 남은 공간이 뒤쪽으로 확장되는 경우 -> 2로 세트
         기존 남은 공간이 양쪽으로 확장되는 경우 -> 3으로 세트
        */
        if ((target_node->start_ < current->start_) && (target_node->start_ + target_node->size_ == current->start_)) {
            rear_of_target = current;
            opt += 1;
        }
        if ((target_node->start_ > current->start_) && (current->start_ + current->size_ == target_node->start_)) {
            front_of_target = current;
            opt += 2;
        }
        current = current->next;
    }
    switch (opt) {
    case 1:
        // 공간이 앞쪽으로 확장되는경우
        rear_of_target->start_ = target_node->start_;
        rear_of_target->size_ += target_node->size_;
        this->used_ -= target_node->size_;
        delete target_node;
        return;
    case 2:
        // 공간이 뒤쪽으로 확장되는 경우
        front_of_target->size_ += target_node->size_;
        this->used_ -= target_node->size_;
        delete target_node;
        return;
    case 3:
        // 공간이 양쪽으로 확장되는 경우
        front_of_target->size_ += (target_node->size_ + rear_of_target->size_);
        rear_of_target->size_ = 0;
        this->used_ -= target_node->size_;
        delete target_node;
        return;
    case 0:
        /* 만약 hole이 연속되지 않는 경우, 해제할 노드를 그대로
         availabe에 삽입하여 빈공간으로 표현 */
        target_node->name_ = NULL;
        target_node->next = available_;
        available_->prev = target_node;
        available_ = target_node;
        this->used_ -= target_node->size_;   // 사용량 반영
        return;
    default:
        throw MemoryFreeException(opt); //
    }
}

void Memory::setPlacementPolicy(int placement_policy) {
    if (FIRST_FIT <= placement_policy && placement_policy <= WORST_FIT) {
        this->placement_policy_ = placement_policy;
    }
    else {
        throw MemorySetException(placement_policy);
    }
}

// reset
void Memory::resetAvailable() {
    this->available_ = new Node(this->capacity_, 0);
}
void Memory::resetAllocated() {
    this->allocated_ = NULL;
}

void Memory::cleanUp(int resetFlag) {
    //allocated_, available_ : 완전삭제
    Node* towardNode = this->allocated_;
    while (towardNode != NULL) {
        Node* targetNode = towardNode;
        towardNode = towardNode->next;
        if (resetFlag) cout << "메모리: " << targetNode->name_ << " 삭제 중..." << endl;
        delete(targetNode);
    }

    cout << endl;

    towardNode = this->available_;
    while (towardNode != NULL) {
        Node* targetNode = towardNode;
        towardNode = towardNode->next;
        if (resetFlag) cout << "잔여공간: " << targetNode->start_ << ", " << targetNode->size_ << " 반환 중..." << endl;
        delete(targetNode);
    }
    if (resetFlag) {
        this->resetAllocated();
        this->resetAvailable();
        this->used_ = 0;
    }
}

// getter
Node* Memory::getAvailable() { return this->available_; }
Node* Memory::getAllocated() { return this->allocated_; }
int Memory::getPlacementPolicy() { return this->placement_policy_; }
int Memory::getCapacity() { return this->capacity_; }
int Memory::getUsed() { return this->used_; }


// 아래는 예외 클래스 정의
MemorySetException::MemorySetException(int e) :req(e) {};
void MemorySetException::What() {
    cout << "메모리 설정 오류: 잘못된 인자가 들어온 경우입니다. " << "< " << this->req << " >" << endl;
}
MemoryAllocException::MemoryAllocException(int e) :req(e) {};
void MemoryAllocException::What() {
    cout << "메모리 할당 오류: 유효하지 않은 size이거나, 공간이 부족합니다. " << "< " << this->req << " >" << endl;
}
MemoryFreeException::MemoryFreeException(char e) :req(e) {};
void MemoryFreeException::What() {
    cout << "메모리 해제 오류: 해제할 메모리가 존재하지 않거나, 해제 과정 중 오류가 발생하였습니다." << "< " << this->req << " >" << endl;
}