#ifndef MAIN_CPP_HASHTABLE_H
#define MAIN_CPP_HASHTABLE_H

#include "assert.h"
#include "iostream"

#define START_SIZE 10
#define UNINITIALIZED_ID -1
#define MAX_STRESS 1
#define MIN_STRESS 0.25
#define INCREASE_FACTOR 2
#define DECREASE_FACTOR 0.5


template <class T>
class HashTable {
private:
    // node in linked list (chain hashing)
    struct Node {
        int id;
        T data; //assumes T has an empty constructor
        struct Node* next;
        struct Node* previous;
        Node(int id, T value): id(id), data(value), next(nullptr), previous(nullptr) {};
        Node() : id(UNINITIALIZED_ID), next(nullptr), previous(nullptr), data(T()) {};
        ~Node() = default;
    };
    int arraySize;
    int numOfMembers;
    Node** hashArray;


public:
    HashTable();
    ~HashTable();
    T operator[](int id);
    bool insert(int id, T value);
    bool remove(int id);
    void print();

private:
    Node* find(int id);
    void addToTopOfList(Node* newRoot, int cell);
    void updateSize();
    void changeSize(double factor);
    int hashFunc(int id);
    Node* findInList(Node* root, int id);
    void deleteList(Node* root);
    void removeFromList(Node* nodeToRemove, int cell);
    double getStressState();
    void houseList(Node* nodeToAdd, Node** newHashArray);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// C'tor & D'tor ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checked v
template <class T>
HashTable<T>::HashTable() : arraySize(START_SIZE), numOfMembers(0) {
    hashArray = new Node*[arraySize];
    for (int i=0; i<arraySize; ++i) {
        hashArray[i] = nullptr;
    }
}

template <class T>
HashTable<T>::~HashTable() {
    for (int i=0; i < arraySize; ++i) {
        deleteList(hashArray[i]);
    }
    delete[] hashArray;
}

template <class T>
void HashTable<T>::deleteList(Node* root) {
    if (root == nullptr) {
        return;
    }
    deleteList(root->next);
    delete root->data;
    delete root;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// FIND ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checked v
template <class T>
T HashTable<T>:: operator[](int id) {
    Node* node = find(id);
    if (node == nullptr) {
        assert(0==1); // T() in the following line needs to be changed to nullptr
        return T();
    }
    return node->data;
}

template <class T>
typename HashTable<T>::Node* HashTable<T>::find(int id) {
    int cell = hashFunc(id);
    return findInList(hashArray[cell], id);
}

template <class T>
typename HashTable<T>::Node* HashTable<T>::findInList(Node* root, int id) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->id == id) {
        return root;
    }
    return findInList(root->next, id);
}

template <class T>
int HashTable<T>::hashFunc(int id) {
    assert(id>=0);
    return (id % arraySize); //modulo func promises even distribution
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Insert & REMOVE ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//returns false if id already existed
template <class T>
bool HashTable<T>::insert(int id, T value) {
    Node* root = find(id);
    if (root != nullptr) {
        return false;
    }
    Node* new_node = new Node(id, value);
    int cell = hashFunc(id);
    addToTopOfList(new_node, cell);
    numOfMembers++;
    updateSize();
    return true;
}

template <class T>
void HashTable<T>::addToTopOfList(Node* newRoot, int cell) {
    Node* oldRoot = hashArray[cell];
    hashArray[cell] = newRoot;
    newRoot->next = oldRoot;
    newRoot->previous = nullptr;
    if (oldRoot != nullptr) {
        oldRoot->previous = newRoot;
    }
}

//returns false if id doesn't existed
template <class T>
bool HashTable<T>::remove(int id) {
    Node* root = find(id);
    if (root == nullptr) {
        return false;
    }
    int cell = hashFunc(id);
    removeFromList(root, cell);
    numOfMembers--;
    updateSize();
    return true;
}

template <class T>
void HashTable<T>::removeFromList(Node* nodeToRemove, int cell) {
    if (nodeToRemove->previous != nullptr) {
        if (nodeToRemove->next != nullptr) {
            nodeToRemove->previous->next = nodeToRemove->next;
            nodeToRemove->next->previous = nodeToRemove->previous;
        }
        else { // if (nodeToRemove->next == nullptr)
            nodeToRemove->previous->next = nullptr;
        }
    }
    else {
        hashArray[cell] = nodeToRemove->next;
        if (hashArray[cell] != nullptr) {
            hashArray[cell]->previous = nullptr;
        }
    }
    delete nodeToRemove;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// CHANGE SIZE /////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checked v
template <class T>
double HashTable<T>:: getStressState() {
    return (double)numOfMembers / (double)arraySize;
}

template <class T>
void HashTable<T>::updateSize() {
    double stressFactor = getStressState();
    if (stressFactor > MAX_STRESS) {
        changeSize(INCREASE_FACTOR);
        return;
    }
    else if (stressFactor < MIN_STRESS) {
        if (arraySize <= START_SIZE) { //START_SIZE is the minimum size of HashTable
            return;
        }
        changeSize(DECREASE_FACTOR);
        return;
    } //if stressFactor == wantedStressFactor do nothing
}

template <class T>
void HashTable<T>::changeSize(double factor) {
    int oldArraySize = arraySize;
    int newArraySize = int(oldArraySize * factor); //factor should be 2 or 0.5
    assert(newArraySize == oldArraySize*factor); // else complexity is not correct
    arraySize = newArraySize;
    Node** oldHashArray = hashArray;
    Node** newHashArray = new Node*[arraySize];
    hashArray = newHashArray;
    for (int i=0; i<newArraySize; ++i) {
        newHashArray[i] = nullptr;
    }
    for (int i=0; i<oldArraySize; ++i) {
        houseList(oldHashArray[i], newHashArray);
    }
    delete[] oldHashArray;
}

template <class T>
void HashTable<T>::houseList(Node* head, Node** newHashArray) {
    if (head == nullptr) {
        return;
    }
    houseList(head->next, newHashArray);
    int cell = hashFunc(head->id);
    head->next = nullptr;
    head->previous = nullptr;
    addToTopOfList(head, cell);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// PRINT ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//TODO remember to delete this shit
template <class T>
void HashTable<T>::print() {
    Node* n;
    std::cout << "--------------------------------------------------------------" <<std::endl;
    std::cout << "numOfMembers: " << numOfMembers << " arraySize: " << arraySize << std::endl;
    for (int i = 0; i < arraySize; ++i) {
        std::cout << i << ": ";
        n = hashArray[i];
         do {
             if (n==nullptr) {
                 continue;
             }
             std::cout << "(" << n->id << "," << n->data->getData() << ") " << "->";
             n = n->next;
         } while (n != nullptr);
        std::cout << std::endl;
    }
}

#endif //MAIN_CPP_HASHTABLE_H