#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <set>

template<class T>
class Set {
private:
    struct Node {
        T key;
        Node* left_child;
        Node* right_child;
        Node *parent;
        uint8_t height;
        uint32_t size;

        Node(const T& key) {
            this->key = key;
            left_child = nullptr;
            right_child = nullptr;
            parent = nullptr;
            height = 1;
            size = 1;
        }

        Node(const Node& ot) {
            key = ot.key;
            left_child = ot.left_child;
            right_child = ot.right_child;
            parent = ot.parent;
            height = ot.height;
            size = ot.size;
        }

        ~Node() {
            delete left_child;
            delete right_child;
        };
    };

    /*
     * Gets the height of a tree
     */
    uint8_t GetHeight(const Node* root) const {
        if (root != nullptr) {
            return root->height;
        } else {
            return 0;
        }
    }

    /*
     * Gets the number of elements in the tree
     */
    uint32_t GetSize(const Node* root) const {
        if (root != nullptr) {
            return root->size;
        } else {
            return 0;
        }
    }

    /*
     * Gets the difference of left and right child height
     */
    int8_t GetAVHeight(const Node* root) const {
        return GetHeight(root->right_child) - GetHeight(root->left_child);
    }

    /*
     * An utility function to maintain meta information stored in nodes
     */
    void PullHeight(Node* root) {
        if (root == nullptr) {
            return;
        }
        if (GetHeight(root->left_child) > GetHeight(root->right_child)) {
            root->height = static_cast<uint8_t>(GetHeight(root->left_child) + 1);
        } else {
            root->height = static_cast<uint8_t>(GetHeight(root->right_child) + 1);
        }
        root->size = GetSize(root->left_child) + 1 + GetSize(root->right_child);
        if (root->left_child != nullptr) {
            root->left_child->parent = root;
        }
        if (root->right_child != nullptr) {
            root->right_child->parent = root;
        }
    }

    /*
     * This function removes parent from a child
     */
    void DisconnectParents(Node* root) {
        if (root != nullptr) {
            root->parent = nullptr;
        }
    }

    /*
     * This function makes a left rotate
     */
    Node* LeftRotate(Node* root) {
        Node* q = root;
        Node* p = q->right_child;
        DisconnectParents(q);
        DisconnectParents(p);
        DisconnectParents(p->left_child);
        DisconnectParents(p->right_child);
        DisconnectParents(q->left_child);
        DisconnectParents(q->right_child);
        q->right_child = p->left_child;
        PullHeight(q);
        p->left_child = q;
        PullHeight(p);
        PullHeight(q);
        PullHeight(p);
        return p;
    }

    /*
     * This function makes a right rotate
     */
    Node* RightRotate(Node* root) {
        Node* p = root;
        Node* q = root->left_child;
        DisconnectParents(p);
        DisconnectParents(q);
        DisconnectParents(p->left_child);
        DisconnectParents(p->right_child);
        DisconnectParents(q->left_child);
        DisconnectParents(q->right_child);
        p->left_child = q->right_child;
        q->right_child = p;
        PullHeight(q);
        PullHeight(p);
        PullHeight(q);
        return q;
    }

    /*
     * This function balances a node
     */
    Node* Balance(Node* root) {
        PullHeight(root);
        int8_t val = GetAVHeight(root);
        if (val == -2) {
            if (GetAVHeight(root->left_child) > 0) {
                DisconnectParents(root->left_child);
                root->left_child = LeftRotate(root->left_child);
                PullHeight(root);
            }
            return RightRotate(root);
        } else if (val == 2) {
            if (GetAVHeight(root->right_child) < 0) {
                DisconnectParents(root->right_child);
                root->right_child = RightRotate(root->right_child);
                PullHeight(root);
            }
            return LeftRotate(root);
        }
        return root;
    }

