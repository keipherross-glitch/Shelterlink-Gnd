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
    if (root == nullptr) {
        root = new TreeNode{e, nullptr, nullptr};
    } else if (e.name < root->data.name) {
        insertEvacuee(root->left, e);
    } else if (e.name > root->data.name) {
        insertEvacuee(root->right, e);
    } else {
        cout << "Already registered: " << e.name << endl;
    }
}



// Return a pointer to the matching node, or nullptr if not found
   TreeNode* searchEvacuee(TreeNode* root, const string& name) {
    if (root == nullptr) return nullptr;       // not found
    if (name == root->data.name) return root;  // found
    if (name < root->data.name) return searchEvacuee(root->left, name);
    return searchEvacuee(root->right, name);
}
// ---------- Step 2: Queue ----------
void enqueue(Queue& q, const string& name) {
    QueueNode* node = new QueueNode{name, nullptr};
    if (q.front == nullptr) {      // queue is empty
        q.front = node;
        q.back = node;
    } else {
        q.back->next = node;       // link after the current last node
        q.back = node;             // the new node is now the last
    }
}


// Returns false if the queue is empty, otherwise puts the front name in 'name'.
bool dequeue(Queue& q, string& name) {
    if (q.front == nullptr) return false;   // nothing to remove
    QueueNode* old = q.front;
    name = old->name;                       // hand the name back to the caller
    q.front = q.front->next;                // move the front along
    if (q.front == nullptr) q.back = nullptr;   // queue is now empty
    delete old;
    return true;
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

    // ---- temporary BST test ----
    insertEvacuee(root, {"Linda", 34, 1});
    insertEvacuee(root, {"Brian", 52, 1});
    insertEvacuee(root, {"Peter", 7, 2});
    insertEvacuee(root, {"Anna", 29, 1});
    insertEvacuee(root, {"Linda", 34, 1});   // duplicate on purpose

    TreeNode* found = searchEvacuee(root, "Brian");
    if (found) cout << "Found: " << found->data.name << ", age " << found->data.age << endl;
    else cout << "Brian not found" << endl;

    found = searchEvacuee(root, "Mark");
    if (found) cout << "Found: " << found->data.name << endl;
    else cout << "Mark not found" << endl;
    // ---- end test ----

  Queue q;
 enqueue(q, "Linda");
 enqueue(q, "Brian");
 string n;
 while (dequeue(q, n)) cout << "Admitted: " << n << endl;
    return 0;
}

