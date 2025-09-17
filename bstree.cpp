#include "bstree.h"
#include <stack>

BSTree::BSTree() {
    root_ = nullptr;
}

BSTree::~BSTree() {
    //Start with the root node
    Node* current = root_;
    //Trying stack for iterative traversal
    stack<Node*> nodes_to_delete;  

    while (current != nullptr || !nodes_to_delete.empty()) {
        //Go as left as possible 
        while (current != nullptr) {
            nodes_to_delete.push(current);
            current = current->left_;
        }

        //Pop from stack and process the node
        current = nodes_to_delete.top();
        nodes_to_delete.pop();

        //Delete the account and node
        delete current->account_;
        delete current;

        //Move to the right subtree
        current = current->right_;
    }

    //Set root to nullptr to mark the tree as empty
    root_ = nullptr;
}

bool BSTree::Insert(Account *account) {
    Node* node_to_insert = new Node();
    node_to_insert->account_ = account;
    node_to_insert->left_ = nullptr;
    node_to_insert->right_ = nullptr;

    //Root is empty
    if (root_ == nullptr) {
        root_ = node_to_insert;
        return true;
    }

    //Insert as leaf
    Node* current = root_;
    Node* parent = nullptr;

    while (current != nullptr) {
        //Track parent node
        parent = current; 
        //No duplicates of account ids
        if (account->id() == current->account_->id()) {
            //Avoid memory leak
            delete node_to_insert; 
            return false;
        }
        //Less than
        if (account->id() < current->account_->id()) {
            current = current->left_;
        } 
        //Greater than
        else {
            current = current->right_;
        }
    }

    //Attach the new node to the parent
    if (account->id() < parent->account_->id()) {
        parent->left_ = node_to_insert;
    } 
    else {
        parent->right_ = node_to_insert;
    }

    return true;
}

bool BSTree::Retrieve(int account_id, Account *&account) const {
    Node* current = root_;
    while (current != nullptr) {
        //Compare to 'account_id'
        if (account_id == current->account_->id()) { 
            account = current->account_;
            //Found
            return true; 
        }
        if (account_id < current->account_->id()) {
            current = current->left_;
        } else {
            current = current->right_;
        }
    }
    //Not found
    return false; 
}


void BSTree::Display() const {
    InOrderTraversal(root_);
}

void BSTree::InOrderTraversal(Node* node) const {
    if (node == nullptr) return;        //Base case: empty subtree
    InOrderTraversal(node->left_);      //Visit left subtree
    cout << *(node->account_) << "\n";      //Visit current node (print account details)
    InOrderTraversal(node->right_);        //Visit right subtree
}

bool BSTree::Delete(int account_id, Account*& account) {
    Node* parent = nullptr;
    Node* current = root_;

    while (current != nullptr && current->account_->id() != account_id) {
        parent = current;
        if (account_id < current->account_->id()) {
            current = current->left_;
        } else {
            current = current->right_;
        }
    }

    if (current == nullptr) {
        return false;
    }

    account = current->account_;

    //Node is a leaf
    if (current->left_ == nullptr && current->right_ == nullptr) {
        //Root
        if (current == root_) {
            root_ = nullptr; 
        } else if (parent->left_ == current) {
            parent->left_ = nullptr;
        } else {
            parent->right_ = nullptr;
        }
        delete current;
    }

    //Node has only one child
    else if (current->left_ == nullptr || current->right_ == nullptr) {
        Node* child = (current->left_ != nullptr) ? current->left_ : current->right_;
        //Root
        if (current == root_) {
            root_ = child; 
        } else if (parent->left_ == current) {
            parent->left_ = child;
        } else {
            parent->right_ = child;
        }
        delete current;
    }

    else {
        //In-order successor
        Node* successor_parent = current;
        Node* successor = current->right_;
        while (successor->left_ != nullptr) {
            successor_parent = successor;
            successor = successor->left_;
        }

        //Replace
        current->account_ = successor->account_;

        //Delete successor node
        if (successor_parent->left_ == successor) {
            successor_parent->left_ = successor->right_; //Successor might have a right child
        } else {
            successor_parent->right_ = successor->right_;
        }
        delete successor;
    }

    return true;
}

