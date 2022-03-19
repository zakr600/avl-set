#pragma once
#include <set>
#include <cstddef>
#include <cassert>

template<class T>
struct node {
    T key;
    node *l, *r;
    node *p;
    int height;
    int size;
    node(const T& key) {
        this->key = key;
        l = nullptr;
        r = nullptr;
        p = nullptr;
        height = 1;
        size = 1;
    }

    node(const node<T>& ot) {
        key = ot.key;
        l = ot.l;
        r = ot.r;
        p = ot.p;
        height = ot.height;
        size = ot.size;
    }

    ~node() {
        delete l;
        delete r;
    };
};

template<class T>
int getHeight(node<T> *root) {
    if (root != nullptr) {
        return root->height;
    } else {
        return 0;
    }
}

template<class T>
int getSize(node<T> *root) {
    if (root != nullptr) {
        return root->size;
    } else {
        return 0;
    }
}

template<class T>
int getAVHeight(node<T> *root) {
    return getHeight(root->r) - getHeight(root->l);
}

template<class T>
void pull(node<T> *root) {
    if (root == nullptr) {
        return;
    }
    if (getHeight(root->l) > getHeight(root->r)) {
        root->height = getHeight(root->l) + 1;
    } else {
        root->height = getHeight(root->r) + 1;
    }
    root->size = getSize(root->l) + 1 + getSize(root->r);
    if (root->l != nullptr) {
        root->l->p = root;
    }
    if (root->r != nullptr) {
        root->r->p = root;
    }
}

template<class T>
void disc(node<T> *root) {
    if (root != nullptr) {
        root->p = nullptr;
    }
}

template<class T>
node<T>* leftRotate(node<T> *root) {
    node<T> *q = root;
    node<T> *p = q->r;
    disc(q);
    disc(p);
    disc(p->l);
    disc(p->r);
    disc(q->l);
    disc(q->r);
    q->r = p->l;
    pull(q);
    p->l = q;
    pull(p);
    pull(q);
    pull(p);
    return p;
}

template<class T>
node<T>* rightRotate(node<T> *root) {
    node<T> *p = root;
    node<T> *q = root->l;
    disc(p);
    disc(q);
    disc(p->l);
    disc(p->r);
    disc(q->l);
    disc(q->r);
    p->l = q->r;
    q->r = p;
    pull(q);
    pull(p);
    pull(q);
    return q;
}

template<class T>
node<T>* balance(node<T> *root) {
    pull(root);
    int val = getAVHeight(root);
    if (val == -2) {
        if (getAVHeight(root->l) > 0) {
            disc(root->l);
            root->l = leftRotate(root->l);
            pull(root);
        }
        return rightRotate(root);
    } else if (val == 2) {
        if (getAVHeight(root->r) < 0) {
            disc(root->r);
            root->r = rightRotate(root->r);
            pull(root);
        }
        return leftRotate(root);
    }
    return root;
}

template<class T>
node<T>* Insert(node<T> *root, const T& x) {
    if (root == nullptr) {
        return new node<T>(x);
    } else if (x < root->key) {
        disc(root->l);
        root->l = Insert(root->l, x);
        pull(root);
        return balance(root);
    } else if (root->key < x) {
        disc(root->r);
        root->r = Insert(root->r, x);
        pull(root);
        return balance(root);
    } else {
        return root;
    }
}

template<class T>
node<T>* findMin(node<T> *root) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->l == nullptr) {
        return root;
    } else {
        return findMin(root->l);
    }
}

template<class T>
node<T>* findMax(node<T> *root) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->r == nullptr) {
        return root;
    } else {
        return findMax(root->r);
    }
}

template<class T>
node<T>* removeMin(node<T> *root) {
    if (root->l == nullptr) {
        disc(root->r);
        return root->r;
    } else {
        disc(root->l);
        root->l = removeMin(root->l);
        pull(root);
        root = balance(root);
        return root;
    }
}

