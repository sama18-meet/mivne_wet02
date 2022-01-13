#ifndef _RankTreeOPK_H
#define _RankTreeOPK_H

#include <iostream>
#include <cmath>
#include <assert.h>

#define ALL_NODES -1
#define SCALE_MAX 201

class RankTreeOPK {
private:
    int size;
    struct Node {
        int key;
        int mul;
        int mulInSubtree;
        int keysSumInSubtree;
        struct Node* left;
        struct Node* right;
        int height;
        int vals[SCALE_MAX];
        int valsMul[SCALE_MAX];
        Node() = default;
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
            return *this;
        };
        Node(const Node& n) {
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


    // MERGE
    template <class function, class param>
    void applyInorder(function func, param p) const;
    template <class function, class param>
    void applyInorderInternal(Node* node, function func, param p, int* done_nodes) const;
    static void insertNodeInArray(Node* arr, Node node, int idx);
    int mergeSortedArrays(int size1, int size2, Node* arr1, Node* arr2, Node* arrMerged);
    Node* buildRankTreeOPKFromArray(int tree_size, Node* arr);

    // AVG HIGHEST
    bool getSumOfHighestKeys(Node* node, int m, int* current_sum, int val, int* lowerBound, int* upperBound) const;

    /*
    Node* getMaxInSubtree(Node* root) const;
     */
    void printBT(const std::string& prefix, const Node* node, bool isLeft) const;



public:
    RankTreeOPK();
    RankTreeOPK(const RankTreeOPK&) = default; // shallow!
    RankTreeOPK& operator=(const RankTreeOPK&) = default; // shallow!
    ~RankTreeOPK();
    bool insert(int key, int val);
    bool getPercentOfValueInKeyBounds(int lowerKey, int higherKey, int value, double* res, int* keyMulInRange) const;
    bool remove(int key, int value);
    RankTreeOPK(RankTreeOPK* rt1, RankTreeOPK* rt2); // merge constructor. Does not delete old avls!
    int getSize() const;
    double getAvgHighest(int m) const;
    bool getValRange(int val, int m, int* lowerBound, int* upperBound) const;
    void printBT() const;

};

#endif // _RankTreeOPK_H