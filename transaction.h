#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
using namespace std;

class Transaction {
public:
    // Constructor for 'O'
    Transaction(char type, int account_id, const string& last_name, const string& first_name);

    // Constructor for 'D' or 'W'
    Transaction(char type, int account_id, int fund_id, int amount);

    Transaction(char type, int account_id, int fund_id, int amount, char success);

    // Constructor for 'T'
    Transaction(char type, int from_account, int from_fund, int amount, int to_account, int to_fund, char success);

    Transaction(char type, int from_account, int from_fund, int amount, int to_account, int to_fund);

    // Constructor for 'A'
    Transaction(char type, int account_id);

    // Constructor for 'F'
    Transaction(char type, int account_id, int fund_id);

    void SetSuccess(char a);

    char type() const;
    int account_id() const;
    int fund_id() const;
    int amount() const;
    int target_account() const;
    int target_fund() const;
    char success() const;
    const string& last_name() const;  //For O transactions
    const string& first_name() const; //For O transactions

    friend ostream& operator<<(ostream& os, const Transaction& rhs);

private:
    char type_ = 'N'; // O, D, W, T, A, F
    int account_id_ = -1;
    int fund_id_ = -1;
    int amount_ = -1;
    int target_account_ = -1;
    int target_fund_ = -1;
    char success_ = 'N';

    string last_name_ = "No Last Name";
    string first_name_ = "No First Name";
};

#endif
