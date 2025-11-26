#ifndef PARENT_H_
#define PARENT_H_

_Monitor Printer;
_Monitor Bank;


_Task Parent {
    Printer & prt; 
    Bank & bank; 
    unsigned int numStudents; 
    unsigned int parentalDelay; 
    unsigned int totalAmount;  
	void main();
  public:
	Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
};


#endif