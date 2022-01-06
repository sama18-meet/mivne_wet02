#ifndef _RankTreeOPK_H
#define _RankTreeOPK_H

#include <iostream>
#include <cmath>
#include <assert.h>
#include "OrderedPair.h"

#define ALL_NODES -1
#define SCALE_MAX 201

class RankTreeOPK {
private:
    int size;
    int scale;
    struct Node {
        int key;
        int mul;
        int mulInSubtree;
        int keysSumInSubtree;
        int vals[SCALE_MAX];
        int valsMul[SCALE_MAX];
        int height;
        struct Node* left;
        struct Node* right;
        Node(int key, int val):
                key(key), mul(1), mulInSubtree(1), keysSumInSubtree(key), left(nullptr), right(nullptr), height(0) {
            for (int i=0; i<SCALE_MAX; ++i) {
                vals[i] = 0;
                valsMul[i] = 0;
            }
            vals[val] = 1;
            valsMul[val] = 1;
        };
        ~Node() = default;
        Node& operator=(const Node& n) {
            key = n.key;
            mul = n.mul;
            mulInSubtree = n.mulInSubtree;
            keysSumInSubtree = n.keysSumInSubtree;
            left = n.left;
            right = n.right;
            for (int i=0; i<SCALE_MAX; ++i) {
                vals[i] = (n.vals)[i];
                valsMul[i] = (n.valsMul)[i];
            }
        };
    };
    Node* root;


private:

    // DESTRUCTOR
    void deleteSubtree(Node*);

    // NODE STRUCTURAL PROPERTIES
    int getHeight(Node* node) const;
    void updateHeight(Node* node);
    void updateMulInSubtree(Node* node);
    void updateKeySumInSubtree(Node* node);
    void updateValsMul(Node* node);
    int getMulInSubtree(Node* node) const;
    int getKeySumInSubtree(Node* node) const;
    int getValsMul(Node* node, int i) const;
    void updateNodeStructuralProperties(Node* node);
    int getBf(Node* node) const;

    // INSERT
    Node* rollRight(Node* B);
    Node* rollLeft(Node* A);
    Node* LL(Node* C);
    Node* LR(Node* C);
    Node* RL(Node* C);
    Node* RR(Node* C);
    Node* fixBalance(Node* node);
    Node* insertInSubtree(Node* node, int key, int val, bool* success);

    // FIND
    int get(int key, int* keyMulRank, int* keysSumRank, int* valsMulRank, int* keyVals) const;
    Node* findInSubtree(Node* node, int key, int* keyMulRank, int* keysSumRank, int* valsMulRank) const;

    // REMOVE
    Node* removeFromSubtree(Node* node, int key, int value, bool* success);
    Node* getNextInInorder(Node* node);
    Node* getSmallestDescendant(Node* node);
    void switchNodes(Node* higher_node, Node* lower_node);


    /*
    template <class function, class param>
    void applyInorderInternal(Node* node, function func, param p, int* done_nodes, int num_nodes);
    template <class function, class param>
    void applyInorderOnKey(function func, param p, int num_nodes);
    template <class function, class param>
    void applyInorderOnKeyInternal(Node* node, function func, param p, int* done_nodes, int num_nodes);

    static void mergeSortedArrays(int size1, int size2, OrderedPair* keys1, T* data1, OrderedPair* keys2, T* data, OrderedPair* keysMerged, T* dataMerged);
    Node* buildRankTreeOPKFromArray(int n, OrderedPair* keyArr, T* dataArr);
    template <class arrType>
    static void insertInArray(arrType* arr, arrType element, int index);
    Node* getMaxInSubtree(Node* root) const;
     */
    void printBT(const std::string& prefix, const Node* node, bool isLeft) const;



public:
    RankTreeOPK(int scale);
    RankTreeOPK(const RankTreeOPK&) = delete;
    RankTreeOPK& operator=(const RankTreeOPK&) = default; // shallow!
    ~RankTreeOPK();
    bool insert(int key, int val);
    double getPercentOfValueInKeyBounds(int lowerKey, int higherKey, int value);
    bool remove(int key, int value);


