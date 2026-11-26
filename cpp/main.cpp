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
 int countWaiting(const Queue& q) {
    int n = 0;
    for (QueueNode* cur = q.front; cur != nullptr; cur = cur->next) n++;
    return n;
}

void showShelters(Shelter* head) {
    if (head == nullptr) {
        cout << "No shelters registered." << endl;
        return;
    }
    for (Shelter* s = head; s != nullptr; s = s->next) {
        cout << s->id << ". " << s->name << " - " << s->occupied << "/" << s->capacity
             << " housed, " << countWaiting(s->waiting) << " waiting" << endl;
    }
}

string describe(const Evacuee& e, Shelter* head) {
    if (e.shelterId > 0) {
        Shelter* s = findShelter(head, e.shelterId);
        return "housed in " + (s ? s->name : string("unknown shelter"));
    }
    if (e.shelterId == 0) return "on the waiting list";
    return "discharged";
}

int readInt(const string& prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a number: ";
    }
    cin.ignore(1000, '\n');
    return x;
}
int countEvacuees(TreeNode* root) {
    if (root == nullptr) return 0;
    return 1 + countEvacuees(root->left) + countEvacuees(root->right);
}

void collectInOrder(TreeNode* root, Evacuee arr[], int& idx) {
    if (root == nullptr) return;
    collectInOrder(root->left, arr, idx);
    arr[idx++] = root->data;
    collectInOrder(root->right, arr, idx);
}

void bubbleSort(Evacuee arr[], int n, long long& comparisons) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            comparisons++;
            if (arr[j].age < arr[j + 1].age) {
                Evacuee temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void insertionSort(Evacuee arr[], int n, long long& comparisons) {
    for (int i = 1; i < n; i++) {
        Evacuee key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            comparisons++;
            if (arr[j].age < key.age) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

int main() {
    TreeNode* root = nullptr;
    Shelter* shelters = nullptr;
    int choice = -1;

    do {
        cout << "\n=== ShelterLink Grenada ===\n"
             << "1. Add shelter\n2. Register evacuee\n3. Search by name\n"
             << "4. Discharge evacuee\n5. Show shelters\n6. List evacuees by age (compare sorts)\n0. Exit\n";
        choice = readInt("Choice: ");

        switch (choice) {
        case 1: {
            int id = readInt("Shelter id: ");
            if (findShelter(shelters, id) != nullptr) {
                cout << "That shelter id already exists." << endl;
                break;
            }
            string name;
            cout << "Shelter name: ";
            getline(cin, name);
            int cap = readInt("Capacity: ");
            if (cap <= 0) {
                cout << "Capacity must be more than 0." << endl;
                break;
            }
            addShelter(shelters, id, name, cap);
            cout << "Shelter added." << endl;
            break;
        }
        case 2: {
            int sid = readInt("Shelter id: ");
            string name;
            cout << "Evacuee name: ";
            getline(cin, name);
            int age = readInt("Age: ");
            registerEvacuee(root, shelters, sid, {name, age, 0});
            TreeNode* n = searchEvacuee(root, name);
            if (n != nullptr) cout << name << " is " << describe(n->data, shelters) << "." << endl;
            break;
        }
        case 3: {
            string name;
            cout << "Name to search: ";
            getline(cin, name);
            TreeNode* n = searchEvacuee(root, name);
            if (n == nullptr) cout << name << " is not registered." << endl;
            else cout << n->data.name << ", age " << n->data.age << ": "
                      << describe(n->data, shelters) << "." << endl;
            break;
        }
       case 4: {
            string name;
            cout << "Name to discharge: ";
            getline(cin, name);
            discharge(root, shelters, name);
            break;
        }
        case 5:
            showShelters(shelters);
            break;
        case 6: {
            int n = countEvacuees(root);
            if (n == 0) {
                cout << "No evacuees registered." << endl;
                break;
            }
            Evacuee* a = new Evacuee[n];
            Evacuee* b = new Evacuee[n];
            int idx = 0;
            collectInOrder(root, a, idx);
            for (int i = 0; i < n; i++) b[i] = a[i];

            long long bubbleCmp = 0, insertCmp = 0;
            bubbleSort(a, n, bubbleCmp);
            insertionSort(b, n, insertCmp);

            cout << "Evacuees by age, oldest first:" << endl;
            for (int i = 0; i < n; i++)
                cout << i + 1 << ". " << a[i].name << ", age " << a[i].age
                     << " - " << describe(a[i], shelters) << endl;
            cout << "Bubble sort: " << bubbleCmp << " comparisons" << endl;
            cout << "Insertion sort: " << insertCmp << " comparisons" << endl;

            delete[] a;
            delete[] b;
            break;
        }
        case 0:
            cout << "Goodbye." << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
        }
    } while (choice != 0);

    return 0;
}
