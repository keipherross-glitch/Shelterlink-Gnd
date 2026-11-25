// ShelterLink Grenada - starter skeleton (CIT234)
// Structs and function stubs only. You write the function bodies.
// Build: g++ -std=c++17 shelterlink_starter.cpp -o shelterlink
#include <iostream>
#include <string>
using namespace std;

// ---------- Data structures ----------

// One evacuee. shelterId: 0 = waiting, -1 = discharged, otherwise the shelter's id.
struct Evacuee {
    string name;
    int age;
    int shelterId;
};

// Node of the binary search tree (evacuees keyed by name).
struct TreeNode {
    Evacuee data;
    TreeNode* left;
    TreeNode* right;
};

// Node of a waiting-list queue. Stores only the evacuee's name;
// the full record lives in the BST.
struct QueueNode {
    string name;
    QueueNode* next;
};

struct Queue {
    QueueNode* front = nullptr;
    QueueNode* back = nullptr;
};

// Node of the linked list of shelters. Each shelter has its own waiting list.
struct Shelter {
    int id;
    string name;
    int capacity;
    int occupied;
    Queue waiting;
    Shelter* next;
};

// ---------- Step 1: BST ----------
// Insert by name. Take root by reference (TreeNode*&) so the new node is linked in.
// Decide what to do with a duplicate name.
void insertEvacuee(TreeNode*& root, const Evacuee& e) {
    // TODO (recursive): empty spot -> create node; else go left or right
}

// Return a pointer to the matching node, or nullptr if not found.
TreeNode* searchEvacuee(TreeNode* root, const string& name) {
    // TODO (recursive)
    return nullptr;
}

// ---------- Step 2: Queue ----------
void enqueue(Queue& q, const string& name) {
    // TODO: add at the back (watch the empty-queue case)
}

// Returns false if the queue is empty, otherwise puts the front name in 'name'.
bool dequeue(Queue& q, string& name) {
    // TODO: remove from the front (watch the case where the queue becomes empty)
    return false;
}

// ---------- Step 3: Linked list of shelters ----------
void addShelter(Shelter*& head, int id, const string& name, int capacity) {
    // TODO
}

Shelter* findShelter(Shelter* head, int id) {
    // TODO: walk the list
    return nullptr;
}

// ---------- Step 4: Putting it together ----------
// If the shelter has space: occupied++, set shelterId, insert into the BST.
// If full: shelterId = 0, insert into the BST and enqueue the name.
void registerEvacuee(TreeNode*& root, Shelter* head, int shelterId, Evacuee e) {
    // TODO
}

// Mark the evacuee discharged, occupied--, then admit the next name from the
// waiting list (if any): look them up in the BST and update their shelterId.
void discharge(TreeNode* root, Shelter* head, const string& name) {
    // TODO
}

// ---------- Step 5: Menu ----------
int main() {
    TreeNode* root = nullptr;
    Shelter* shelters = nullptr;

    // TODO: menu loop
    //  1. Add shelter   2. Register evacuee   3. Search by name
    //  4. Discharge evacuee   5. Show shelters   0. Exit
    return 0;
}
