#ifndef Q3PRINTER_H
#define Q3PRINTER_H

#ifdef NOOUTPUT
#define PRINT( stmt )
#else
#define PRINT( stmt ) stmt
#endif // NOOUTPUT


#include <iostream>

_Monitor Printer {
    unsigned int total;
    unsigned int numStudents;
    unsigned int numVendingMachines;
    unsigned int numCouriers;

    struct MemberState{
        bool isFilled;

        char state;
        TallyVotes::Tour tour;
        TallyVotes::Ballot vote;
        unsigned int numBlocked, group;
    };
    MemberState *state_buffer; 

    void print_buffer();
  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, unsigned int value1 );
    void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
    void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );

  private:
    int kindToIdx(Kind kind, int lid = -1);
};

#endif