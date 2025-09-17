#include "transaction.h"
#include <iostream>
using namespace std;

//Open Account transaction ('O')
Transaction::Transaction(char type, int account_id, const string& last_name, const string& first_name)
    : type_(type), account_id_(account_id), fund_id_(-1), amount_(0), target_account_(-1), target_fund_(-1),
      last_name_(last_name), first_name_(first_name) {}

//Deposit or Withdraw transaction ('D' or 'W')
Transaction::Transaction(char type, int account_id, int fund_id, int amount)
    : type_(type), account_id_(account_id), fund_id_(fund_id), amount_(amount), target_account_(-1), target_fund_(-1) {
    //For Deposit and Withdraw, the amount is required
}

//Another for success (lame way, but works)
Transaction::Transaction(char type, int account_id, int fund_id, int amount, char success)
    : type_(type), account_id_(account_id), fund_id_(fund_id), amount_(amount), target_account_(-1), target_fund_(-1), success_(success) {
}

//Transfer transaction ('T')
//With success (lame way, but works)
Transaction::Transaction(char type, int from_account, int from_fund, int amount, int to_account, int to_fund, char success)
    : type_(type), account_id_(from_account), fund_id_(from_fund), amount_(amount), target_account_(to_account), target_fund_(to_fund), success_(success) {
    //Transfer transactions require both source and destination account/fund info
}

//Another for no success (lame way, but works)
Transaction::Transaction(char type, int from_account, int from_fund, int amount, int to_account, int to_fund)
    : type_(type), account_id_(from_account), fund_id_(from_fund), amount_(amount), target_account_(to_account), target_fund_(to_fund) {
}

//Account History transaction ('A')
Transaction::Transaction(char type, int account_id)
    : type_(type), account_id_(account_id), fund_id_(-1), amount_(0), target_account_(-1), target_fund_(-1) {
    //Account History just requires the account ID
}

//Fund History transaction ('F')
Transaction::Transaction(char type, int account_id, int fund_id)
    : type_(type), account_id_(account_id), fund_id_(fund_id), amount_(0), target_account_(-1), target_fund_(-1) {
    //Fund History requires both account and fund IDs
}

void Transaction::SetSuccess(char a) {
    success_ = a;
}

char Transaction::type() const {
    return type_;
}

int Transaction::account_id() const {
    return account_id_;
}

int Transaction::fund_id() const {
    return fund_id_;
}

int Transaction::target_account() const {
    return target_account_;
}

int Transaction::target_fund() const {
    return target_fund_;
}

int Transaction::amount() const {
    return amount_;
}

const string& Transaction::last_name() const {
    return last_name_;
}

char Transaction::success() const {
    return success_;
}

const string& Transaction::first_name() const {
    return first_name_;
}

ostream &operator<<(ostream &os, const Transaction &rhs) {
    if (rhs.type_ == 'T' && rhs.success_ == 'F') {
        os << rhs.type_ << " " << rhs.account_id_ << " " << rhs.fund_id_ << " " << rhs.target_account_ << " " << rhs.target_fund_ << " " << rhs.amount_ << " (Failed)";
    }
    else if (rhs.type_ == 'T') {
        os << rhs.type_ << " " << rhs.account_id_ << " " << rhs.fund_id_ << " " << rhs.target_account_ << " " << rhs.target_fund_ << " " << rhs.amount_;
    }
    else if (rhs.success_ == 'F') {
        os << rhs.type_ << " " << rhs.account_id_ << " " << rhs.fund_id_ << " " << rhs.amount_ << " (Failed)";
    }
    else {
        os << rhs.type_ << " " << rhs.account_id_ << " " << rhs.fund_id_ << " " << rhs.amount_;
    }
    return os;
}