template<class T>
node<T>* remove(node<T> *root, const T& x) {
    if (root == nullptr) {
        return root;
    } else if (x < root->key) {
        disc(root->l);
        root->l = remove(root->l, x);
        pull(root);
        root = balance(root);
        return root;
    } else if (root->key < x) {
        disc(root->r);
        root->r = remove(root->r, x);
        pull(root);
        root = balance(root);
        return root;
    } else {
        node<T> *p = root->l;
        node<T> *q = root->r;
        disc(p);
        disc(q);
        root->l = nullptr;
        root->r = nullptr;
        delete root;
        if (q == nullptr) {
            return p;
        } else {
            node<T> *a = findMin(q);
            node<T>* pa = a->p;
            q = removeMin(q);
            disc(a->r);
            disc(q);
            a->r = q;
            disc(a->l);
            disc(p);
            a->l = p;
            pull(a);
            pull(p);
            pull(q);
            pull(pa);
            a = balance(a);
            return a;
        }
    }
}

template<class T>
node<T>* Find(node<T> *root, const T& x) {
    if (root == nullptr || (!(root->key < x) && !(x < root->key))) {
        return root;
    } else if (x < root->key) {
        return Find(root->l, x);
    } else {
        return Find(root->r, x);
    }
}

template<class T>
node<T>* Lower_bound(node<T>* root, const T& x) {
    if (root == nullptr || (!(root->key < x) && !(x < root->key))) {
        return root;
    } else if (root->key < x) {
        return Lower_bound(root->r, x);
    } else {
        auto a = Lower_bound(root->l, x);
        if (a != nullptr) {
            return a;
        } else {
            return root;
        }
    }
}

template<class T>
node<T>* upper_bound(node<T>* root, const T& x) {
    if (root == nullptr) {
        return root;
    } else if (root->key <= x) {
        return upper_bound(root->r, x);
    } else {
        auto a = upper_bound(root->l, x);
        if (a != nullptr) {
            return a;
        } else {
            return root;
        }
    }
}

template <class T>
node<T>* copy_tree(node<T>* root, node<T>* parent, node<T>* ot) {
    if (ot == nullptr) {
        return nullptr;
    }
    root = new node<T>(*ot);
    root->p = parent;
    root->l = copy_tree(root->l, root, ot->l);
    root->r = copy_tree(root->r, root, ot->r);
    return root;
}
template<class T>
class Set {
public:
    Set() {}

    ~Set() {
        delete data_;
    }

    Set(const Set& ot) {
        data_ = copy_tree(data_, static_cast<node<T>*>(nullptr), ot.data_);
    }

    Set operator=(const Set& ot) {
        delete data_;
        data_ = copy_tree(data_, static_cast<node<T>*>(nullptr), ot.data_);
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

    size_t size() const {
        return getSize(data_);
    }

    bool empty() const {
        return getSize(data_) == 0;
    }

    void insert(const T& elem) {
        data_ = Insert(data_, elem);
    }

    void erase(const T& elem) {
        data_ = remove(data_, elem);
    }

    class iterator {
    private:
        node<T>* root;
        bool end;
    public:
        iterator(): root(nullptr), end(true) {}

        iterator(node<T>* root): root(root), end(root == nullptr) {}

        iterator(node<T>* root, bool end): root(root), end(end) {}

        iterator& operator++() {
            node<T> *proot = root;
            if (root->r != nullptr) {
                root = root->r;
                while (root->l != nullptr) {
                    root = root->l;
                }
                return *this;
            }
            while (root->p != nullptr && root==root->p->r) {
                root = root->p;
            }
            if (root->p == nullptr) {
                root = proot;
                end = true;
            } else {
                root = root->p;
            }
            return *this;
        }

        iterator& operator--() {
            if (end == true) {
                end = false;
                return *this;
            }
            if (root->l != nullptr) {
                root = root->l;
                while (root->r != nullptr) {
                    root = root->r;
                }
                return *this;
            }
            while (root->p != nullptr && root==root->p->l) {
                root = root->p;
            }
            if (root->p == nullptr) {
                //234324
            } else {
                root = root->p;
            }
            return *this;
        }

        iterator operator++(int) {
            node<T>* proot = root;
            ++(*this);
            return iterator(proot);
        }

        iterator operator--(int) {
            node<T>* proot = root;
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
            return iterator(findMin(data_));
        }
    }

    iterator end() const {
        return iterator(findMax(data_), true);
    }

    iterator find(const T& elem) const {
        node<T>* root = Find(data_, elem);
        return (root == nullptr ? end() : iterator(root));
    }

    iterator lower_bound(const T& elem) const {
        node<T>* root = Lower_bound(data_, elem);
        return (root == nullptr ? end() : iterator(root));
    }
private:
    node<T>* data_ = nullptr;
};