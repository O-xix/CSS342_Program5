#ifndef BANK_H
#define BANK_H

#include <queue>
#include "bstree.h"
#include "transaction.h"
#include "account.h"

using namespace std;

class Bank {
public:
    Bank();

    //Reads transactions from a file and loads them into the queue
    bool LoadTransactions(const string& filename);

    //Processes transactions from the queue
    void ProcessTransactions();

    //Displays all accounts and their funds
    void DisplayAllAccounts() const;

private:
    queue<Transaction> transactions_;
    BSTree accounts_; 

    //Helper function to process a single transaction
    void ProcessTransaction(const Transaction& trans);
};

#endif
