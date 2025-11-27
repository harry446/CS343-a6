#include "groupoff.h"

Groupoff::Groupoff(Printer& prt, unsigned int numStudents,
                   unsigned int sodaCost, unsigned int groupoffDelay): prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {}


WATCard::FWATCard Groupoff::giftCard(unsigned int id) {
  // Create a new watcard future and store it in the futures vector of all the students value 
  WATCard::FWATCard watcardFuture;
  futures[id] = watcardFuture; 
  return watcardFuture;
}

void Groupoff::main() {
    ptr.print(Printer::Kind::Groupoff, 'S');

    for(int i = 0; i < numStudents; i ++) {
         _Accept(~Groupoff) { break; } or _Accept(giftCard);            // first accept the call from all students to get a giftcard
    }
    unsigned int remaining = numStudents;
    for(;;) {
        _Accept(~Groupoff) {
            break;
        } _Else {
            if (remaining == 0) break;          // all student's gift card has been assigned for value 
            yield(groupoffDelay);   
            unsigned int idx = prng( remaining - 1 );   // pick a random unassigned student to assign a giftcard 
            unsigned int studentID = 0;
            for ( unsigned int count = 0; count <= idx; studentID++ ) {       // Walk to the idx-th unassigned student
                if ( !assigned[studentID] ) count++;  
            }
            studentID--;        // complement the overshoot by one 
            WATCard *card = new WATCard();
            card->deposit( sodaCost );

            //Deliver into the student's future ---
            prt.print( Printer::Groupoff, 'D', studentID, sodaCost );
            futures[studentID].delivery( card );

            assigned[studentID] = true;
            remaining--;

        }   

    }
    prt.print( Printer::Groupoff, 'F' );    


}