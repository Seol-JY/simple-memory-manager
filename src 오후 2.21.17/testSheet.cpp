
#include <iostream>
#include <iostream>
#include <iomanip>
#include <map>
#include <windows.h>
#include "Memory.h"
#include "VisualMemory.h"

using namespace std;

int assertSetThrows(int capacity);
int assertAllocThrows(Memory& memory, char allocated_name, int allocated_size);
int assertFreeThrows(Memory& memory, char free_name);
int assertVariable(Memory& memory, int placement_policy, int capacity, int used);
int assertAllocNames(Memory& memory, char array[], int arrayLen);


bool testSheet() {
   cout << "=============== Test ===============" << endl;
   int test_cnt = 0;
   int err_cnt = 0;

   // Test 1: 메모리 할당을 음수로 할 경우 예외발생
   cout << "\n[Test " << ++test_cnt << "]: ";
   if (assertSetThrows(-50)) {
      cout << "Success" << endl;
   }
   else {
      cout << "Failed" << endl;
      ++err_cnt;
   }

   // Test 2: 메모리 가용용량보다 큰 용량을 할당하려는 경우 예외발생
   Memory testMemory2(100);
   cout << "\n[Test " << ++test_cnt << "]: ";
   if (assertAllocThrows(testMemory2, 'a', 110)) {
      cout << "Success" << endl;
   }
   else {
      cout << "Failed" << endl;
      ++err_cnt;
   }

   // Test 3: 연속되는 공간이 없어 해당 메모리를 할당하지 못하는 경우 예외발생
   Memory testMemory3(100);
   testMemory3.MyMalloc('a', 10);
   testMemory3.MyMalloc('b', 20);
   testMemory3.MyMalloc('c', 30);
   testMemory3.MyMalloc('d', 20);
   testMemory3.MyMalloc('e', 20);
   testMemory3.MyFree('b');
   testMemory3.MyFree('d');
   cout << "\n[Test " << ++test_cnt << "]: ";  // 예외를 기대;
   if (assertAllocThrows(testMemory3, 'f', 30)) {
      cout << "Success" << endl;
   }
   else {
      cout << "Failed" << endl;
      ++err_cnt;
   }

   // Test 4: 존재하지 않는 메모리를 할당 해제하려 할 경우 예외발생
   Memory testMemory4(100);
   testMemory4.MyMalloc('a', 10);
   testMemory4.MyMalloc('b', 20);
   testMemory4.MyMalloc('c', 20);
   testMemory4.MyMalloc('d', 30);
   testMemory4.MyMalloc('e', 20);
   cout << "\n[Test " << ++test_cnt << "]: ";  // 예외를 기대;
   if (assertFreeThrows(testMemory4, 'f')) {
      cout << "Success" << endl;
   }
   else {
      cout << "Failed" << endl;
      ++err_cnt;
   }

   // Test 5: 메모리 할당과 해제 반복 후 정상적으로 할당되는지 테스트 - 1
   Memory testMemory5(100);
   testMemory5.MyMalloc('a', 10);
   testMemory5.MyMalloc('b', 20);
   testMemory5.MyMalloc('c', 20);
   testMemory5.MyMalloc('d', 30);
   testMemory5.MyMalloc('e', 20);
   testMemory5.MyFree('b');
   testMemory5.MyFree('c');
   testMemory5.MyMalloc('f', 30);
   testMemory5.MyFree('e');

   char expectedList5[] = { 'f', 'd', 'a' };

   cout << "\n[Test " << ++test_cnt << "]: ";
   if (assertVariable(testMemory5, FIRST_FIT, 100, 70) && assertAllocNames(testMemory5, expectedList5, 3)) {
      cout << "Success" << endl;
   }
   else {
      cout << "Failed" << endl;
      ++err_cnt;
   }

   // Test 6: 메모리 할당과 해제 반복 후 정상적으로 할당되는지 테스트 - 2
   Memory testMemory6(100);
   testMemory6.MyMalloc('a', 10);
   testMemory6.MyMalloc('b', 20);
   testMemory6.MyMalloc('c', 30);
   testMemory6.MyMalloc('d', 40);
   testMemory6.MyFree('c');
   testMemory6.MyMalloc('c', 20);

   char expectedList6[] = { 'c', 'd', 'b', 'a'};

   cout << "\n[Test " << ++test_cnt << "]: ";
   if (assertVariable(testMemory6, FIRST_FIT, 100, 90) && assertAllocNames(testMemory6, expectedList6, 4)) {
      cout << "Success" << endl;
   }
   else {
      cout << "Failed" << endl;
      ++err_cnt;
   }

   /*
    7~9 까지의 테스트는 배치정책이 알맞게 적용되었는지 확인하는 테스트,
    30, 20, 40크기의 홀을 차례로 만든 후, 20을 할당함.
    여기서 first-fit이면 30, best-fit이면 20 worst-fit이면 40의 위치에 할당 되어야함
   */
   
   // Test 7: First-Fit 
   Memory testMemory7(150);
   testMemory7.setPlacementPolicy(FIRST_FIT);
   testMemory7.MyMalloc('a', 20);
   testMemory7.MyMalloc('b', 40);
   testMemory7.MyMalloc('c', 10);
   testMemory7.MyMalloc('d', 20);
   testMemory7.MyMalloc('e', 10);
   testMemory7.MyMalloc('f', 30);
   testMemory7.MyMalloc('g', 20);
   testMemory7.MyFree('b');
   testMemory7.MyFree('d');
   testMemory7.MyFree('f');
   testMemory7.MyMalloc('b', 20);  // 크기가 20이던곳(f)에 할당되어야 함

   char expectedList7[] = { 'b', 'g', 'e', 'c', 'a' };

   cout << "\n[Test " << ++test_cnt << "]: ";
   if (assertVariable(testMemory7, FIRST_FIT, 150, 80) && assertAllocNames(testMemory7, expectedList7, 5)) {
      cout << "Success" << endl;
   }
   else {
      cout << "Failed" << endl;
      ++err_cnt;
   }

   // Test 8: Best-Fit 
   Memory testMemory8(150);
   testMemory8.setPlacementPolicy(BEST_FIT);
   testMemory8.MyMalloc('a', 20);
   testMemory8.MyMalloc('b', 40);
   testMemory8.MyMalloc('c', 10);
   testMemory8.MyMalloc('d', 20);
   testMemory8.MyMalloc('e', 10);
   testMemory8.MyMalloc('f', 30);
   testMemory8.MyMalloc('g', 20);
   testMemory8.MyFree('b');
   testMemory8.MyFree('d');
   testMemory8.MyFree('f');
   testMemory8.MyMalloc('b', 20);  // 크기가 20이던곳(f)에 할당되어야 함

   char expectedList8[] = { 'b', 'g', 'e', 'c', 'a' };

   cout << "\n[Test " << ++test_cnt << "]: ";
   if (assertVariable(testMemory8, BEST_FIT, 150, 80) && assertAllocNames(testMemory8, expectedList8, 5)) {
      cout << "Success" << endl;
   }
   else {
      cout << "Failed" << endl;
      ++err_cnt;
   }

   // Test 9: Worst-Fit 
   Memory testMemory9(150);
   testMemory9.setPlacementPolicy(WORST_FIT);
   testMemory9.MyMalloc('a', 20);
   testMemory9.MyMalloc('b', 40);
   testMemory9.MyMalloc('c', 10);
   testMemory9.MyMalloc('d', 20);
   testMemory9.MyMalloc('e', 10);
   testMemory9.MyMalloc('f', 30);
   testMemory9.MyMalloc('g', 20);
   testMemory9.MyFree('b');
   testMemory9.MyFree('d');
   testMemory9.MyFree('f');
   testMemory9.MyMalloc('b', 20);  // 크기가 20이던곳(f)에 할당되어야 함

   char expectedList9[] = { 'b', 'g', 'e', 'c', 'a' };

   cout << "\n[Test " << ++test_cnt << "]: ";
   if (assertVariable(testMemory9, WORST_FIT, 150, 80) && assertAllocNames(testMemory9, expectedList9, 5)) {
      cout << "Success" << endl;
   }
   else {
      cout << "Failed" << endl;
      ++err_cnt;
   }

   // Test 10~11: 문제의 시나리오
   // Test 10: 단편화로 인한 할당 실패 확인

   Memory testMemory10(100);
   testMemory10.MyMalloc('a', 10);
   testMemory10.MyMalloc('b', 20);
   testMemory10.MyMalloc('c', 10);
   testMemory10.MyFree('a');
   testMemory10.MyMalloc('a', 30);
   testMemory10.MyFree('c');
   testMemory10.MyMalloc('c', 10);

   
   cout << "\n[Test " << ++test_cnt << "]: ";  // 예외를 기대;
   if (assertAllocThrows(testMemory10, 'd', 40)) {
      cout << "Success" << endl;
   }
   else {
      cout << "Failed" << endl;
      ++err_cnt;
   }

   // Test 11: 문제 시나리오 정상 동작 확인
   testMemory10.MyFree('b');

   char expectedList10[] = { 'c', 'a' };
   cout << "\n[Test " << ++test_cnt << "]: ";
   if (assertVariable(testMemory10, FIRST_FIT, 100, 40) && assertAllocNames(testMemory10, expectedList10, 2)) {
      cout << "Success" << endl;
   }
   else {
      cout << "Failed" << endl;
      ++err_cnt;
   }


   cout << "\n\n============== Result ==============" << endl;
   if (!err_cnt) {
      cout << "Tests Success" << endl;
   } else {
      cout << "Tests Failed" << endl;
   }
   cout << "Test Count: "<< test_cnt << ", Error Count: " << err_cnt << endl;
   system("pause");
   return !(err_cnt);
}

