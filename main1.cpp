#include <iostream>
#include "UnionFind.h"

int main() {
    int membersArr[] = {0,1,2};
    int newSet, oldSet;
    int set, member = 2;
    UnionFind<int, int*> sets = UnionFind<int, int*>(3, membersArr);
    sets.print();

    set = sets.find(member);
    std::cout << "member " << member << " is in set " << set << std::endl;

    std::cout << "+++++" <<std::endl;
    sets.Union(0,1, &newSet, &oldSet);
    sets.print();
    std::cout << "new set is: " << newSet << " old set is: " << oldSet << std::endl;
    std::cout << "+++++" <<std::endl;
    sets.Union(0,2, &newSet, &oldSet);
    sets.print();
    std::cout << "new set is: " << newSet << " old set is: " << oldSet << std::endl;
    std::cout << "+++++" <<std::endl;

    set = sets.find(member);
    std::cout << "member " << member << " is in set " << set << std::endl;

    return 1;
}
