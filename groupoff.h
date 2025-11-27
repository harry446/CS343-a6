#ifndef GROUPOFF_H_
#define GROUPOFF_H_

#include <vector>

#include "watCard.h"

using namespace std;

_Monitor Printer;

_Task Groupoff {
    struct Work {
        unsigned int id;
        WATCard::FWATCard giftCard;
        Work(unsigned int id): id(id) {}
    };
    Printer &prt; 
    const unsigned int numStudents; 
    const unsigned int sodaCost; 
    const unsigned int groupoffDelay; 
    unsigned int giftCardCnt = 0;

    bool * assigned;
    Work ** requests;  // for each student store their future "requests" 

    void main();
  public:
    Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    WATCard::FWATCard giftCard( unsigned int id );
};

#endif