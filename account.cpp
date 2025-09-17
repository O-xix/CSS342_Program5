#include "account.h"
#include <string>
#include <iostream>
using namespace std;

Account::Account(int id, const string &first_name, const string &last_name) {
    id_ = id;
    first_name_ = first_name;
    last_name_ = last_name;
}

int Account::id() const {
    return id_;
}

const string &Account::first_name() const {
    return first_name_;
}

const string &Account::last_name() const {
    return last_name_;
}

const string &Account::fund_name(int fund_id) const {
    return fund_names_[fund_id];
}

bool Account::Deposit(int fund_id, int amount, bool transfer) {
    //Should always be successful if the account was found
    if (fund_id < 8 && fund_id >= 0 && amount > 0) {
        funds_[fund_id] += amount;
        if (!transfer) {
            AddTransaction(Transaction('D', this->id_, fund_id, amount));
        }
        return true;
    }
    //If fund id doesn't correspond to a fund that exists or amount was false
    AddTransaction(Transaction('D', this->id_, fund_id, amount, 'F'));
    return true;
}

bool Account::Withdraw(int fund_id, int amount, bool transfer) {
    //If there are enough funds, then Withdraw normally.
    if (funds_[fund_id] >= amount) {
        funds_[fund_id] -= amount;
        if (!transfer) {
            AddTransaction(Transaction('W', this->id_, fund_id, amount)); 
        }
        return true;
    }

    //Handle linked funds withdrawals
    if (fund_id < 4 && funds_[fund_id ^ 1] >= (amount - funds_[fund_id])) {
        int remaining = amount - funds_[fund_id];
        //Should work due to withdraw first from the linked fund and then transferring for transfers. For withdrawals, needs to show up twice.
        if (!transfer) {
            AddTransaction(Transaction('W', this->id_, fund_id, funds_[fund_id]));
        }
        funds_[fund_id] = 0;
        AddTransaction(Transaction('W', this->id_, (fund_id ^ 1), remaining));
        funds_[fund_id ^ 1] -= remaining;
        return true;
    }

    //If not a transfer, put failed Withdrawal in fund history
    if (!transfer) {
        AddTransaction(Transaction('W', this->id_, fund_id, amount, 'F')); 
    }
    return false;
}


bool Account::Transfer(int from_fund, Account* to_account, int to_fund, int amount) {
    //Check for self-transfer
    char success = 'S';

    //Amount could be invalid or fund id could be invalid.
    if ((this == to_account && from_fund == to_fund) || (!((from_fund < 8) && (from_fund >= 0)) && !((to_fund < 8) && (to_fund >= 0)))) {
        //cerr << "ERROR: Cannot transfer to the same account and fund.\n";
        AddTransaction(Transaction('T', this->id_, from_fund, amount, to_account->id(), to_fund, 'F'));
        to_account->AddTransaction(Transaction('T', this->id_, from_fund, amount, to_account->id(), to_fund, 'F'), to_fund);
        return false;
    }

    //Attempt withdrawal (special handling for transfer)
    if (!Withdraw(from_fund, amount, true)) {
        AddTransaction(Transaction('T', this->id_, from_fund, amount, to_account->id(), to_fund, 'F')); 
        return false; //Insufficient funds
    }

    //Attempt deposit to the target account
    if (!to_account->Deposit(to_fund, amount, true)) {
        cerr << "ERROR: Transfer failed due to deposit issue in target account.\n";
        return false;
    }

    //Add successful transfer in both accounts
    AddTransaction(Transaction('T', this->id_, from_fund, amount, to_account->id(), to_fund));
    to_account->AddTransaction(Transaction('T', this->id_, from_fund, amount, to_account->id(), to_fund), to_fund);
    return true;
}


void Account::AddTransaction(const Transaction &trans) {
    if (!(trans.fund_id() < 8 && trans.fund_id() >= 0)) {
        //Push failed transactions that are out of fund_id to 9th fund
        transaction_history_[8].push_back(trans);
        transaction_history_[8].back().SetSuccess('F');
        return;
    }
    transaction_history_[trans.fund_id()].push_back(trans);
}

void Account::AddTransaction(const Transaction &trans, int to_fund_id) {
    transaction_history_[to_fund_id].push_back(trans);
}

void Account::DisplayHistory() const {
    cout << "Transaction History for " << this->first_name_ << " " << this->last_name_ << " by fund." << endl;
    /**
     * @todo Maybe should do something if there is no history.
     */
    for (int i = 0; i < 8; i++) {
        if (transaction_history_[i].size() == 0) {
            continue;
        }
        cout << fund_names_[i] << ": $" << this->funds_[i] << endl;
        for (int j = 0; j < transaction_history_[i].size(); j++) {
            cout << "\t" << transaction_history_[i][j] << endl;
        }
    }
    cout << endl;
}

void Account::DisplayFundHistory(int fund_id) const {
    if (fund_id < 0 || fund_id >= 8) {
        cerr << "ERROR: Invalid fund ID " << fund_id << endl;
        return;
    }
    cout << "Transaction History for " << first_name_ << " " << last_name_ << " " << fund_names_[fund_id] << ": ";
    cout << "$" << funds_[fund_id] << endl;

    vector<Transaction> fund_history = transaction_history_[fund_id];

    if (fund_history.size() == 0) {
        cout << "No transactions for this fund.\n";
        return;
    }

    for (Transaction transaction : fund_history) {
        cout << "  " << transaction << endl; //Assuming Transaction overloads operator<<
    }

    cout << endl;
}

bool Account::operator==(const Account& other) const {
    return this->id_ == other.id_;
}

bool Account::operator<(const Account& other) const {
    return this->id_ < other.id_;
}

bool Account::operator>(const Account& other) const {
    return this->id_ > other.id_;
}

ostream& operator<<(ostream& os, const Account& account) {
    os << account.first_name_ << " " << account.last_name_ << " Account ID: " << account.id() << endl;
    for (size_t i = 0; i < 8; ++i) {
        os << "\t\t" << account.fund_names_[i] << ": $" << account.funds_[i] << "\n";
    }
    return os;
}
