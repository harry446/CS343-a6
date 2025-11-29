#include "groupoff.h"

Groupoff::Groupoff(Printer& prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay): 
    prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
    requests = new Work*[numStudents];
}

Groupoff::~Groupoff() {
    delete [] requests;
}


WATCard::FWATCard Groupoff::giftCard(unsigned int id) {
    giftCardCnt++;
    // Create a new watcard future and store it in the futures vector of all the students value 
    Work *w = new Work(id);
    requests[id] = w;

    return w->giftCard;
}

void Groupoff::main() {
    ptr.print(Printer::Kind::Groupoff, 'S');

    unsigned int remaining = numStudents;
    for( ;; ) {
        _Accept(~Groupoff) {
            break;
        } _Else {
            if (giftCardCnt < numStudents) {        // first accept calls from all students to get a giftcard
                _Accept(giftCard);
            } else {
                if (remaining == 0) break;          // all student's gift card has been assigned for value 
                yield(groupoffDelay);  
                
                unsigned int idx = prng( remaining );   // pick a random unassigned student to assign a giftcard 
                unsigned int studentID = 0;
                for (unsigned int count = 0; count <= idx; studentID++) {       // Walk to the idx-th unassigned student
                    if ( !requests[studentID] ) count++;  
                }
                studentID--;        // complement the overshoot by one 

                Work *w = requests[studentID];

                WATCard card;
                card.deposit( sodaCost );

                //Deliver into the student's future ---
                prt.print( Printer::Groupoff, 'D', studentID, sodaCost );
                w->delivery( card );
                remaining--;

                delete w;
                requests[studentID] = nullptr;  // used above to identify if the request has been "fulfilled"
            }
        }
    }
    prt.print( Printer::Groupoff, 'F' );    
}

