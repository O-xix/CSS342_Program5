#include <fstream>
#include <queue>
#include <iostream>

#include "bank.h"
#include "transaction.h"

Bank::Bank() : transactions_(), accounts_() {
    cout << "";
}

bool Bank::LoadTransactions(const string& filename) {
    //Open the file
    ifstream infile(filename); 
    if (!infile.is_open()) {
        cerr << "ERROR: Unable to open file " << filename << endl;
        return false;
    }

    //Transaction type
    char type; 
    //Read the first character of each line (transaction type)
    while (infile >> type) { 

        if (type == 'O') { //Open Account
            int account_id;
            string last_name, first_name;
            infile >> account_id >> last_name >> first_name;
            transactions_.emplace(type, account_id, last_name, first_name); 
        } 
        else if (type == 'D' || type == 'W') { //Deposit or Withdraw
            int account_id, fund_id, amount;
            infile >> account_id >> fund_id >> amount;
            if (amount < 0) {
                cerr << "ERROR: Deposit/Withdrawal amount " << amount << " cannot be negative.";
            }
            transactions_.emplace(type, account_id, fund_id, amount); //Deposit or Withdraw
        } 
        else if (type == 'T') { //Transfer
            int from_account, from_fund, to_account, to_fund, amount;
            infile >> from_account >> from_fund >> to_account >> to_fund >> amount;
            transactions_.emplace(type, from_account, from_fund, amount, to_account, to_fund); 
        } 
        else if (type == 'A') { //Account History
            int account_id;
            infile >> account_id;
            transactions_.emplace(type, account_id); 
        } 
        else if (type == 'F') { //Fund History
            int account_id, fund_id;
            infile >> account_id >> fund_id;
            transactions_.emplace(type, account_id, fund_id);
        } 
        else {
            cerr << "ERROR: Unknown transaction type '" << type << "' in file." << endl;
        }
    }

    infile.close();
    return true;
}

void Bank::ProcessTransactions() {
    while (!transactions_.empty()) {
        //Get the next transaction from the queue
        Transaction trans = transactions_.front();
        transactions_.pop();

        //Process the transaction
        ProcessTransaction(trans);
    }
}

void Bank::DisplayAllAccounts() const {
    cout << "FINAL BALANCES:" << endl;
    //In-order traversal
    accounts_.Display(); 
}

void Bank::ProcessTransaction(const Transaction& trans) {
    //cout << trans << endl;

    char type = trans.type();
    int account_id = trans.account_id();

    Account* account = nullptr;

    if (account_id < 0) {
        cerr << "ERROR: Accounts with negative id: " << account_id << "cannot exist in the system.";
        return;
    }

    switch (type) {
        //Open a new account
        case 'O': { 
            string first_name = trans.first_name();
            string last_name = trans.last_name();
            account = new Account(account_id, first_name, last_name);
            if (!accounts_.Insert(account)) {
                //Only way it fails is if it's already there
                cerr << "ERROR: Account " << account_id << " is already open. Transaction refused." << endl;
                //Avoid memory leaks
                delete account; 
            }
            break;
        }
        //Deposit to a fund
        case 'D': { 
            if (accounts_.Retrieve(account_id, account)) {
                if (!account->Deposit(trans.fund_id(), trans.amount(), false)) {
                    cerr << "ERROR: Failed to deposit" << trans.amount() << " to Account " << account_id << endl;
                }
            } 
            else {
                //Fails if cannot be found
                cerr << "ERROR: Account " << account_id << " not found." << endl;
            }
            break;
        }
        //Withdraw from a fund
        case 'W': { 
            if (accounts_.Retrieve(account_id, account)) {
                if (!account->Withdraw(trans.fund_id(), trans.amount(), false)) {
                    cerr << "ERROR: Not enough funds to withdraw " << trans.amount() << " from " << account->first_name() << " " << account->last_name() << " " << account->fund_name(trans.fund_id()) << endl;
                }
            } 
            else {
                cerr << "ERROR: Account " << account_id << " not found." << endl;
            }
            break;
        }
        case 'T': { 
            //Transfer between accounts/funds
            int to_account_id = trans.target_account();
            int to_fund_id = trans.target_fund();
            Account* to_account = nullptr;

            if (accounts_.Retrieve(account_id, account) && accounts_.Retrieve(to_account_id, to_account)) {
                if (!account->Transfer(trans.fund_id(), to_account, to_fund_id, trans.amount())) {
                    cerr << "ERROR: Transfer failed for Account " << account_id << endl;
                }
            } 
            else {
                if (!accounts_.Retrieve(account_id, account)) {
                    cerr << "ERROR: Account " << account_id << " not found. Transferal refused." << endl;
                }
                else if (!accounts_.Retrieve(to_account_id, to_account)) {
                    cerr << "ERROR: Account " << to_account_id << " not found. Transferal refused." << endl;
                }
                else {
                    cerr << "ERROR: Account " << account_id << " and account " << to_account_id << " not found. Transferal refused." << endl;
                }
            }
            break;
        }

        case 'A': { 
            //Display transaction history for the account
            if (accounts_.Retrieve(account_id, account)) {
                account->DisplayHistory();
            } else {
                cerr << "ERROR: Account " << account_id << " not found." << endl;
            }
            break;
        }
        case 'F': { 
            //Display transaction history for a specific fund
            if (accounts_.Retrieve(account_id, account)) {
                account->DisplayFundHistory(trans.fund_id());
            } else {
                cerr << "ERROR: Account " << account_id << " not found." << endl;
            }
            break;
        }

        default:
            cerr << "ERROR: Invalid transaction type " << type << endl;
            break;
    }
}



