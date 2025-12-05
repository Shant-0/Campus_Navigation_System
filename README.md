# Navigation-System(C)
This project implements a simple command-line application in C to find the shortest path between locations on a predefined map using Dijkstra's Algorithm.

# 🚀 Features

Shortest Path Calculation: Finds the path with the minimum total distance (weight).

Step-by-Step Navigation: Provides clear, directional instructions for the route.

Undirected Graph: All paths can be traveled in both directions.

Interactive Interface: Allows the user to repeatedly query for new routes until they choose to exit.

# 🧠 How It Works (Technical Overview)

**Graph Representation:** The campus map is stored as an Adjacency List using C structs (Edge). Each edge stores the destination node, the distance (weight), and the direction (dir).

**Algorithm:** The dijkstra function is the core of the system. It uses a greedy approach to find the minimum distance from the source node to every other reachable node.

**Path Reconstruction:** Instead of just calculating the distance, Dijkstra's algorithm also populates a parent array. The print_route function uses this array to backtrack from the destination to the source, reconstructing the correct sequence of nodes and printing the stored directional instructions.

**Error Handling:** Includes checks for invalid node names and cases where no path exists.

