# tournament-organizer

#### A few related programs that demonstrate profiency in C using various Data Structures and Algs.

### Project 1: Dynamic Memory Allocation in a Soccer Tournament System
This project is a C-based simulation of an international soccer tournament that leverages dynamic memory allocation and pointer manipulation to manage complex data structures. 
The system dynamically allocates memory to handle teams, players, matches, and goal events, ensuring efficient storage and retrieval of tournament data. 
Key functionalities include identifying the highest-scoring player, determining match winners, detecting hat tricks, and computing the tournament champion based on defined scoring rules. This assignment provided an in-depth exploration of dynamic memory techniques, rigorous pointer management, and the implementation of multiple command-based queries to analyze and process real-time sports data.

### Project 2: Soccer Match Ticketing Queues using Linked Lists
This project simulates a high-volume soccer match ticketing system using dynamic data structures in C. It implements 12 separate queues—each built as a linked list—to manage customer arrivals. Customers are assigned to a queue based on their arrival time, line number, and number of tickets they plan to purchase. The system then simulates the checkout process at a single counter, selecting the next customer based on who has the fewest tickets (with ties broken by line number) and calculates the processing time, which is determined by a fixed base time plus an incremental time per ticket. The assignment emphasizes dynamic memory allocation for customers and linked list nodes, as well as efficient queue operations (init, enqueue, dequeue, peek). This project provided a practical understanding of queues, linked lists, and dynamic memory management in the context of a real-world simulation.

### Project 3: The Perfect Seat Plan – Optimizing Seating Arrangements Using Permutations
This project addresses the challenge of finding valid seating arrangements for soccer fans under multiple constraints using recursive permutation techniques. It processes a list of attendees, their snack ownership status, and pairs of individuals who prefer not to sit next to each other. The project is split into two parts: one program calculates the total number of valid seating permutations, while the other finds the first valid arrangement in lexicographical order. By leveraging recursion and backtracking, the project demonstrates efficient handling of complex constraint satisfaction problems within a manageable problem size.

## Project 4: Nearest Stadium – Hybrid Sorting and Searching for Optimal Navigation
This project implements a robust search system to help soccer fans quickly locate the nearest stadium based on their current location. By representing both your location and stadium coordinates on a Cartesian plane, the project sorts the stadiums using a hybrid algorithm that combines Merge Sort and Insertion Sort. A threshold value determines when to switch from the divide-and-conquer approach of Merge Sort to the simpler, faster Insertion Sort for small subarrays. The sorted list is then used to answer queries via a Binary Search algorithm, which identifies whether a queried point corresponds to a stadium and, if so, its ranking by proximity. The project emphasizes efficient sorting, precise tie-breaking rules, and memory management, ensuring that the system performs optimally even with large input sizes. All tailored to provide an accurate and user-friendly navigation aid for fans.

## Project 5: Soccer Match Ticket Management Using Binary Search Trees
This project implements a ticket management system for a soccer tournament using a binary search tree (BST) to efficiently store and manage fan data. Each node in the BST represents a fan, storing their name and ticket count, with fan names serving as the key for alphabetical ordering. The system supports various operations such as adding and deducting tickets, searching for a fan’s record (including reporting depth in the tree), calculating the average and total ticket counts, checking tree balance, and summing ticket counts for fans with names preceding a given value. Deletion logic is used to maintain BST properties, including replacing nodes with two children by the maximum node from the left subtree. This project reinforces efficient data manipulation, recursive tree traversals, and dynamic memory management in C while meeting strict operational and performance requirement.






