#include <iostream>
#include <fstream>
#include <string>
#include "bank.h"

int main(int argc, char* argv[]) {
    //Check if file name is provided
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <transaction_file>" << endl;
        return 1;
    }
    //Create a Bank object to manage the system
    Bank bank;

    //Load transactions from the provided file
    string filename = argv[1];
    //string filename = "bank_trans_in.txt";
    if (!bank.LoadTransactions(filename)) {
        cerr << "Failed to load transactions from " << filename << endl;
        return 1;
    }

    //Process all the transactions
    bank.ProcessTransactions();

    //Display the final account balances
    bank.DisplayAllAccounts();

    return 0;
}
