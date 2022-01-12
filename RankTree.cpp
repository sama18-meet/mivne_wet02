#include "RankTree.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// C'tor & D'tor ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RankTreeOPK::RankTreeOPK() : size(0), root(nullptr) {}

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

bool RankTreeOPK::getPercentOfValueInKeyBounds(int lowerKey, int higherKey, int value, double* res, int* keyMulInRange) const {
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
    *keyMulInRange = higherKeyMulRank - lowerKeyMulRank + lowerMul;
    int valueMulInRange=0;
    if (value < SCALE_MAX) {
        valueMulInRange = higherValsMulRank[value] - lowerValsMulRank[value] + lowerKeyVals[value];
    }
    if (*keyMulInRange == 0) {
        *res = 0;
        return false;
    }
    *res = 100 * (double)valueMulInRange / (double) *keyMulInRange;
    return true;
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// MERGE ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// notice that this method does not delete the two old RankTreeOPKs!
RankTreeOPK::RankTreeOPK(RankTreeOPK* rt1, RankTreeOPK* rt2) {
    int n1 = rt1->getSize();
    int n2 = rt2->getSize();
    Node* arr1 = new Node[n1];
    Node* arr2 = new Node[n2];
    rt1->applyInorder(insertNodeInArray, arr1);
    rt2->applyInorder(insertNodeInArray, arr2);
    Node* arrMerged = new Node[n1+ n2];
    int new_size = mergeSortedArrays(n1, n2, arr1, arr2, arrMerged);
    this->size = new_size;
    root = buildRankTreeOPKFromArray(size, arrMerged);
    delete[] arr1;
    delete[] arr2;
    delete[] arrMerged;
}

int RankTreeOPK::getSize() const {
    return this->size;
}

template <class function, class param>
void RankTreeOPK::applyInorder(function func, param p) const {
    int done_nodes = 0;
    applyInorderInternal<function, param>(root, func, p, &done_nodes);
}


template <class function, class param>
void RankTreeOPK::applyInorderInternal(Node* node, function func, param p, int* done_nodes) const {
    if (node == nullptr) {
        return;
    }
    applyInorderInternal(node->left, func, p, done_nodes);
    func(p, *node, *done_nodes);
    (*done_nodes)++;
    applyInorderInternal(node->right, func, p, done_nodes);
}

void RankTreeOPK::insertNodeInArray(Node* arr, Node node, int idx) {
    arr[idx] = node;
}


int RankTreeOPK::mergeSortedArrays(int size1, int size2, Node* arr1, Node* arr2, Node* arrMerged) {
    int curr1 = 0;
    int curr2 = 0;
    int currI = 0;
    while (curr1 < size1 && curr2 < size2) {
        if (arr1[curr1].key < arr2[curr2].key) {
            arrMerged[currI] = arr1[curr1];
            curr1++;
        } else if (arr1[curr1].key > arr2[curr2].key) {
            arrMerged[currI] = arr2[curr2];
            curr2++;
        }
        else { // equal
            Node node = Node(arr1[curr1].key, 0);
            node.mul = arr1[curr1].mul + arr2[curr2].mul;
            for (int i=0; i<SCALE_MAX; ++i) {
                node.vals[i] = arr1[curr1].vals[i] + arr2[curr2].vals[i];
            }
            arrMerged[currI] = node;
            curr1++;
            curr2++;
        }
        currI++;
    }
    while (curr1 < size1) {
        arrMerged[currI] = arr1[curr1];
        currI++;
        curr1++;
    }
    while (curr2 < size2) {
        arrMerged[currI] = arr2[curr2];
        currI++;
        curr2++;
    }
    return currI;
}

typename RankTreeOPK::Node* RankTreeOPK::buildRankTreeOPKFromArray(int tree_size, Node* arr) {
    if (tree_size <= 0) {
        return nullptr;
    }
    int full_rows = log2(tree_size+1); // rounds down
    int remainder = tree_size + 1 - pow(2,full_rows); // number of nodes in last row
    int r1, r2;
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

    Node* leftArr = arr;
    Node* rightArr = arr+left_size+1;
    Node currentNode = arr[left_size];
    Node* n = new Node(currentNode);
    n->left = buildRankTreeOPKFromArray(left_size, leftArr);
    n->right = buildRankTreeOPKFromArray(right_size, rightArr);
    updateNodeStructuralProperties(n);
    return n;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// GET_AVG_M_HIGHEST ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double RankTreeOPK::getAvgHighest(int m) const {
    assert(m>0);
    int sum = 0;
    int lowerBound = 0;
    int upperBound = 0;
    getSumOfHighestKeys(root, m, &sum, 0, &lowerBound, &upperBound);
    return (double)sum / (double)m;
}

bool RankTreeOPK::getValRange(int val, int m, int* lowerBound, int* upperBound) const {
    assert(m>=0);
    int sum = 0;
    *lowerBound = 0;
    *upperBound = 0;
    if (getSumOfHighestKeys(root, m, &sum, val, lowerBound, upperBound) == false) {
        if (root == nullptr && m == 0) { //TODO ask sama to look not sure this is correct
            return true;
        }
        assert(size < m);
        return false; // there are no M players
    }
    return true;
}

bool RankTreeOPK::getSumOfHighestKeys(Node* node, int m, int* current_sum, int val, int* lowerBound, int* upperBound) const {
    assert(m>=0);
    if (node == nullptr) {
        return false;
    }
    Node* prev_node = nullptr;
    while (node != nullptr && node->mulInSubtree > m) {
        prev_node = node;
        node = node->right;
    }
    if (prev_node == nullptr) { // didn't enter loop
        if (node->mulInSubtree < m) { // there aren't enough players
            return false;
        }
        else { // node->mulInSubtree == m
            //assert(node->mulInSubtree == m); TODO
            *current_sum += node->keysSumInSubtree;
            *lowerBound += node->valsMul[val];
            *upperBound += node->valsMul[val];
            return true;
        }
    }
    if (node == nullptr) {
        assert(prev_node != nullptr);
        //assert(prev_node->mul > m); TODO check with sama why it's here
        *current_sum += m*prev_node->key;
        int not_val_mul = prev_node->mul - prev_node->vals[val];
        *lowerBound += not_val_mul >= m ? 0 : m-not_val_mul;
        *upperBound += prev_node->vals[val];
        return true;
    }
    if (node->mulInSubtree == m) {
        *current_sum += node->keysSumInSubtree;
        *lowerBound += node->valsMul[val];
        *upperBound += node->valsMul[val];
        return true;
    }
    // else: node->mulInsubtree < m
    *current_sum += node->keysSumInSubtree;
    m = m - node->mulInSubtree;
    if (prev_node->mul >= m) {
        int not_val_mul = prev_node->mul - prev_node->vals[val];
        *lowerBound += not_val_mul >= m ? 0 :m-not_val_mul;
        *upperBound += fmin(m, prev_node->vals[val]);
        *current_sum += m*prev_node->key;
        return true;
    }
    else { // (prev_node->mul < m)
        *current_sum += prev_node->mul*prev_node->key;
        *lowerBound += prev_node->vals[val];
        *upperBound += prev_node->vals[val];
        m -= prev_node->mul;
        return getSumOfHighestKeys(prev_node->left, m, current_sum, val, lowerBound, upperBound);
    }
}

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
    std::cout << "printing RankTree" << std::endl;
    std::cout << "key, mul, mulInSubtree, vals, valsMul" << std::endl;
    printBT("", root, false);
}
