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
    Shelter* s = new Shelter;
    s->id = id;
    s->name = name;
    s->capacity = capacity;
    s->occupied = 0;
    s->next = head;     // new shelter points at the old first shelter
    head = s;           // and becomes the new first shelter
}

Shelter* findShelter(Shelter* head, int id) {
    Shelter* cur = head;
    while (cur != nullptr) {
        if (cur->id == id) return cur;   // found it
        cur = cur->next;                 // move to the next shelter
    }
    return nullptr;                      // reached the end, not found
}

// ---------- Step 4: Putting it together ----------
// If the shelter has space: occupied++, set shelterId, insert into the BST.
// If full: shelterId = 0, insert into the BST and enqueue the name.
void registerEvacuee(TreeNode*& root, Shelter* head, int shelterId, Evacuee e) {
    Shelter* s = findShelter(head, shelterId);
    if (s == nullptr) {
        cout << "Shelter " << shelterId << " not found" << endl;
        return;
    }
    if (searchEvacuee(root, e.name) != nullptr) {
        cout << "Already registered: " << e.name << endl;
        return;
    }
    if (s->occupied < s->capacity) {
        e.shelterId = shelterId;          // housed
        s->occupied++;
        insertEvacuee(root, e);
    } else {
        e.shelterId = 0;                  // waiting
        insertEvacuee(root, e);
        enqueue(s->waiting, e.name);
    }
}


// Mark the evacuee discharged, occupied--, then admit the next name from the
// waiting list (if any): look them up in the BST and update their shelterId.
void discharge(TreeNode* root, Shelter* head, const string& name) {
    TreeNode* node = searchEvacuee(root, name);
    if (node == nullptr) {
        cout << name << " not found" << endl;
        return;
    }
    if (node->data.shelterId <= 0) {
        cout << name << " is not in a shelter" << endl;
        return;
    }
    Shelter* s = findShelter(head, node->data.shelterId);
    node->data.shelterId = -1;            // discharged
    s->occupied--;

    string next;
    if (dequeue(s->waiting, next)) {      // someone is waiting
        TreeNode* n = searchEvacuee(root, next);
        n->data.shelterId = s->id;
        s->occupied++;
        cout << next << " moved from the waiting list into " << s->name << endl;
    }
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

 addShelter(shelters, 1, "Grand Anse Primary", 2);
addShelter(shelters, 2, "St Andrew's Church", 3);

Shelter* s = findShelter(shelters, 1);
if (s) cout << "Shelter " << s->id << ": " << s->name << ", capacity " << s->capacity << endl;
else cout << "Shelter 1 not found" << endl;

s = findShelter(shelters, 9);
cout << (s ? "Found 9" : "Shelter 9 not found") << endl;

registerEvacuee(root, shelters, 1, {"Cora", 41, 0});
registerEvacuee(root, shelters, 1, {"Dale", 25, 0});
registerEvacuee(root, shelters, 1, {"Eve", 60, 0});
cout << "Eve shelterId: " << searchEvacuee(root, "Eve")->data.shelterId << endl;
discharge(root, shelters, "Cora");
cout << "Eve shelterId: " << searchEvacuee(root, "Eve")->data.shelterId << endl;
cout << "Cora shelterId: " << searchEvacuee(root, "Cora")->data.shelterId << endl;
cout << "Shelter 1 occupied: " << findShelter(shelters, 1)->occupied << endl;
    return 0;
}

