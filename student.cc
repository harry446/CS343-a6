#include "student.h"
#include "printer.h"
#include "nameServer.h"
#include "watCardOffice.h"
#include "groupoff.h"
#include "watCard.h"
#include "bottlingPlant.h"
#include "vendingMachine.h"


Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, unsigned int id, unsigned int maxPurchases ) : 
    prt(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases)  {}

void Student::main() {
    WATCard *card = nullptr;
    unsigned int totalDrinks = 0, totalFreeDrinks = 0;
    bool usedGiftcard = false;

    unsigned int purchases = prng(1, maxPurchases);     // [1, maxPurchases]
    unsigned int flavour = prng(BottlingPlant::Flavours::NUM_OF_FLAVOURS);     // 4 flavours, [0, 4)

    prt.print(Printer::Kind::Student, id, 'S', flavour, purchases);

    WATCard::FWATCard watCard = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard(id);

    VendingMachine * curMachine = nameServer.getMachine(id);
    prt.print(Printer::Kind::Student, id, 'V', curMachine->getId());

    for (unsigned int i=0; i<purchases; i++) {
        yield(prng(1, 10));

        for ( ;; ) {        // for loop to skip yield(prng(1, 10)) after Lost
            card = nullptr;
            try {
                _Enable{
                    _Select(giftCard) {
                        for ( ;; ) {        // for loop to skip yield(prng(1, 10)) after free bottle
                            try {
                                card = giftCard();
                                curMachine->buy((BottlingPlant::Flavours)flavour, *card);     // two possible exceptions: Free or Stock

                                prt.print(Printer::Kind::Student, id, 'G', flavour, 0);

                                delete card;        // completed one purchase with giftcard, cannot use it anymore, delete it and reset it
                                card = nullptr;
                                giftCard.reset();       // used giftcard, reset it to avoid more uses

                                totalDrinks++;
                                usedGiftcard = true;
                            } catch (VendingMachine::Free &) {      // free drink, gift card still has money => use it again
                                prt.print(Printer::Kind::Student, id, 'A', flavour);
                                if (prng(2) == 0) {     // 50% chance to watch ad, [0, 2)
                                    yield(4);
                                } else {
                                    prt.print(Printer::Kind::Student, id, 'X');
                                }

                                totalFreeDrinks++;
                                totalDrinks++;
                                continue;       // loop again, skip purchase yielding
                            } catch (VendingMachine::Stock &) {     // out of stock, call nameServer to get a new machine
                                VendingMachine * curMachine = nameServer.getMachine(id);
                                prt.print(Printer::Kind::Student, id, 'V', curMachine->getId());

                                // purchase attempted but not successful, do not count as a "purchase" but need to yield(prng(1, 10)) again
                                i--;        
                            }

                            break;
                        }
                    } or _Select(watCard) {
                        for ( ;; ) {        // for loop to skip yield(prng(1, 10)) after free bottle
                            try {
                                card = watCard();
                                curMachine->buy((BottlingPlant::Flavours)flavour, *card);         // three possible exceptions: Free or Stock orr Fund

                                prt.print(Printer::Kind::Student, id, 'B', flavour, card->getBalance());

                                totalDrinks++;
                            } catch (VendingMachine::Free &) {
                                prt.print(Printer::Kind::Student, id, 'A', flavour);
                                if (prng(2) == 0) {     // 50% chance to watch ad, [0, 2)
                                    yield(4);
                                } else {
                                    prt.print(Printer::Kind::Student, id, 'X');
                                }

                                totalFreeDrinks++;
                                totalDrinks++;
                                continue;       // loop again, skip purchase yielding
                            } catch (VendingMachine::Stock &) {
                                VendingMachine * curMachine = nameServer.getMachine(id);
                                prt.print(Printer::Kind::Student, id, 'V', curMachine->getId());

                                // purchase attempted but not successful, do not count as a "purchase" but need to yield(prng(1, 10)) again
                                i--;   
                            } catch (VendingMachine::Funds &) {
                                watCard = cardOffice.transfer(id, curMachine->cost() + 5, card);

                                // purchase attempted but not successful, do not count as a "purchase" but need to yield(prng(1, 10)) again
                                i--;
                            }


                            break;
                        }   // for ( ;; ); used to skip yield() when Free is thrown
                    }   // _Select
                }   // _Enable
            } catch (WATCardOffice::Lost &) {
                prt.print(Printer::Kind::Student, id, 'L');
                watCard = cardOffice.create(id, 5);
                // purchase attempted but not successful due to Fund
                // Fund transfer not successful due to Lost
                // DO NOT count as a "purchase" and DO NOT yield(prng(1, 10)) again
                continue;
            }
            // no Lost exception, may break to make next purchase
            break;
        }   // for ( ;; ); used to skip yield() when Lost is thrown
    } // for (int i=0; i<purchases; i++)


    // Handle memory leak/Lost card on edge case:
    // When student only buys 1 soda using the gift card,
    // without this block, watCard() never gets accessed, thus:
    //  - if it's lost, the exception is ignored
    //  - if it's successfully created, it doesn't get deleted and memory leak occurs
    // This block forces program to wait for watCard to get created successfully and assigns its pointer to card to get deleted
    // Does not cause any problems when purchases >= 2
    for ( ;; ) {
        try {
            _Enable{
                card = watCard();
                break;
            }
        } catch (WATCardOffice::Lost &) {
            prt.print(Printer::Kind::Student, id, 'L');
            watCard = cardOffice.create(id, 5);
        }
    }

    delete card;

    if (!usedGiftcard) {        // if giftcard was never used, need to wait for it and delete it
        card = giftCard();       
        delete card;
    }

    prt.print(Printer::Kind::Student, id, 'F', totalDrinks, totalFreeDrinks);
}