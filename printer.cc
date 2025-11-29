#include "printer.h"
using namespace std; 

void Printer::print_buffer() {
    // logic to print everything in the buffers if they are currently filled (skipped if the buffer is not filled)
    for (int i=0; i<(int)total; i++) {
        if (!state_buffer[i].isFilled) {
            if (i != (int)total-1) cout << "\t";    // not the last one
            continue;
        }

        // print out the states
        cout << state_buffer[i].state;
        switch (state_buffer[i].state) {
            case 'S':
                if (state_buffer[i].type == 'S') {
                    cout << state_buffer[i].a << "," << state_buffer[i].b;
                } else if (state_buffer[i].type == 'M') {
                    cout << state_buffer[i].a;
                }
                break;
            case 'E':
                cout << state_buffer[i].a << "," << state_buffer[i].b;
                break;
            case 'D':
                if (state_buffer[i].type == 'P' || state_buffer[i].type == 'G') {
                    cout << state_buffer[i].a << "," << state_buffer[i].b;
                } else {
                    cout << state_buffer[i].a << "," << state_buffer[i].b;
                }
                break;
            case 'F':
                if (state_buffer[i].type == 'P') {
                    cout << state_buffer[i].a;
                } else if (state_buffer[i].type == 'S') {
                    cout << state_buffer[i].a << ", " << state_buffer[i].b;
                }
                break;
            case 'W':
                break;
            case 'C':
                cout << state_buffer[i].a << " " << state_buffer[i].b;
                break;
            case 'T':
                cout << state_buffer[i].a << " " << state_buffer[i].b;
                break;
            case 'R':
                if (state_buffer[i].type == 'N') {
                    cout << state_buffer[i].a;
                } else if (state_buffer[i].type == 'T') {
                    cout << state_buffer[i].a;
                }
                break;
            case 'N':
                cout << state_buffer[i].a << " " << state_buffer[i].b;
                break;
            case 'P':
                if (state_buffer[i].type == 'T') {
                    cout << state_buffer[i].a;
                }
                break;
            case 'd':
                cout << state_buffer[i].a << " " << state_buffer[i].b;
                break;
            case 'U':
                cout << state_buffer[i].a << " " << state_buffer[i].b;
                break;
            case 'G':
                if (state_buffer[i].type == 'p') {
                    cout << state_buffer[i].a;
                } else {
                    cout << state_buffer[i].a << " " << state_buffer[i].b;
                }
                break;
            case 'V':
                cout << state_buffer[i].a;
                break;
            case 'B':
                if (state_buffer[i].type == 'S') {
                    cout << state_buffer[i].a << " " << state_buffer[i].b;
                } else {
                    cout << state_buffer[i].a << " " << state_buffer[i].b;
                }
                break;
            case 'A':
                if (state_buffer[i].type == 'S') {
                    cout << state_buffer[i].a;
                } 
                break;
            case 'X':
                break;
            case 'L':
                if (state_buffer[i].type == 'L') {
                    cout << state_buffer[i].a;
                }
                break;
            case 'r':
                break;
            case 't':
                cout << state_buffer[i].a << " " << state_buffer[i].b;
                break;
        }

        if (i != (int)total-1) cout << "\t";        // not the last one
        state_buffer[i].isFilled = false;       // set to not filled => can freely overwrite
    }

    cout << endl;

}
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) : numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
    total = numStudents + numVendingMachines + numCouriers + 6;
    state_buffer = new MemberState[total];
    for (unsigned int i=0; i<total; i++) {
        state_buffer[i].isFilled = false;
    }

    // indicate type of each index
    state_buffer[0].type = 'P';
    state_buffer[1].type = 'G';
    state_buffer[2].type = 'W';
    state_buffer[3].type = 'N';
    state_buffer[4].type = 'T';
    state_buffer[5].type = 'p';     // lower case p for Plant
    for (unsigned int i=6; i<6+numStudents; i++) {
        state_buffer[i].type = 'S';
    }
    for (unsigned int i=6+numStudents; i<6+numStudents+numVendingMachines; i++) {
        state_buffer[i].type = 'M';
    }
    for (unsigned int i=6+numStudents+numVendingMachines; i<total; i++) {
        state_buffer[i].type = 'C';
    }


    // print first two lines of the output table

    cout << "Parent" << "\t";
    cout << "Gropoff" << "\t";
    cout << "WATOff" << "\t";
    cout << "Names" << "\t";
    cout << "Truck" << "\t";
    cout << "Plant" << "\t";

    for (int i=0; i<(int)numStudents; i++) {
        cout << "Stud" << i << "\t";
    }
    for (int i=0; i<(int)numVendingMachines; i++) {
        cout << "Mach" << i << "\t";
    }
    for (int i=0; i<(int)numCouriers; i++) {
        cout << "Cour" << i << "\t";
    }
    cout << endl;

    for (int i=0; i<(int)total; i++) {
        cout << "*******" << "\t";
    }
    cout << endl;
    
}


int Printer::kindToIdx(Kind kind, int lid) {
    switch (kind) {
        case Kind::Parent:
            return 0;
        case Kind::Groupoff:
            return 1;
        case Kind::WATCardOffice:
            return 2;
        case Kind::NameServer:
            return 3;
        case Kind::Truck:
            return 4;
        case Kind::BottlingPlant:
            return 5;
        case Kind::Student:
            return 6+lid;
        case Kind::Vending:
            return 6+numStudents+lid;
        case Kind::Courier:
            return 6+numStudents+numVendingMachines+lid;
    }

    return -1;      // should never happen
}

void Printer::print( Kind kind, char state ) {
    int id = kindToIdx(kind);
    // cout << "id " << id << " with state " << state << endl;
    if (!state_buffer[id].isFilled) {
        state_buffer[id].isFilled = true;
        state_buffer[id].state = state;
        return;
    }

    print_buffer();
    state_buffer[id].isFilled = true;
    state_buffer[id].state = state;
}

void Printer::print( Kind kind, char state, unsigned int value1 ) {
    print(kind, state);
    state_buffer[kindToIdx(kind)].a = value1;
}

void Printer::print( Kind kind, char state, unsigned int value1, unsigned int value2 ) {
    print(kind, state);
    state_buffer[kindToIdx(kind)].a = value1;
    state_buffer[kindToIdx(kind)].b = value2;
}


void Printer::print( Kind kind, unsigned int lid, char state ) {
    int id = kindToIdx(kind, lid);
    // cout << "id " << id << ", lid " << lid << " with state " << state << endl;
    if (!state_buffer[id].isFilled) {
        state_buffer[id].isFilled = true;
        state_buffer[id].state = state;
        return;
    }

    print_buffer();
    state_buffer[id].isFilled = true;
    state_buffer[id].state = state;
}
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1 ) {
    print(kind, lid, state);

    state_buffer[kindToIdx(kind, lid)].a = value1;
}
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ) {
    print(kind, lid, state);

    state_buffer[kindToIdx(kind, lid)].a = value1;
    state_buffer[kindToIdx(kind, lid)].b = value2;
}

Printer::~Printer() {
    print_buffer();
    delete [] state_buffer;
}