int assertSetThrows(int capacity) {
   try {
      try { Memory tmp(capacity); }
      catch (MemorySetException& e) { return 1; }
   }
   catch (...) {
      cout << " expected: MemorySetException" << endl;
      cout << "  but was: Other Exception" << endl;
      return 0;
   }
   cout << " expected: MemorySetException" << endl;
   cout << "  but was: No Exception" << endl;
   return 0;
}

int assertAllocThrows(Memory& memory, char allocated_name, int allocated_size) {
   try {
      try { memory.MyMalloc(allocated_name, allocated_size); }
      catch (MemoryAllocException& e) { return 1; }
   }
   catch (...) {
      cout << " expected: MemoryAllocException" << endl;
      cout << "  but was: Other Exception" << endl;
      return 0;
   }
   cout << " expected: MemoryAllocException" << endl;
   cout << "  but was: No Exception" << endl;
   return 0;
}

int assertFreeThrows(Memory& memory, char free_name) {
   try {
      try { memory.MyFree(free_name); }
      catch (MemoryFreeException& e) { return 1; }
   }
   catch (...) {
      cout << " expected: MemoryFreeException" << endl;
      cout << "  but was: Other Exception" << endl;
      return 0;
   }
   cout << " expected: MemoryFreeException" << endl;
   cout << "  but was: No Exception" << endl;
   return 0;
}

