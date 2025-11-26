#ifndef BANK_H_
#define BANK_H_


_Monitor Bank {
  unsigned int numStudents;     // keeping track of how many students account should be in the bank
  public:
	Bank( unsigned int numStudents );
    ~Bank();    // dtors 
	void deposit( unsigned int id, unsigned int amount );
	void withdraw( unsigned int id, unsigned int amount );
};

#endif