#ifndef STUDENT_H_
#define STUDENT_H_

_Monitor Printer;
_Task NameServer;
_Task WATCardOffice
_Task Groupoff

_Task Student {
    Printer & prt;
    NameServer & nameServer;
    WatCardOffice & cardOffice;
    Groupoff & groupoff;
    const unsigned int id, maxPurchases;

	void main();
  public:
	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
};

#endif