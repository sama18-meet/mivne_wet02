#include <iostream>
#include "RankTree.h"

int main() {
    RankTreeOPK* rt = new RankTreeOPK(4);
    rt->insert(1, 2);
    rt->insert(1, 1);
    rt->insert(1, 1);
    rt->insert(2, 3);
    rt->insert(2, 3);
    rt->insert(3, 3);
    rt->insert(3, 3);
    rt->printBT();
    std::cout << "done. next function should print 1. maqam should be 4. bast should be 4." << std::endl;
    std::cout << rt->getPercentOfValueInKeyBounds(2, 3, 3) << std::endl;
    std::cout << "done. next function should print 0.4. mqam should be 5. bast should be 2" << std::endl;
    std::cout << rt->getPercentOfValueInKeyBounds(0, 2, 1) << std::endl;
    rt->remove(3, 3);
    rt->printBT();
    std::cout << "done. next function should print 0.5" << std::endl;
    std::cout << rt->getPercentOfValueInKeyBounds(-1, 7, 3) << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    RankTreeOPK* rt2 = new RankTreeOPK(4);
    rt2->insert(3, 2);
    rt2->insert(3, 3);
    rt2->insert(3, 3);
    rt2->insert(3, 3);
    rt2->insert(4, 3);
    rt2->insert(2, 1);
    rt2->insert(2, 2);
    rt2->printBT();
    RankTreeOPK* rt3 = new RankTreeOPK(rt, rt2);
    std::cout << "Printing merged rankTree" << std::endl;
    rt3->printBT();
    std::cout << "next should print 10/3 = 3.33333" << std::endl;
    std::cout << rt3->getAvgHighest(3) << std::endl;
    std::cout << "next should print 13/4 = 3.25" << std::endl;
    std::cout << rt3->getAvgHighest(4) << std::endl;
    std::cout << "next should print 21/7 = 3" << std::endl;
    std::cout << rt3->getAvgHighest(7) << std::endl;
    int lower;
    int upper;
    std::cout << "nest should be 0-1" << std::endl;
    rt3->getValRange(3, 2, &lower, &upper);
    std::cout << lower << "-" << upper << std::endl;
    std::cout << "nest should be 1-2" << std::endl;
    rt3->getValRange(3, 3, &lower, &upper);
    std::cout << lower << "-" << upper << std::endl;
    std::cout << "nest should be 0-1" << std::endl;
    rt3->getValRange(2, 3, &lower, &upper);
    std::cout << lower << "-" << upper << std::endl;

    return 0;
}
