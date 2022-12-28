#pragma once
#include "Memory.h"
#include <string>


class VisualMemory {
public:
    VisualMemory(Memory& memory);

    void DisplayNodeStatus();
    void DisplayMemoryBlock();
    void DisplayMemoryInfo();
    void DisplayMemoryMeter();
    void DisplayAllocatedNames();

private:
    Memory& memory_;
};