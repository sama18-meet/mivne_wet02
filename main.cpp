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
    std::cout << "printing ranks 6-11" << std::endl;
    std::cout << "Hello, World!" << std::endl;

    return 0;
}
