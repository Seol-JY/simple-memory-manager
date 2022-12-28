#include <iostream>
#include <iomanip>
#include <map>
#include <windows.h>
#include "Memory.h"
#include "VisualMemory.h"
#include "testSheet.h"
using namespace std;

int main() {

    testSheet(); // 테스트 실행, 무시하려면 주석처리 할 것
    system("cls");

    int MaxSize = -1;
    int select = 0;
    string name;
    string size;
    map<int, int> mymap; int num;
    int TIPolicy; string TSPolicy;

    //모드, 임시이름
    cout << "                            ┌───────────────────────┐" << endl;
    cout << "                            │ Simple Memory Manager │" << endl;
    cout << "                            └───────────────────────┘" << endl;
    cout << left;
    cout << "┌────────────────────────────────────────────────────────────────────────────────┐" << endl;
    cout << "│ " << setw(79) << "이 프로그램은 동적할당과 동적해제에 대한 전반적인 이해를 위해 제작된" << "│" << endl;
    cout << "│ " << setw(79) << "'메모리 관리 시뮬레이션 프로그램' 입니다." << "│" << endl;
    cout << "└────────────────────────────────────────────────────────────────────────────────┘";

    cout << "\n\n\n'시뮬레이션'에서 사용될 가상의 '힙 영역'의 총량을 입력하세요.(Bytes)\n";

    while (true) {
        cout << " > ";
        cin >> MaxSize;
        if (!cin) {
            cout << "정수형만 입력 가능합니다. 다시 입력해주세요.\n";
            cin.clear();
            cin.ignore(100, '\n');
        }
        else if (MaxSize <= 0) {
            cout << "음수나 0으로 설정할 수 없습니다. 다시 입력해주세요.\n";
            cin.clear();
            cin.ignore(100, '\n');
        }
        else {
            cin.clear();
            cin.ignore(100, '\n');
            break;
        }
    }

    Memory memory(MaxSize);
    VisualMemory view(memory);
    //기본적으로 FIRST_FIT로 설정되어있음

    system("cls");

    while (select != 6) {
        TIPolicy = memory.getPlacementPolicy();
        if (TIPolicy == 1)  TSPolicy = "FIRST_FIT"; else if (TIPolicy == 2) TSPolicy = "BEST_FIT"; else if (TIPolicy == 3) TSPolicy = "WORST_FIT";
        cout << "┌──────┐" << endl;
        cout << "│ 메뉴 │" << endl;
        cout << "└──────┘" << endl;

        view.DisplayMemoryMeter();
        cout << "┌ Menu ──────────────┐" << endl;
        cout << "│ 1. 메모리 할당     │" << endl;
        cout << "│ 2. 메모리 해제     │" << endl;
        cout << "│ 3. 상태 출력       │" << endl;
        cout << "│ 4. 할당모드 변경   │" << endl;
        cout << "│ 5. 메모리 초기화   │" << endl;
        cout << "│ 6. 프로그램 종료   │" << endl;
        cout << "└────────────────────┘" << endl;
        cout << " \n메뉴를 입력하세요.\n";

        while (true) {
            cout << " > ";
            cin >> select;
            if (!cin) {
                cout << "메뉴에 해당하는 숫자만 입력해주세요!\n";
                cin.clear();
                cin.ignore(100, '\n');
            }
            else if (select < 1 || select > 6) {
                cout << "메뉴에 해당하는 숫자만 입력해주세요!\n";
                cin.clear();
                cin.ignore(100, '\n');
            }
            else {
                cin.clear();
                cin.ignore(100, '\n');
                break;
            }
        }

        system("cls");

        switch (select) {

        case 1://할당
            cout << "┌────────────────────┐" << endl;
            cout << "│ 메뉴 > 메모리 할당 │";
            cout << "                                           뒤로가려면 ; 입력" << endl;
            cout << "└────────────────────┘" << endl;
            view.DisplayMemoryMeter();
            //메모리게이지
            cout << "\n\n메모리를 할당합니다." << endl;
            cout << "메모리 할당함수 'malloc'의 형태는 다음과 같습니다." << endl;
            cout << "┌────────────────────────────────────────────────────────────────────────────────┐" << endl;
            cout << "│                 [type]* '변수명' = ([type]*)malloc('크기').                    │" << endl;
            cout << "└────────────────────────────────────────────────────────────────────────────────┘" << endl;
            cout << "\n변수명과 크기를 지정해 주세요. 변수명은 한 글자만 지정 가능합니다.\n\n";
            cout << "변수명: "; cin >> name;
            cin.clear();
            cin.ignore(100, '\n');
            //중복검사는 하지 않는다.
            //변수명에서 돌아가기 입력받은 경우

            while (name.length() != 1) {
                cout << "변수명은 문자 하나만 가능합니다!\n변수명: ";
                cin >> name;
                cin.clear();
                cin.ignore(100, '\n');
            }
            if (name.compare(";") == 0) { system("cls"); break; }
            //변수명 잘못 입력받은 경우 예외처리

            //크기에서 돌아가기 입력받은경우
            while (true) {
                cout << "크기: ";
                getline(cin, size);
                if (!size.compare(";")) { break; }
                else {
                    if (atoi(size.c_str()) <= 0) {
                        cout << "크기를 다시 입력하세요. 0보다 큰 정수만 입력 가능합니다.\n";
                    }
                    else { break; }
                }
            }
            if (!size.compare(";")) { system("cls"); break; }

            try {
                memory.MyMalloc(name[0], stoi(size));
            }
            catch (MemoryAllocException& e) {
                cout << "메모리 할당에 실패하였습니다! 연속된 메모리 공간이 존재하지 않습니다.\n" << endl;
                system("pause"); system("cls");
                break;
            }
            system("pause"); system("cls");
            //할당작업 한 후 메뉴로
            break;

        case 2://해제
            //할당된 것들의 이름들
            cout << "┌────────────────────┐" << endl;
            cout << "│ 메뉴 > 메모리 해제 │";
            cout << "                                           뒤로가려면 ; 입력" << endl;
            cout << "└────────────────────┘" << endl;
            view.DisplayAllocatedNames();

            cout << "\n\n메모리를 해제합니다." << endl;
            cout << "메모리 해제함수 'free'의 형태는 다음과 같습니다." << endl;
            cout << "┌────────────────────────────────────────────────────────────────────────────────┐" << endl;
            cout << "│                               free('변수명')                                   │" << endl;
            cout << "└────────────────────────────────────────────────────────────────────────────────┘" << endl;
            cout << "\n해제할 변수명을 입력해 주세요.\n\n";

            cout << "변수명: "; cin >> name;
            cin.clear();
            cin.ignore(100, '\n');

            while (name.length() != 1) {
                cout << "변수명은 문자 하나만 가능합니다!\n변수명: ";
                cin >> name;
                cin.clear();
                cin.ignore(100, '\n');
            }
            if (name.compare(";") == 0) { system("cls"); break; }

            try {
                memory.MyFree(name[0]);
                cout << name << " 메모리를 해제하였습니다." << endl;

                system("pause"); system("cls");

            }
            catch (MemoryFreeException& e) {
                cout << "해당 이름을 가진 메모리가 없어 해제에 실패하였습니다.\n" << endl;
                system("pause"); system("cls");
            }
            break;

        case 3://출력
            cout << "┌────────────────────┐" << endl;
            cout << "│ 메뉴 > 상태 출력   │" << endl;
            cout << "└────────────────────┘" << endl;


            view.DisplayMemoryBlock();
            view.DisplayMemoryMeter();
            view.DisplayAllocatedNames();
            view.DisplayMemoryInfo();
            cout << endl;
            cout << "┌ Node status ───────────────────────────────────────────────────────────────────┐" << endl;

            view.DisplayNodeStatus();


            system("pause"); system("cls"); break;

        case 4://할당모드 변경
            num = 1;
            cout << "┌──────────────────────┐" << endl;
            cout << "│ 메뉴 > 할당모드 변경 │";
            cout << "                                           뒤로가려면 ; 입력" << endl;
            cout << "└──────────────────────┘" << endl;
            cout << "\n현재 할당모드는 " << TSPolicy << "입니다.\n\n변경할 할당모드를 선택하세요." << endl;
            cout << "┌ policy ───────┐" << endl;

            if (TIPolicy != 1) { mymap.insert(pair<int, int>(num, FIRST_FIT)); cout << "│ " << num++ << " : FIRST_FIT │" << endl; }
            if (TIPolicy != 2) { mymap.insert(pair<int, int>(num, BEST_FIT)); cout << "│ " << num++ << " : BEST_FIT  │" << endl; }
            if (TIPolicy != 3) { mymap.insert(pair<int, int>(num, WORST_FIT)); cout << "│ " << num++ << " : WORST_FIT │" << endl; }
            cout << "└───────────────┘" << endl;

            cout << "\n메뉴를 고르세요.\n";
            while (true) {
                cout << " > ";
                getline(cin, size);
                if (!size.compare(";")) { break; }
                else {
                    if (atoi(size.c_str())==1 || atoi(size.c_str()) == 2) {
                        break;
                    }
                    else {
                        cout << "\n메뉴를 다시 고르세요.\n";
                    }
                }
            }
            if (!size.compare(";")) { system("cls"); break; }


            memory.setPlacementPolicy(mymap[stoi(size)]);
            mymap.clear(); 
            cout << name << "\n할당모드를 변경하였습니다." << endl;
            system("pause"); system("cls");
            break;

        case 5://초기화
            cout << "┌──────────────────────┐" << endl;
            cout << "│ 메뉴 > 메모리 초기화 │" << endl;
            cout << "└──────────────────────┘" << endl;
            cout << "\n\n현재 메모리 전체를 초기화 합니다. \n이 작업은 되돌릴 수 없습니다." << endl;
            cout << "\n정말 초기화하시겠습니까?(Y,n)\n > ";

            cin >> name;

            while (name.compare(";") && name.compare("n") && name.compare("Y")) {
                cout << "Y나 n만 입력하세요.\n > ";
                cin >> name;
                cin.clear();
                cin.ignore(100, '\n');
            }
            if (!name.compare(";")) { system("cls"); break; }
            else if (!name.compare("n")) {
                cout << "\n초기화를 취소하였습니다." << endl;
                system("cls"); break;
            }
            else if (!name.compare("Y")) {
                cout << "\n─ Reset progress ─────────────────────────────────────────────────────────────────\n" << endl;
                memory.cleanUp(1);
                cout << "\n──────────────────────────────────────────────────────────────────────────────────" << endl;
                cout << "메모리가 정상적으로 초기화 되었습니다.\n\n" << endl;
                view.DisplayMemoryMeter();
                view.DisplayMemoryInfo();
                system("pause"); system("cls");
            }
            break;
        case 6://종료
            select = 6;
        default:
            break;
        }
    }

    return 0;
}