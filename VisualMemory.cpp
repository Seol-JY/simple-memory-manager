#include "VisualMemory.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
VisualMemory::VisualMemory(Memory& memory) : memory_(memory) {}


void VisualMemory::DisplayNodeStatus() {

    int available_low = 0;
    int allocated_low = 0;
    Node* current;

    current = this->memory_.getAvailable();
    while (current != NULL) { available_low += 5; current = current->next; }
    current = this->memory_.getAllocated();
    while (current != NULL) { allocated_low += 6; current = current->next; }

    if (!allocated_low) { allocated_low++; }
    string* alloc_string = new string[allocated_low]{ "" };
    string* avail_string = new string[available_low]{ "" };


    cout << "           ┌────────────────────┐          ┌────────────────────┐" << endl;
    cout << "           │     available_     │          │     allocated_     │" << endl;
    cout << "           └─────────┬┬─────────┘          └─────────┬┬─────────┘" << endl;


    current = this->memory_.getAllocated();
    int alloc_row_cnt = 0;
    string tmp = "";
    while (current != NULL) {
        alloc_string[alloc_row_cnt++] += "          ↓↑          ";
        alloc_string[alloc_row_cnt++] += "┌─────────┴┴─────────┐";
        alloc_string[alloc_row_cnt] += "│ start:";
        for (int i = 0; i < 12 - to_string(current->start_).length(); i++) { alloc_string[alloc_row_cnt] += " "; }
        alloc_string[alloc_row_cnt++] += to_string(current->start_) + " │";
        alloc_string[alloc_row_cnt] += "│  size:";
        for (int i = 0; i < 12 - to_string(current->size_).length(); i++) { alloc_string[alloc_row_cnt] += " "; }
        alloc_string[alloc_row_cnt++] += to_string(current->size_) + " │";
        alloc_string[alloc_row_cnt] += "│  name:";
        tmp = current->name_;
        for (int i = 0; i < 12 - tmp.length(); i++) { alloc_string[alloc_row_cnt] += " "; }
        alloc_string[alloc_row_cnt++] += tmp + " │";
        if (current->next) { alloc_string[alloc_row_cnt++] += "└─────────┬┬─────────┘"; }
        else { alloc_string[alloc_row_cnt++] += "└────────────────────┘"; }
        current = current->next;
    }

    current = this->memory_.getAvailable();
    int avail_row_cnt = 0;
    while (current != NULL) {
        avail_string[avail_row_cnt++] += "          ↓↑          ";
        avail_string[avail_row_cnt++] += "┌─────────┴┴─────────┐";
        avail_string[avail_row_cnt] += "│ start:";
        for (int i = 0; i < 12 - to_string(current->start_).length(); i++) { avail_string[avail_row_cnt] += " "; }
        avail_string[avail_row_cnt++] += to_string(current->start_) + " │";
        avail_string[avail_row_cnt] += "│  size:";
        if (current->size_ == 0) {
            avail_string[avail_row_cnt] += " [GARBAGE]";
            for (int i = 0; i < 2 - to_string(current->size_).length(); i++) { avail_string[avail_row_cnt] += " "; }
        }
        else {
            for (int i = 0; i < 12 - to_string(current->size_).length(); i++) { avail_string[avail_row_cnt] += " "; }
        }
        avail_string[avail_row_cnt++] += to_string(current->size_) + " │";
        if (current->next) { avail_string[avail_row_cnt++] += "└─────────┬┬─────────┘"; }
        else { avail_string[avail_row_cnt++] += "└────────────────────┘"; }
        current = current->next;
    }

    for (int i = 0; i < min(allocated_low, available_low); i++) {
        cout << "           " << avail_string[i] << "          " << alloc_string[i] << endl;
    }
    if (allocated_low == available_low) { return; }
    for (int i = min(allocated_low, available_low); i < max(allocated_low, available_low); i++) {
        if (available_low > allocated_low) {
            cout << "           " << avail_string[i] << endl;
        }
        else {
            cout << "                                           " << alloc_string[i] << endl;
        }
    }

}