int assertVariable(Memory& memory, int placement_policy, int capacity, int used) {
   int flag = 1;
   if (!(memory.getPlacementPolicy() == placement_policy)) {
      flag *= 0;
      cout << "\nPlacementPolicy:" << endl;
      cout << " expected: " << placement_policy << endl;
      cout << "  but was: " << memory.getPlacementPolicy() << endl;
   }
   if (!(memory.getCapacity() == capacity)) {
      flag *= 0;
      cout << "\nCapacity:" << endl;
      cout << " expected: " << capacity << endl;
      cout << "  but was: " << memory.getCapacity() << endl;
   }
   if (!(memory.getUsed() == used)) {
      flag *= 0;
      cout << "\nused:" << endl;
      cout << " expected: " << used << endl;
      cout << "  but was: " << memory.getUsed() << endl;
   }
   return flag;
}

int assertAllocNames(Memory& memory, char array[], int arrayLen) {
   int cnt = 0;
   int flag = 1;
   Node* current = memory.getAllocated();
   while (current != NULL) {
      if (cnt > arrayLen) {
         flag *= 0;
         break;
      }
      else if (current->name_ == array[cnt]) {
         cnt++;
         current = current->next;
      }
      else {
         flag *= 0;
         break;
      }
   }
   if (!(cnt == arrayLen)) {
      flag *= 0;
   }

   if (flag) return flag; // 플래그가 1이면 성공

   string expected = "[";
   for (int i = 0; i < arrayLen; i++) {
      expected += array[i];
      if(i != arrayLen-1) expected += ", ";
   }
   expected += "]";

   string allocated_names = "[";
   current = memory.getAllocated();
   while (current != NULL) {
      allocated_names += current->name_;
      current = current->next;
      if(current) allocated_names += ", ";
   }
   allocated_names += "]";

   cout << "\n expected: " << expected << endl;
   cout << "  but was: " << allocated_names << endl;

   return flag;
}