#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include "assert.h"
#include "iostream"

#define NO_PARENT -1

template <class T>
class UnionFind {
private:
    struct Set {
        T value; //T is a pointer and should have C'tor that accepts integer. will be his ID, also
        int size;
        int parent;
    };
    int num_of_groups;
    Set* setArr;

public:
    UnionFind(int num_of_groups);
    UnionFind() = default;
    ~UnionFind() = default;
    void Union(int setID1, int setID2); //TODO change name, right now is this way because union is builtin word in c++
    T find(int setID);
    void print(); //TODO remember to delete, written like shit

private:
    int findParent(int setID);
    void shrinkTree(int setID, int parent);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// C'tor & D'tor ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
UnionFind<T>::UnionFind(int num_of_groups):num_of_groups(num_of_groups) {
    setArr = new Set[num_of_groups]();
    for (int i = 0; i < num_of_groups; ++i) {
        setArr[i].size = 1;
        setArr[i].parent = NO_PARENT;
        setArr[i].value = T(i);
    }
}
/* TODO not sure how to write this properly
template <class T, class A>
UnionFind<T, A>::~UnionFind() {
    //~A(setArr);
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// FIND ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
T UnionFind<T>:: find(int setID) {
    assert(setID<num_of_groups && setID>=0); //TODO when testing make sure there is a coherence with setID and array indexes
    int parent = findParent(setID);
    return setArr[parent].value;
}

template <class T>
int UnionFind<T>:: findParent(int setID) {
    int root = setID;
    while (setArr[root].parent != NO_PARENT) {
        root = setArr[root].parent;
    }
    shrinkTree(setID, root);
    return root;
}

template <class T>
void UnionFind<T>::shrinkTree(int setID, int parent) {
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
template <class T>
void UnionFind<T>:: Union(int setID1, int setID2) { //assumes setID's point to  are valid sets for union (with no parents)
    assert(setID1<num_of_groups && setID1>=0);
    assert(setID2<num_of_groups && setID2>=0);
    assert(setArr[setID1].parent == NO_PARENT);
    assert(setArr[setID2].parent == NO_PARENT);
    if (setID1==setID2) { //if same set do nothing
        return;
    }
    int newParent = (setArr[setID2].size > setArr[setID1].size)? setID2 : setID1; //make sure to union by size
    int newChild = (setArr[setID2].size > setArr[setID1].size)? setID1 : setID2;
    setArr[newChild].parent = newParent;
    setArr[newParent].size += setArr[newChild].size;
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
        std::cout << "setID: " << i << " parent: " << set.parent << " size: " << set.size << std::endl;
    }
    std::cout << "----------------------------------" <<std::endl;
}

#endif //WET2_UNIONFIND_H
