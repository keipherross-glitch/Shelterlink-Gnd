# ShelterLink Grenada

A C++ console application for coordinating hurricane shelters.

I built this data structures application for my final project in CIT234 Data Structures and Algorithms.

Shelter managers can register shelters and evacuees, search for people by name, and keep a waiting list when a shelter is full.

## Features

- Add shelters with a capacity
- Register an evacuee: housed if the shelter has space, otherwise added to that shelter's waiting list
- Search for an evacuee by name
- Discharge an evacuee, and the next person waiting is admitted
- Show each shelter's occupancy and number waiting

## Data structures

| Structure | Used for |
|---|---|
| Binary search tree | All evacuees, keyed by name |
| Queue | A waiting list for each shelter |
| Singly linked list | The registered shelters |

## How to run

You need a C++ compiler such as g++.

```
g++ -std=c++17 cpp/main.cpp -o shelterlink
./shelterlink
```

On Windows, you can also run `shelterlink.exe`.

## Menu

```
1. Add shelter
2. Register evacuee
3. Search by name
4. Discharge evacuee
5. Show shelters
0. Exit
```

## Known limitations

- Names are case-sensitive, and two evacuees cannot share the same name.
- Discharged evacuees stay in the tree, marked as discharged, instead of being deleted.
- Data is not saved when the program closes.
- The binary search tree is not self-balancing.

## Author

Keipher Ross, T.A. Marryshow Community College