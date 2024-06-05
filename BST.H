#ifndef BST_MAP_H
#define BST_MAP_H

#include <iostream>
#include <functional>
#include <utility>

template <typename Key, typename Value>
class BSTMap {
private:
    struct Node {
        std::pair<Key, Value> data;
        Node* left;
        Node* right;
        Node(Key key, Value value) : data(std::make_pair(key, value)), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void insert(Node*& node, const Key& key, const Value& value);
    Node* deleteNode(Node* node, const Key& key);
    Node* findMin(Node* node) const;
    void inOrder(Node* node, void (*visit)(std::pair<Key, Value>&)) const;
    void deleteTree(Node* node);
    void copyTree(Node* src, Node*& dest);
    Node* search(Node* node, const Key& key) const;

public:
    BSTMap();
    BSTMap(const BSTMap& other);
    BSTMap& operator=(const BSTMap& other);
    ~BSTMap();

    void insert(const Key& key, const Value& value);
    void erase(const Key& key);
    Value& operator[](const Key& key);
    Value* find(const Key& key);
    bool contains(const Key& key) const;
    void inOrder(void (*visit)(std::pair<Key, Value>&)) const;
    bool isEmpty() const;

    // Iterator class definitions go here
    // Implement begin(), end(), rbegin(), rend() if needed
};

template <typename Key, typename Value>
BSTMap<Key, Value>::BSTMap() : root(nullptr) {}

template <typename Key, typename Value>
BSTMap<Key, Value>::BSTMap(const BSTMap& other) : root(nullptr) {
    copyTree(other.root, root);
}

template <typename Key, typename Value>
BSTMap<Key, Value>& BSTMap<Key, Value>::operator=(const BSTMap& other) {
    if (this != &other) {
        deleteTree(root);
        root = nullptr;
        copyTree(other.root, root);
    }
    return *this;
}

template <typename Key, typename Value>
BSTMap<Key, Value>::~BSTMap() {
    deleteTree(root);
}

template <typename Key, typename Value>
void BSTMap<Key, Value>::insert(const Key& key, const Value& value) {
    insert(root, key, value);
}

template <typename Key, typename Value>
void BSTMap<Key, Value>::insert(Node*& node, const Key& key, const Value& value) {
    if (node == nullptr) {
        node = new Node(key, value);
    } else if (key < node->data.first) {
        insert(node->left, key, value);
    } else if (key > node->data.first) {
        insert(node->right, key, value);
    } else {
        node->data.second = value;
    }
}

template <typename Key, typename Value>
typename BSTMap<Key, Value>::Node* BSTMap<Key, Value>::deleteNode(Node* node, const Key& key) {
    if (node == nullptr) {
        return node;
    }

    if (key < node->data.first) {
        node->left = deleteNode(node->left, key);
    } else if (key > node->data.first) {
        node->right = deleteNode(node->right, key);
    } else {
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        Node* temp = findMin(node->right);
        node->data = temp->data;
        node->right = deleteNode(node->right, temp->data.first);
    }

    return node;
}

template <typename Key, typename Value>
typename BSTMap<Key, Value>::Node* BSTMap<Key, Value>::findMin(Node* node) const {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

template <typename Key, typename Value>
Value& BSTMap<Key, Value>::operator[](const Key& key) {
    Node* node = search(root, key);
    if (node != nullptr) {
        return node->data.second;
    }
    insert(key, Value());
    return search(root, key)->data.second;
}

template <typename Key, typename Value>
typename BSTMap<Key, Value>::Node* BSTMap<Key, Value>::search(Node* node, const Key& key) const {
    if (node == nullptr || node->data.first == key) {
        return node;
    } else if (key < node->data.first) {
        return search(node->left, key);
    } else {
        return search(node->right, key);
    }
}

template <typename Key, typename Value>
Value* BSTMap<Key, Value>::find(const Key& key) {
    Node* node = search(root, key);
    return node ? &node->data.second : nullptr;
}

template <typename Key, typename Value>
bool BSTMap<Key, Value>::contains(const Key& key) const {
    return search(root, key) != nullptr;
}

template <typename Key, typename Value>
void BSTMap<Key, Value>::erase(const Key& key) {
    root = deleteNode(root, key);
}

template <typename Key, typename Value>
void BSTMap<Key, Value>::inOrder(void (*visit)(std::pair<Key, Value>&)) const {
    inOrder(root, visit);
}

template <typename Key, typename Value>
void BSTMap<Key, Value>::inOrder(Node* node, void (*visit)(std::pair<Key, Value>&)) const {
    if (node != nullptr) {
        inOrder(node->left, visit);
        visit(node->data);
        inOrder(node->right, visit);
    }
}

template <typename Key, typename Value>
void BSTMap<Key, Value>::deleteTree(Node* node) {
    if (node != nullptr) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

template <typename Key, typename Value>
void BSTMap<Key, Value>::copyTree(Node* src, Node*& dest) {
    if (src == nullptr) {
        dest = nullptr;
    } else {
        dest = new Node(src->data.first, src->data.second);
        copyTree(src->left, dest->left);
        copyTree(src->right, dest->right);
    }
}

template <typename Key, typename Value>
bool BSTMap<Key, Value>::isEmpty() const {
    return root == nullptr;
}

#endif // BST_MAP_H