    /*
     * This function inserts given node to a tree
     */
    Node* Insert(Node* root, const T& x) {
        if (root == nullptr) {
            return new Node(x);
        } else if (x < root->key) {
            DisconnectParents(root->left_child);
            root->left_child = Insert(root->left_child, x);
            PullHeight(root);
            return Balance(root);
        } else if (root->key < x) {
            DisconnectParents(root->right_child);
            root->right_child = Insert(root->right_child, x);
            PullHeight(root);
            return Balance(root);
        } else {
            return root;
        }
    }

    /*
     * This function finds node with minimum key in a subtree
     */
    Node* FindMin(Node* root) const {
        if (root == nullptr) {
            return nullptr;
        }
        if (root->left_child == nullptr) {
            return root;
        } else {
            return FindMin(root->left_child);
        }
    }

    /*
     * This function finds node with maximum key in a subtree
     */
    Node* FindMax(Node* root) const {
        if (root == nullptr) {
            return nullptr;
        }
        if (root->right_child == nullptr) {
            return root;
        } else {
            return FindMax(root->right_child);
        }
    }

    /*
     * This function removes node with minimum key in a subtree
     */
    Node* RemoveMin(Node* root) {
        if (root->left_child == nullptr) {
            DisconnectParents(root->right_child);
            return root->right_child;
        } else {
            DisconnectParents(root->left_child);
            root->left_child = RemoveMin(root->left_child);
            PullHeight(root);
            root = Balance(root);
            return root;
        }
    }

    /*
     * This function removes a selected node from a subtree
     */
    Node* Remove(Node* root, const T& x) {
        if (root == nullptr) {
            return root;
        } else if (x < root->key) {
            DisconnectParents(root->left_child);
            root->left_child = Remove(root->left_child, x);
            PullHeight(root);
            root = Balance(root);
            return root;
        } else if (root->key < x) {
            DisconnectParents(root->right_child);
            root->right_child = Remove(root->right_child, x);
            PullHeight(root);
            root = Balance(root);
            return root;
        } else {
            Node* p = root->left_child;
            Node* q = root->right_child;
            DisconnectParents(p);
            DisconnectParents(q);
            root->left_child = nullptr;
            root->right_child = nullptr;
            delete root;
            if (q == nullptr) {
                return p;
            } else {
                Node* a = FindMin(q);
                Node* pa = a->parent;
                q = RemoveMin(q);
                DisconnectParents(a->right_child);
                DisconnectParents(q);
                a->right_child = q;
                DisconnectParents(a->left_child);
                DisconnectParents(p);
                a->left_child = p;
                PullHeight(a);
                PullHeight(p);
                PullHeight(q);
                PullHeight(pa);
                a = Balance(a);
                return a;
            }
        }
    }

    /*
     * This function finds node with selected key in a subtree
     */
    Node* Find(Node* root, const T& x) const {
        if (root == nullptr || (!(root->key < x) && !(x < root->key))) {
            return root;
        } else if (x < root->key) {
            return Find(root->left_child, x);
        } else {
            return Find(root->right_child, x);
        }
    }

    /*
     * This function find a node with minimum key, which in not less than x (like std::lower_bound)
     */
    Node* Lower_bound(Node* root, const T& x) const {
        if (root == nullptr || (!(root->key < x) && !(x < root->key))) {
            return root;
        } else if (root->key < x) {
            return Lower_bound(root->right_child, x);
        } else {
            auto a = Lower_bound(root->left_child, x);
            if (a != nullptr) {
                return a;
            } else {
                return root;
            }
        }
    }

    /*
     * This function find a node with minimum key, which in greater than x (like std::upper_bound)
     */
    Node* Upper_bound(Node* root, const T& x) {
        if (root == nullptr) {
            return root;
        } else if (root->key <= x) {
            return Upper_bound(root->right_child, x);
        } else {
            auto a = Upper_bound(root->left_child, x);
            if (a != nullptr) {
                return a;
            } else {
                return root;
            }
        }
    }

