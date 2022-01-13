#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include "assert.h"
#include "iostream"

#define NO_PARENT -1

template <class T>
class UnionFind {
private:
    struct Set {
        T* value; // T is a pointer and should have C'tor that accepts integer which will be its ID
        int size;
        int parent;
        Set() = default;
        Set(int i) : size(1), parent(NO_PARENT) { value = new T(i); };
    };
    int num_of_groups;
    Set* setArr;

public:
    UnionFind(int num_of_groups);
    UnionFind() = delete;
    ~UnionFind();
    int unite(int setId1, int setId2);
    T* find(int setId);
    void print(); //TODO remember to delete, written like shit

private:
    int findParent(int setId);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// C'tor & D'tor ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
UnionFind<T>::UnionFind(int num_of_groups) : num_of_groups(num_of_groups+1) {
    setArr = new Set[num_of_groups + 1];
    for (int i = 0; i < num_of_groups + 1; ++i) {
        setArr[i] = Set(i);
    }
}

template <class T>
UnionFind<T>::~UnionFind() {
    for (int i = 0; i < num_of_groups; ++i) {
        delete setArr[i].value;
    }
    delete setArr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// FIND ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
T* UnionFind<T>:: find(int setId) {
    assert(setId<num_of_groups && setId>=0); //TODO when testing make sure there is a coherence with setId and array indexes
    int parent = findParent(setId);
    return setArr[parent].value;
}

template <class T>
int UnionFind<T>:: findParent(int setId) {
    if (setArr[setId].parent == NO_PARENT) {
        return setId;
    }
    int parent = findParent(setArr[setId].parent);
    setArr[setId].parent = parent;
    return parent;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// UNION ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//parentValue and childValue are used so can have access to update values outside of class
template <class T>
int UnionFind<T>:: unite(int setId1, int setId2) { //assumes setId's are pointing to valid sets for union (with no parents)
    assert(setId1<num_of_groups && setId1>=0);
    assert(setId2<num_of_groups && setId2>=0);
    if (setId1==setId2) { //if same set do nothing
        return setId1;
    }
    int newParent = (setArr[setId2].size > setArr[setId1].size) ? setId2 : setId1; //make sure to union by size
    int newChild = (setArr[setId2].size > setArr[setId1].size) ? setId1 : setId2;
    setArr[newChild].parent = newParent;
    setArr[newParent].size += setArr[newChild].size;
    return newParent;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// PRINT ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//TODO is written like shit. delete before submitting

template <class T>
void UnionFind<T>::print() {
    std::cout << "----------------------------------" <<std::endl;
    Set set;
    for (int i = 0; i < num_of_groups; ++i) {
        set = setArr[i];
        std::cout << "setId: " << i << " parent: " << set.parent << " size: " << set.size << std::endl;
    }
    std::cout << "----------------------------------" <<std::endl;
}

#endif //WET2_UNIONFIND_H
