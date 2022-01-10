#include <iostream>
#include "UnionFind.h"
#include "HashTable.h"
//#include "Player.h"
//#include "RankTree.h"
//#include "Group.h"

class testObj { //has getSet method
private:
    int set;
public:
    int getData() {
        return set;
    }
    int getGroup() {
        return set;
    }
    testObj(int a): set(a) {};
    testObj() = default;
};

int main() {
    HashTable<testObj> ht = HashTable<testObj>();
    UnionFind<testObj> sets = UnionFind<testObj>(8);
    //put stuff in hash table
    for (int i = 0; i < 15; ++i) {
        ht.insert(i,testObj(i));
    }
    ht.print();
    //play with union find
    sets.print();
    testObj t1, t2;
    sets.Union(0,1);
    sets.Union(7,6);
    sets.Union(7,5);
    sets.print();
    t1 = sets.find(1);
    t2 = sets.find(6);
    sets.Union(t1.getGroup(),t2.getGroup());
    sets.print();
    sets.find(1);
    sets.print();
    sets.Union(2,3);
    sets.Union(4,2);
    sets.print();
    t1 = sets.find(3);
    sets.Union(7,t1.getGroup());
    sets.print();
    sets.Union(5,1);
    return 1;
}
