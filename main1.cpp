#include <iostream>
#include "UnionFind.h"
#include "HashTable.h"

class testObj { //has getSet method
private:
    int set;
public:
    int getSet() {
        return set;
    }
    testObj(int a): set(a) {};
    testObj() = default;
};

int main() {
    HashTable<testObj> ht = HashTable<testObj>();
    UnionFind<testObj, HashTable<testObj>> sets = UnionFind<testObj, HashTable<testObj>>(5, ht);
    //put stuff in hash table
    for (int i = 0; i < 15; ++i) {
        ht.add(i,testObj(i));
    }
    ht.print();
    //play with union find
    sets.print();
    int memberID = 2;
    testObj set, newSet, oldSet;
    set = sets.find(memberID);
    std::cout << "member " << memberID << " is in set " << set.getSet() << std::endl;

    std::cout << "+++++" <<std::endl;
    sets.Union(0,1, &newSet, &oldSet);
    sets.print();
    std::cout << "new set is: " << newSet.getSet() << " old set is: " << oldSet.getSet() << std::endl;
    std::cout << "+++++" <<std::endl;
    sets.Union(0,2, &newSet, &oldSet);
    sets.print();
    std::cout << "new set is: " << newSet.getSet() << " old set is: " << oldSet.getSet() << std::endl;
    std::cout << "+++++" <<std::endl;

    set = sets.find(memberID);
    std::cout << "member " << memberID << " is in set " << set.getSet() << std::endl;
     return 1;
}
