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
    struct Node {
        int id;
        T data; //assumes T has an empty constructor
        struct Node* next;
        struct Node* previous;
        Node(int id, T value): id(id), data(value), next(nullptr), previous(nullptr) {};
        Node(): id(UNINITIALIZED_ID), next(nullptr), previous(nullptr), data(T()) {};
        ~Node() = default;
    };
    int arraySize=START_SIZE;
    int numOfMembers=0;
    Node* hashArray = new Node[arraySize]; //TODO maybe change to pointers instead od instances which is weird


public:
    HashTable() = default;
    ~HashTable();
    T operator[](int id);
    bool add(int id, T value);
    bool remove(int id);
    void print();

private:
    Node* find(int id);
    void updateSize();
    void changeSize(float factor);
    int hashFunc(int id);
    Node* findInList(Node* root, int id);
    void deleteList(Node* root);
    void removeFromList(Node* root);
    float getStressState();
    void houseList(Node* nodeToAdd, Node* newHashArray);
    void addToTopOfList(Node* newRoot, Node* nodeToAdd);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// C'tor & D'tor ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
HashTable<T>::~HashTable() {
    for (int i = 0; i < numOfMembers; ++i) {
        deleteList(&hashArray[i]);
    }
    delete []hashArray; //redundant?
    //TODO delete T values as well?
}

template <class T>
void HashTable<T>::deleteList(Node* root) {
    if (root->next != nullptr) {
        deleteList(root->next);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// FIND ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
T HashTable<T>:: operator[](int id) {
    Node* root = find(id);
    if (root->id == id) {
        return root->data;
    }
    return T();
}
template <class T>
typename HashTable<T>::Node* HashTable<T>::find(int id) {
    int cell = hashFunc(id);
    Node* listStart = &hashArray[cell];
    return findInList(listStart, id); //responsibility of the caller to make sure that got wanted id back
}

template <class T>
typename HashTable<T>::Node* HashTable<T>::findInList(Node* root, int id) {
    while (root->next != nullptr && id != root->id) {
        root = root->next;
    }
    return root; //responsibility of the caller to make sure that got wanted id back
}

template <class T>
int HashTable<T>::hashFunc(int id) {
    assert(id>=0);
    return (id % arraySize);//modulo func promises even distribution
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// ADD & REMOVE ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//returns false if id already existed
template <class T>
bool HashTable<T>::add(int id, T value) {
    Node* root = find(id);
    if (root->id==id) {
        return false;
    }
    root->next = new Node(id, value);
    root->next->previous = root;
    numOfMembers++;
    updateSize();
    return true;
}

//returns false if id already existed
template <class T>
bool HashTable<T>::remove(int id) {
    Node* root = find(id);
    if (root->id != id) {
        return false;
    }
    removeFromList(root);
    numOfMembers--;
    updateSize();
    return true;
}

template <class T>
void HashTable<T>::removeFromList(Node* root) {
    root->previous->next = root->next;
    if (root->next != nullptr) { //root->previous is always not equal to nullptr
        root->next->previous = root->previous;
    }
    delete root;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// CHANGE SIZE /////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
float HashTable<T>:: getStressState() {
    return float(numOfMembers) / arraySize;
}

template <class T>
void HashTable<T>::updateSize() {
    float stressFactor = getStressState();
    if (stressFactor >= MAX_STRESS) {
        changeSize(INCREASE_FACTOR);
        return;
    }
    else if (stressFactor <= MIN_STRESS) {
        if (arraySize <= START_SIZE) { //START_SIZE is the minimum size of HashTable
            return;
        }
        changeSize(DECREASE_FACTOR);
        return;
    } //if stressFactor == wantedStressFactor do nothing
}

template <class T>
void HashTable<T>::changeSize(float factor) {
    int oldArraySize = arraySize;
    arraySize = int(arraySize * factor); //factor should be 2 or 0.5
    Node* newHashArray = new Node[arraySize];
    for (int i = 0; i < oldArraySize; ++i) {
        houseList(&hashArray[i], newHashArray);
    }
    Node* oldHashArray = hashArray;
    hashArray = newHashArray;
    delete []oldHashArray;
}

template <class T>
void HashTable<T>::houseList(Node* nodeToAdd, Node* newHashArray) {
    Node* newRoot, newSecondNode;
    while (nodeToAdd->next != nullptr) {
        nodeToAdd = nodeToAdd->next;
        newRoot = &newHashArray[hashFunc(nodeToAdd->id)];
        addToTopOfList(newRoot, nodeToAdd);
    }
}

template <class T>
void HashTable<T>::addToTopOfList(Node* newRoot, Node* nodeToAdd) {
    Node* secondInLine = newRoot->next;
    newRoot->next = nodeToAdd;
    nodeToAdd->next = secondInLine;
    nodeToAdd->previous = newRoot;
    if (secondInLine != nullptr) {
        secondInLine->previous = nodeToAdd;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// PRINT ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//TODO remember to delete this shit
template <class T>
void HashTable<T>::print() {
    Node* n;
    std::cout << "numOfMembers: " << numOfMembers << " arraySize: " << arraySize << std::endl;
    for (int i = 0; i < arraySize; ++i) {
        std::cout << i << ": ";
        n = &hashArray[i];
        while (n->next != nullptr) {
            n= n->next;
            std::cout << n->id << "->";
        }
        std::cout << std::endl;
    }
}

#endif //MAIN_CPP_HASHTABLE_H
