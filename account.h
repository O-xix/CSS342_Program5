#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>
#include <vector>
#include "transaction.h"

using namespace std;

class Account {
public:
    Account(int id, const string& first_name, const string& last_name);

    int id() const;
    const string& first_name() const;
    const string& last_name() const;
    const string& fund_name(int fund_id) const;

    //Transaction operations
    bool Deposit(int fund_id, int amount, bool transfer);
    bool Withdraw(int fund_id, int amount, bool transfer);
    bool Transfer(int from_fund_id, Account* to_account, int to_fund_id, int amount);

    //Transaction history
    void AddTransaction(const Transaction& trans);
    void AddTransaction(const Transaction &trans, int to_fund_id);
    void DisplayHistory() const;
    void DisplayFundHistory(int fund_id) const;

    bool operator==(const Account& other) const;
    bool operator<(const Account& other) const;
    bool operator>(const Account& other) const;
    friend ostream& operator<<(ostream& os, const Account& account);

private:
    int id_ = -1;
    string first_name_ = "";
    string last_name_ = "";
    //Eight funds
    int funds_[8] = {0, 0, 0, 0, 0, 0, 0, 0}; 
    //Nine funds down here because I recall that you need to keep a log of failed transaction that applies to the account, btu couldn't apply to the fund.
    string fund_names_[9] = {"Money Market", "Prime Money Market", "Long-Term Bond", "Short-Term Bond", "500 Index Fund", "Capital Value Fund", "Growth Equity Fund", "Growth Index Fund", "Unknown/Failed Account Transactions"};
    vector<Transaction> transaction_history_[9];
};

#endif
