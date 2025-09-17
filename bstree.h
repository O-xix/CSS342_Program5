#ifndef BSTREE_H
#define BSTREE_H

#include "account.h"

class BSTree {
public:
    struct Node {
        Account* account_;
        Node* left_;
        Node* right_;
    };
    
    BSTree();
    ~BSTree();

    bool Insert(Account* account);
    bool Retrieve(int account_id, Account*& account) const;
    bool Delete(int account_id, Account*& account);
    void Display() const;

    void InOrderTraversal(Node* node) const;

private:
    Node* root_;

    void Clear(Node* node);
    void Display(Node* node) const;
};

#endif
