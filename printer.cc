#include "printer.h"

void Printer::print_buffer() {
    // logic to print everything in the buffers if they are currently filled (skipped if the buffer is not filled)
    for (int i=0; i<(int)total; i++) {
        if (!state_buffer[i].isFilled) {
            if (i != (int)total-1) cout << "\t";    // not the last one
            continue;
        }

        // S / V / B / U / b / D / C / G / X / T
        cout << state_buffer[i].state;
        switch (state_buffer[i].state) {
            case 'S':
                if (state_buffer[i].type == 'S') {
                    cout << state_buffer[i].f << "," << state_buffer[i].b;
                } else if (state_buffer[i].type == 'M') {
                    cout << state_buffer[i].c;
                }
                break;
            case 'E':
                cout << state_buffer[i].s << "," << state_buffer[i].g;
                break;
            case 'D':
                if (state_buffer[i].type == 'P' || state_buffer[i].type == 'G') {
                    cout << state_buffer[i].s << "," << state_buffer[i].g;
                } else {
                    cout << state_buffer[i].v << "," << state_buffer[i].r;
                }
                break;
            case 'F':
                if (state_buffer[i].type == 'P') {
                    cout << state_buffer[i].t;
                } else if (state_buffer[i].type == 'S') {
                    cout << state_buffer[i].d << ", " << state_buffer[i].a;
                }
                break;
            case 'W':
                break;
            case 'C':
                cout << state_buffer[i].s << " " << state_buffer[i].a;
                break;
            case 'T':
                cout << state_buffer[i].s << " " << state_buffer[i].a;
                break;
            case 'R':
                if (state_buffer[i].type == 'N') {
                    cout << state_buffer[i].v;
                } else if (state_buffer[i].type == 'T') {
                    cout << state_buffer[i].s;
                }
                break;
            case 'N':
                cout << state_buffer[i].s << " " << state_buffer[i].v;
                break;
            case 'P':
                if (state_buffer[i].type == 'T') {
                    cout << state_buffer[i].a;
                }
                break;
            case 'd':
                cout << state_buffer[i].v << " " << state_buffer[i].r;
                break;
            case 'U':
                cout << state_buffer[i].v << " " << state_buffer[i].b;
                break;
            case 'G':
                if (state_buffer[i].type == 'p') {
                    cout << state_buffer[i].b;
                } else {
                    cout << state_buffer[i].f << " " << state_buffer[i].b;
                }
                break;
            case 'V':
                cout << state_buffer[i].v;
                break;
            case 'B':
                if (state_buffer[i].type == 'S') {
                    cout << state_buffer[i].f << " " << state_buffer[i].b;
                } else {
                    cout << state_buffer[i].f << " " << state_buffer[i].r;
                }
                break;
            case 'A':
                if (state_buffer[i].type == 'S') {
                    cout << state_buffer[i].f;
                } 
                break;
            case 'X':
                break;
            case 'L':
                if (state_buffer[i].type == 'L') {
                    cout << state_buffer[i].s;
                }
                break;
            case 'r':
                break;
            case 't':
                cout << state_buffer[i].s << " " << state_buffer[i].a;
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
    for (int i=0; i<total; i++) {
        state_buffer[i].isFilled = false;
    }

    // indicate type of each index
    state_buffer[i].type = 'P';
    state_buffer[i].type = 'G';
    state_buffer[i].type = 'W';
    state_buffer[i].type = 'N';
    state_buffer[i].type = 'T';
    state_buffer[i].type = 'p';     // lower case p for Plant
    for (int i=6; i<6+numStudents; i++) {
        state_buffer[i].type = 'S';
    }
    for (int i=6+numStudents; i<6+numStudents+numVendingMachines; i++) {
        state_buffer[i].type = 'M';
    }
    for (int i=6+numStudents+numVendingMachines; i<total; i++) {
        state_buffer[i].type = 'C';
    }


    // print first two lines of the output table
    PRINT(
        cout << "Parent" << i << "\t";
        cout << "Gropoff" << i << "\t";
        cout << "WATOff" << i << "\t";
        cout << "Names" << i << "\t";
        cout << "Truck" << i << "\t";
        cout << "Plant" << i << "\t";

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
    )
}

// S/X
void Printer::print( unsigned int id, Voter::States state ) {
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

// C/G
void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour tour ) {
    print(id, state);       // call print first in case the buffer is currently filled
    state_buffer[id].tour = tour;
}

// V/T
void Printer::print( unsigned int id, Voter::States state, TallyVotes::Ballot vote ) {
    print(id, state);       // call print first in case the buffer is currently filled
    state_buffer[id].vote = vote;
}

// B/U
void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ) {
    print(id, state);       // call print first in case the buffer is currently filled
    state_buffer[id].numBlocked = numBlocked;
}

// b
void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked, unsigned int group ) {
    print(id, state);       // call print first in case the buffer is currently filled
    state_buffer[id].numBlocked = numBlocked;
    state_buffer[id].group = group;
}

Printer::~Printer() {
    print_buffer();
    delete [] state_buffer;
}