    /*

    RankTreeOPK(RankTreeOPK<T>* rankTreeOPK1, RankTreeOPK<T>* rankTreeOPK2); // merge constructor

    template <class function, class param>
    void applyInorder(function func, param p, int num_nodes);

    int getSize() const;
    T getMax() const;
     */
    void printBT() const;

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// C'tor & D'tor ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RankTreeOPK::RankTreeOPK(int scale) :root(nullptr), size(0), scale(scale) {}

RankTreeOPK::~RankTreeOPK() {
    deleteSubtree(root);
}

void RankTreeOPK::deleteSubtree(Node *n) {
    if (n == nullptr) {
        return;
    }
    deleteSubtree(n->left);
    deleteSubtree(n->right);
    delete n;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// NODE PROPERTIES//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int RankTreeOPK::getHeight(Node* node) const {
    if (node == nullptr) {
        return -1;
    }
    return (node->height);
}

void RankTreeOPK::updateHeight(Node* node) {
    if (node == nullptr) {
        return;
    }
    node->height = fmax(getHeight(node->right), getHeight(node->left)) + 1;
}

void RankTreeOPK::updateMulInSubtree(Node* node) {
    if (node == nullptr) {
        return;
    }
    node->mulInSubtree = getMulInSubtree(node->right) + getMulInSubtree(node->left) + node->mul;
}

void RankTreeOPK::updateKeySumInSubtree(Node* node) {
    if (node == nullptr) {
        return;
    }
    node->keysSumInSubtree = getKeySumInSubtree(node->right) + getKeySumInSubtree(node->left) + (node->key)*(node->mul);
}

void RankTreeOPK::updateValsMul(Node* node) {
    if (node == nullptr) {
        return;
    }
    for (int i=0; i< SCALE_MAX; ++i) {
        node->valsMul[i] = getValsMul(node->right, i) + getValsMul(node->left, i) + node->vals[i];
    }
}

int RankTreeOPK::getMulInSubtree(Node* node) const {
    if (node == nullptr) {
        return 0;
    }
    return node->mulInSubtree;
}

int RankTreeOPK::getKeySumInSubtree(Node* node) const {
    if (node == nullptr) {
        return 0;
    }
    return node->keysSumInSubtree;
}

int RankTreeOPK::getValsMul(Node* node, int i) const {
    if (node == nullptr) {
        return 0;
    }
    return (node->valsMul)[i];
}


void RankTreeOPK::updateNodeStructuralProperties(Node* node) {
    updateHeight(node);
    updateMulInSubtree(node);
    updateKeySumInSubtree(node);
    updateValsMul(node);
}


int RankTreeOPK::getBf(Node* node) const {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// INSERT //////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


typename RankTreeOPK::Node* RankTreeOPK::rollRight(Node* B) {
    Node* A = B->left;
    B->left = A->right;
    A->right = B;
    updateNodeStructuralProperties(B);
    updateNodeStructuralProperties(A);
    return A;
}

typename RankTreeOPK::Node* RankTreeOPK::rollLeft(Node* A) {
    Node* B = A->right;
    A->right = B->left;
    B->left = A;
    updateNodeStructuralProperties(A);
    updateNodeStructuralProperties(B);
    return B;
}

typename RankTreeOPK::Node* RankTreeOPK::LL(Node* C) {
    return rollRight(C);
}

typename RankTreeOPK::Node* RankTreeOPK::LR(Node* C) {
    C->left = rollLeft(C->left);
    return rollRight(C);
}

typename RankTreeOPK::Node* RankTreeOPK::RL(Node* C) {
    C->right = rollRight(C->right);
    return rollLeft(C);
}

typename RankTreeOPK::Node* RankTreeOPK::RR(Node* C) {
    return rollLeft(C);
}

typename RankTreeOPK::Node* RankTreeOPK::fixBalance(Node* node) {
    int bf_node = getBf(node);
    int bf_right = getBf(node->right);
    int bf_left = getBf(node->left);
    if (bf_node == 2) {
        if (bf_left >= 0) {
            return LL(node);
        }
        else {
            return LR(node);
        }
    }
    else {
        if (bf_right <= 0) {
            return RR(node);
        }
        else {
            return RL(node);
        }
    }
}

bool RankTreeOPK::insert(int key, int val) {
    bool success = true;
    root = insertInSubtree(root, key, val, &success);
    return success;
}

// insert node into subtree and return the head of the subtree after insertion
typename RankTreeOPK::Node* RankTreeOPK::insertInSubtree(Node* node, int key, int val, bool* success) {
    if (node == nullptr) {
        this->size += 1;
        Node* n = new Node(key, val);
        *success = true;
        return n;
    }
    if (key == node->key) {
        node->mul += 1;
        node->mulInSubtree += 1;
        node->keysSumInSubtree += node->key;
        node->vals[val] += 1;
        node->valsMul[val] += 1;
        *success = true;
        return node;
    }
    if (key < node->key) {
        node->left = insertInSubtree(node->left, key, val, success);
    }
    else if (key > node->key) {
        node->right = insertInSubtree(node->right, key, val, success);
    }
    updateNodeStructuralProperties(node);
    int bf = getBf(node);
    if (bf >= 2 || bf <= -2) {
        return fixBalance(node);
    }
    return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// FIND ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double RankTreeOPK::getPercentOfValueInKeyBounds(int lowerKey, int higherKey, int value) {
    int lowerKeyMulRank = 0;
    int lowerKeySumRank = 0;
    int lowerValsMulRank[SCALE_MAX] = {0};
    int lowerKeyVals[SCALE_MAX] = {0};
    int lowerMul = get(lowerKey, &lowerKeyMulRank, &lowerKeySumRank, lowerValsMulRank, lowerKeyVals);
    int higherKeyMulRank = 0;
    int higherKeySumRank = 0;
    int higherValsMulRank[SCALE_MAX] = {0};
    int higherKeyVals[SCALE_MAX] {0};
    int higherMul = get(higherKey, &higherKeyMulRank, &higherKeySumRank, higherValsMulRank, higherKeyVals);
    int keyMulInRange = higherKeyMulRank - lowerKeyMulRank + lowerMul;
    int valueMulInRange = higherValsMulRank[value] - lowerValsMulRank[value] + lowerKeyVals[value];
    return (double)valueMulInRange / (double)keyMulInRange;
}


int RankTreeOPK::get(int key, int* keyMulRank, int* keysSumRank, int* valsMulRank, int* keyVals) const {
    Node* n = findInSubtree(root, key, keyMulRank, keysSumRank, valsMulRank);
    if (n==nullptr) {
        return 0;
    }
    for (int i=0; i<SCALE_MAX; ++i) {
        keyVals[i] = (n->vals)[i];
    }
    return n->mul; // ???
}

typename RankTreeOPK::Node* RankTreeOPK::findInSubtree(Node* node, int key,
                                                       int* keyMulRank, int* keysSumRank, int* valsMulRank) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->key == key) {
        *keyMulRank += getMulInSubtree(node->left) + node->mul;
        *keysSumRank += getKeySumInSubtree(node->left) + node->key * node->mul;
        for (int i=0; i<SCALE_MAX; ++i) {
            valsMulRank[i] += getValsMul(node->left, i) + (node->vals)[i];
        }
        return node;
    }
    if (key < node->key) {
        return findInSubtree(node->left, key, keyMulRank, keysSumRank, valsMulRank);
    }
    else {
        *keyMulRank += getMulInSubtree(node->left) + node->mul;
        *keysSumRank += getKeySumInSubtree(node->left) + node->key * node->mul;
        for (int i=0; i<SCALE_MAX; ++i) {
            valsMulRank[i] += getValsMul(node->left, i) + (node->vals)[i];
        }
        return findInSubtree(node->right, key, keyMulRank, keysSumRank, valsMulRank);

    }
}
/*
template <class T>
T RankTreeOPK<T>::getMax() const {
    if (root==nullptr){
        return nullptr;
    }
    return getMaxInSubtree(root)->data;
}

template <class T>
typename RankTreeOPK<T>::Node* RankTreeOPK<T>::getMaxInSubtree(Node* node) const {
    if (node->right == nullptr) {
        return node;
    }
    else {
        return getMaxInSubtree(node->right);
    }
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// REMOVE //////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool RankTreeOPK::remove(int key, int value) {
    bool success;
    root = removeFromSubtree(root, key, value, &success);
    return success;
}

typename RankTreeOPK::Node* RankTreeOPK::removeFromSubtree(Node* node, int key, int value, bool* success) {
    if (node == nullptr) {
        *success = false;
        return nullptr;
    }
    if (node->key == key) {
        if (node->mul > 1) {
            node->mul -= 1;
            node->mulInSubtree -= 1;
            node->keysSumInSubtree -= node->key;
            (node->vals)[value] -= 1;
            (node->valsMul)[value] -= 1;
            *success = true;
            return node;
        }
        assert(node->mul == 1);
        if (node->right == nullptr && node->left == nullptr) {
            delete node;
            *success = true;
            size -= 1;
            return nullptr;
        }
        else if (node->right == nullptr && node->left != nullptr) {
            Node* new_node = node->left;
            delete node;
            *success = true;
            size -= 1;
            return new_node;
        }
        else if (node->right != nullptr && node->left == nullptr) {
            Node* new_node = node->right;
            delete node;
            *success = true;
            size -= 1;
            return new_node;
        }
        else { //node has two sons
            Node* low_node = getNextInInorder(node);
            switchNodes(node, low_node);
            node->right = removeFromSubtree(node->right, key, value, success);
            updateNodeStructuralProperties(node->right);
            updateNodeStructuralProperties(node);
            int bf = getBf(node);
            if (bf >= 2 || bf <= -2) {
                return fixBalance(node);
            }
            return node;
        }
    }
    if (key < node->key) {
        node->left = removeFromSubtree(node->left, key, value, success);
    }
    else {
        node->right = removeFromSubtree(node->right, key, value, success);
    }
    updateNodeStructuralProperties(node);
    int bf = getBf(node);
    if (bf >= 2 || bf <= -2) {
        return fixBalance(node);
    }
    return node;
}

typename RankTreeOPK::Node* RankTreeOPK::getNextInInorder(Node* node) {
    node = node->right; //not empty because node has 2 children
    return getSmallestDescendant(node);
}

typename RankTreeOPK::Node* RankTreeOPK::getSmallestDescendant(Node* node) {
    if (node->left == nullptr) {
        return node;
    }
    else {
        return getSmallestDescendant(node->left);
    }
}

//switches between key and dat of node and new_node
void RankTreeOPK:: switchNodes(Node* higher_node, Node* lower_node) {
    Node higher_tmp = *higher_node;
    Node lower_tmp = *lower_node;
    *higher_node = *lower_node;
    higher_node->left = higher_tmp.left;
    higher_node->right = higher_tmp.right;
    *lower_node = higher_tmp;
    lower_node->left = lower_tmp.left;
    lower_node->right = lower_tmp.right;
    updateNodeStructuralProperties(lower_node);
    updateNodeStructuralProperties(higher_node);
}


/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// APPLY_INORDER ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
template <class function, class param>
void RankTreeOPK<K, T>::applyInorder(function func, param p, int num_nodes) {
    int done_nodes = 0;
    applyInorderInternal<function, param>(root, func, p, &done_nodes, num_nodes);
}

template <class T>
template <class function, class param>
void RankTreeOPK<K, T>::applyInorderOnKey(function func, param p, int num_nodes) {
    int done_nodes = 0;
    applyInorderOnKeyInternal<function, param>(root, func, p, &done_nodes, num_nodes);
}

template <class T>
template <class function, class param>
void RankTreeOPK<K, T>::applyInorderInternal(Node* node, function func, param p, int* done_nodes, int num_nodes) {
    if (node == nullptr) {
        return;
    }
    applyInorderInternal(node->left, func, p, done_nodes, num_nodes);
    if (num_nodes == ALL_NODES || *done_nodes < num_nodes) {
        func(p, node->data, *done_nodes);
        (*done_nodes)++;
    }
    applyInorderInternal(node->right, func, p, done_nodes, num_nodes);
}

template <class T>
template <class function, class param>
void RankTreeOPK<K, T>::applyInorderOnKeyInternal(Node* node, function func, param p, int* done_nodes, int num_nodes) {
    if (node == nullptr || (num_nodes!=ALL_NODES && *done_nodes > num_nodes)) {
        return;
    }
    applyInorderOnKeyInternal(node->left, func, p, done_nodes, num_nodes);
    func(p, node->key, *done_nodes);
    (*done_nodes)++;
    applyInorderOnKeyInternal(node->right, func, p, done_nodes, num_nodes);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// BUILD_RankTreeOPK ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class T>
typename RankTreeOPK<T>::Node* RankTreeOPK<T>::buildRankTreeOPKFromArray(int tree_size, OrderedPair* keyArr, T* dataArr) {
    if (tree_size <= 0) {
        return nullptr;
    }
    int full_rows = log2(tree_size+1); // rounds down
    int remainder = tree_size + 1 - pow(2,full_rows); // number of nodes in last row
    int r1, r2; //
    int last_row_size = pow(2,full_rows);
    if (remainder <= last_row_size/2) {
        r1 = remainder;
        r2 = 0;
    }
    else {
        r1 = last_row_size/2;
        r2 = remainder - r1;
    }
    int left_size = pow(2,(full_rows-1)) - 1 + r1;
    int right_size = pow(2,(full_rows-1)) - 1 + r2;
    OrderedPair* leftKeyArr = keyArr;
    T* leftDataArr = dataArr;
    OrderedPair* rightKeyArr = keyArr+left_size + 1;
    T* rightDataArr = dataArr+left_size + 1;
    OrderedPair currentKey = keyArr[left_size];
    T currentData = dataArr[left_size];
    Node* n = new Node(currentKey, currentData);
    n->left = buildRankTreeOPKFromArray(left_size, leftKeyArr, leftDataArr);
    n->right = buildRankTreeOPKFromArray(right_size, rightKeyArr, rightDataArr);
    n->height = std::max(getHeight(n->left), getHeight(n->right)) + 1;
    return n;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// MERGE ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
int RankTreeOPK<T>::getSize() const {
    return this->size;
}

template <class T>
template <class arrType>
void RankTreeOPK<T>::insertInArray(arrType* arr, arrType element, int index) {
    arr[index] = element;
}

template <class T>
void RankTreeOPK<T>::mergeSortedArrays(int size1, int size2, OrderedPair* keys1, T* data1, OrderedPair* keys2, T* data2,
                                       OrderedPair* keysMerged, T* dataMerged) {
    int curr1 = 0;
    int curr2 = 0;
    int currI = 0;
    while (curr1 < size1 && curr2 < size2) {
        if (keys1[curr1] < keys2[curr2]) {
            keysMerged[currI] = keys1[curr1];
            dataMerged[currI] = data1[curr1];
            curr1++;
        } else {
            keysMerged[currI] = keys2[curr2];
            dataMerged[currI] = data2[curr2];
            curr2++;
        }
        currI++;
    }
    while (curr1 < size1) {
        keysMerged[currI] = keys1[curr1];
        dataMerged[currI] = data1[curr1];
        currI++;
        curr1++;
    }
    while (curr2 < size2) {
        keysMerged[currI] = keys2[curr2];
        dataMerged[currI] = data2[curr2];
        currI++;
        curr2++;
    }
}

template <class T>
// notice that this method does not delete the two old RankTreeOPKs!
RankTreeOPK<T>::RankTreeOPK(RankTreeOPK<T>* rankTreeOPK1, RankTreeOPK<T>* rankTreeOPK2) {
    int n1 = rankTreeOPK1->getSize();
    int n2 = rankTreeOPK2->getSize();
    T* dataArray1 = new T[n1];
    T* dataArray2 = new T[n2];
    rankTreeOPK1->applyInorder(insertInArray<T>, dataArray1, ALL_NODES);
    rankTreeOPK2->applyInorder(insertInArray<T>, dataArray2, ALL_NODES);
    OrderedPair* keyArray1 = new OrderedPair[n1];
    OrderedPair* keyArray2 = new OrderedPair[n2];
    rankTreeOPK1->applyInorderOnKey(insertInArray<OrderedPair>, keyArray1, ALL_NODES);
    rankTreeOPK2->applyInorderOnKey(insertInArray<OrderedPair>, keyArray2, ALL_NODES);
    T* dataMerged = new T[n1+ n2];
    OrderedPair* keysMerged = new OrderedPair[n1+n2];
    mergeSortedArrays(n1, n2, keyArray1, dataArray1, keyArray2, dataArray2, keysMerged, dataMerged);
    this->size = n1+n2;
    root = buildRankTreeOPKFromArray(size, keysMerged, dataMerged);
    delete[] dataArray1;
    delete[] dataArray2;
    delete[] keyArray1;
    delete[] keyArray2;
    delete[] dataMerged;
    delete[] keysMerged;
}

*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// PRINT ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RankTreeOPK::printBT(const std::string& prefix, const Node* node, bool isLeft) const {
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "|--" : "^--" );

        std::cout << "(" << node->key << ", " << node->mul <<", " << node->mulInSubtree << ", "
                                                                  << "[" << node->vals[0]
                                                                  << ", " << node->vals[1]
                                                                  << ", " << node->vals[2]
                                                                  << ", " << node->vals[3]
                                                                << "], [" << node->valsMul[0]
                                                                << ", " << node->valsMul[1]
                                                                << ", " << node->valsMul[2]
                                                                << ", " << node->valsMul[3]
                                                                  << "])" << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "|   " : "    "), node->left, true);
        printBT( prefix + (isLeft ? "|   " : "    "), node->right, false);
    }
}

void RankTreeOPK::printBT() const {
    std::cout << "koko" << std::endl;
    int x=5;
    std::cout << x << std::endl;
    std::cout << "key, mul, mulInSubtree, vals, valsMul" << std::endl;
    printBT("", root, false);
}

#endif // _RankTreeOPK_H