#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include "assert.h"
#include "iostream"

#define NO_PARENT -1

template <class T, class A>
class UnionFind {
private:
    struct Set {
        int setID; //TODO might be redundant
        T value; //T is a pointer and should have C'tor that accepts integer. will be his ID
        int size;
        int parent;
    };
    const int num_of_groups;
    A membersArray; //is general so could for instance be a hashtable. has to support subset (meaning [])
    Set* setArr;

public:
    UnionFind(int num_of_groups, A membersArray);
    ~UnionFind();
    void Union(int setID1, int setID2, T* parentValue, T* childValue); //TODO change name, right now is this way because union is builtin word in c++
    T find(int member);
    void print(); //TODO remember to delete, written like shit

private:
    int findParent(int setID);
    void shrinkTree(int setID, int parent);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// C'tor & D'tor ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T, class A>
UnionFind<T, A>::UnionFind(int num_of_groups, A membersArray):num_of_groups(num_of_groups), membersArray(membersArray) {
    setArr = new Set[num_of_groups]();
    for (int i = 0; i < num_of_groups; ++i) {
        setArr[i].size = 1;
        setArr[i].parent = NO_PARENT;
        setArr[i].setID = i;
        setArr[i].value = T(i); //TODO put in header of class that T should have a constructor that accepts an integer
    }
}

template <class T, class A>
UnionFind<T, A>::~UnionFind() {
    delete []setArr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// FIND ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T, class A>
T UnionFind<T, A>:: find(int member) {
    assert(member>=0);
    int setID = membersArray[member]; //substracting one to comply with array indexes
    assert(setID<num_of_groups && setID>=0); //TODO when testing make sure there is a coherence with setID and array indexes
    int parent = findParent(setID);
    shrinkTree(setID, parent); //TODO need to put this in findParent (line above this)
    return setArr[parent].value;
}

template <class T, class A>
int UnionFind<T, A>:: findParent(int setID) {
    int root = setID;
    while (setArr[root].parent != NO_PARENT) {
        root = setArr[root].parent;
    }
    return root;
}

template <class T, class A>
void UnionFind<T, A>::shrinkTree(int setID, int parent) {
    int curr_root = setID, next_root;
    while (curr_root != parent) {
        next_root = setArr[curr_root].parent;
        setArr[curr_root].parent = parent;
        curr_root = next_root;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// UNION ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//parentValue and childValue are used so can have access to update values outside of class
template <class T, class A>
void UnionFind<T, A>:: Union(int setID1, int setID2, T* parentValue, T* childValue) {
    int parent1 = findParent(setID1), parent2 = findParent(setID2); //TODO make sure this is on line with UnionFind complexity because not receiving pointer to top level set

    if (parent1==parent2) { //if already part of the same set do nothing
        return;
    }

    int newParent = (setArr[parent1].size > setArr[parent2].size)? parent1 : parent2; //make sure to union by size
    int newChild = (setArr[parent1].size > setArr[parent2].size)? parent2 : parent1;
    setArr[newChild].parent = newParent;
    setArr[newParent].size += setArr[newChild].size;
    *parentValue = setArr[newParent].value;
    *childValue = setArr[newChild].value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// PRINT ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//TODO is written like shit. delete before submitting

template <class T, class A>
void UnionFind<T, A>::print() {
    Set set;
    for (int i = 0; i < num_of_groups; ++i) {
        set = setArr[i];
        std::cout << "setID: " << i << " parent: " << set.parent << " size: " << set.size << std::endl;
    }
}

#endif //WET2_UNIONFIND_H