    /*
     * This function makes a deep copy of a tree
     */
    Node* Copy_tree(Node* root, Node* parent, Node* ot) {
        if (ot == nullptr) {
            return nullptr;
        }
        root = new Node(*ot);
        root->parent = parent;
        root->left_child = Copy_tree(root->left_child, root, ot->left_child);
        root->right_child = Copy_tree(root->right_child, root, ot->right_child);
        return root;
    }

    Node* data_ = nullptr;

public:
    Set() {}

    ~Set() {
        delete data_;
    }

    Set(const Set& ot) {
        data_ = Copy_tree(data_, static_cast<Node*>(nullptr), ot.data_);
    }

    Set operator=(const Set& ot) {
        delete data_;
        data_ = Copy_tree(data_, static_cast<Node*>(nullptr), ot.data_);
        return *this;
    }

    template<typename iterator>
    Set(iterator first, iterator last) {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }

    Set(std::initializer_list<T> elems) {
        for (const T& el : elems) {
            insert(el);
        }
    }

    /*
     * Gets count of elements in a tree
     */
    size_t size() const {
        return GetSize(data_);
    }

    /*
     * Checks if the tree is empty
     */
    bool empty() const {
        return GetSize(data_) == 0;
    }

    /*
     * Inserts selected node to a tree
     */
    void insert(const T& elem) {
        data_ = Insert(data_, elem);
    }

    /*
     * Erases selected node from a tree
     */
    void erase(const T& elem) {
        data_ = Remove(data_, elem);
    }

    class iterator {
    private:
        Node* root;
        bool end;
    public:
        iterator(): root(nullptr), end(true) {}

        iterator(Node* root): root(root), end(root == nullptr) {}

        iterator(Node* root, bool end): root(root), end(end) {}

        iterator& operator++() {
            Node* proot = root;
            if (root->right_child != nullptr) {
                root = root->right_child;
                while (root->left_child != nullptr) {
                    root = root->left_child;
                }
                return *this;
            }
            while (root->parent != nullptr && root==root->parent->right_child) {
                root = root->parent;
            }
            if (root->parent == nullptr) {
                root = proot;
                end = true;
            } else {
                root = root->parent;
            }
            return *this;
        }

        iterator& operator--() {
            if (end == true) {
                end = false;
                return *this;
            }
            if (root->left_child != nullptr) {
                root = root->left_child;
                while (root->right_child != nullptr) {
                    root = root->right_child;
                }
                return *this;
            }
            while (root->parent != nullptr && root==root->parent->left_child) {
                root = root->parent;
            }
            if (root->parent != nullptr) {
                root = root->parent;
            }
            return *this;
        }

        iterator operator++(int) {
            Node* proot = root;
            ++(*this);
            return iterator(proot);
        }

        iterator operator--(int) {
            Node* proot = root;
            --(*this);
            return iterator(proot);
        }

        bool operator==(const iterator& ot) const {
            return root == ot.root && end == ot.end;
        }

        bool operator!=(const iterator& ot) const {
            return !(*this == ot);
        }

        const T& operator*() const {
            return root->key;
        }

        const T* operator->() const {
            return &root->key;
        }
    };

    iterator begin() const {
        if (empty()) {
            return end();
        } else {
            return iterator(FindMin(data_));
        }
    }

    iterator end() const {
        return iterator(FindMax(data_), true);
    }

    /*
     * Finds node with needed key and returns iterator to it
     */
    iterator find(const T& elem) const {
        Node* root = Find(data_, elem);
        return (root == nullptr ? end() : iterator(root));
    }

    /*
     * This function find a node with minimum key, which in not less than x (like std::lower_bound)
     * And returns iterator to it
     */
    iterator lower_bound(const T& elem) const {
        Node* root = Lower_bound(data_, elem);
        return (root == nullptr ? end() : iterator(root));
    }
};