void VisualMemory::DisplayMemoryBlock() {
    char* map = new char[this->memory_.getCapacity()]{};
    for (int i = 0; i < this->memory_.getCapacity(); i++) {
        map[i] = ' ';
    }
    Node* current = this->memory_.getAllocated();
    while (current != NULL) {
        for (int i = 0; i < current->size_; i++) {
            map[current->start_ + i] = current->name_;
        }
        current = current->next;
    }
    int col = (this->memory_.getCapacity()-1) / 20 + 1;
    cout << "Memory Fragment Map:" << endl;
    cout << "┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐" << endl;


    for (int i = 0; i < col; i++) {
        if (i == col - 1) {
            cout << "│";
            for (int j = 20 * i; j <this->memory_.getCapacity(); j++) {
                if (map[j] != ' ') { cout << " "; }
                else { cout << " "; }
                cout << map[j];
                if (map[j] != ' ') { cout << " "; }
                else { cout << " "; }
                cout << "│";
            }
            for (int j = this->memory_.getCapacity(); j < 20 * i + 20; j++) {
                if (j == this->memory_.getCapacity()) {
                    cout << "EOM│";
                }
                else { cout << "NUL│"; }
            }
            cout << "\n└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘" << endl;
        } else {
            cout << "│";
            for (int j = 20 * i; j < 20 * i + 20; j++) {
                if (map[j] != ' ') { cout << " "; } else { cout << " "; }
                cout << map[j];
                if (map[j] != ' ') { cout << " "; } else { cout << " "; }
                cout << "│";
            }
            cout << "\n├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
        }
  
    }

    cout << endl;

}

void VisualMemory::DisplayMemoryInfo() {
    string arr[] = { "First-fit", "Best-fit", "Worst-fit" };
    string policy = arr[this->memory_.getPlacementPolicy() - 1];
    int hole_cnt = 0;
    int chunk_cnt = 0;
    Node* current = this->memory_.getAllocated();
    while (current != NULL) {
        chunk_cnt++;
        current = current->next;
    }
    current = this->memory_.getAvailable();
    while (current != NULL) {
        if (current->size_) { hole_cnt++; }
        current = current->next;
    }
    cout << right;
    cout << "┌ Memory Usage ─────────────────────────┐" << "┌ Memory Overview ──────────────────────┐" << endl;
    cout << "│      Capacity:" << setw(14) << this->memory_.getCapacity() << " Bytes    │" << "│ Placement Policy:" << setw(16) << policy << "     │" << endl;
    cout << "│     Available:" << setw(14) << this->memory_.getCapacity() - this->memory_.getUsed() << " Bytes    │" << "│      Chunk Count:" << setw(16) << chunk_cnt << "     │" << endl;
    cout << "│          Used:" << setw(14) << this->memory_.getUsed() << " Bytes    │" << "│       Hole Count:" << setw(16) << hole_cnt << "     │" << endl;
    cout << "└───────────────────────────────────────┘└───────────────────────────────────────┘" << endl;
    cout << endl;
    cout << left;

}

void VisualMemory::DisplayMemoryMeter() {
    double rate = (double)(this->memory_.getUsed() / (double)this->memory_.getCapacity());
    rate *= 100.0;
    string info = "";
    info += (to_string(this->memory_.getUsed()) + "Bytes / " + to_string(this->memory_.getCapacity()) + "Bytes,  ");
    cout << "┌ Meter ─────────────────────────────────────────────────────────────────────────┐" << endl;
    cout << "│[";

    int meter = (int)rate * 0.8;
    for (int i = 0; i < meter; i++) { cout << "■"; }
    for (int i = 0; i < 78 - meter; i++) { cout << "_"; }

    cout << "]│" << endl;
    cout << right;
    cout << "│ " << setw(72) << info;
    cout << left;
    cout << fixed;
    cout.precision(1);
    cout << "" << setw(5) << rate << "% │" << endl;
    cout << "└────────────────────────────────────────────────────────────────────────────────┘" << endl;

}

void VisualMemory::DisplayAllocatedNames() {
    string allocated_names = "";
    Node* current = this->memory_.getAllocated();
    while (current != NULL) {
        allocated_names += current->name_;
        allocated_names += " | ";
        current = current->next;
    }

    // 82
    cout << "┌ Allocated Names ───────────────────────────────────────────────────────────────┐" << endl;
    cout << left;
    cout << "│ " << setw(79) << allocated_names;
    cout << "│" << endl;
    cout << "└────────────────────────────────────────────────────────────────────────────────┘" << endl;
}