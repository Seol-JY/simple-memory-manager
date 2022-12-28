#pragma once
bool testSheet();
int assertSetThrows(int capacity);
int assertAllocThrows(Memory& memory, char allocated_name, int allocated_size);
int assertFreeThrows(Memory& memory, char free_name);
int assertVariable(Memory& memory, int placement_policy, int capacity, int used);
int assertAllocNames(Memory& memory, char array[], int arrayLen);