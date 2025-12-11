# Campus Navigation System (C Project)

[![Language](https://img.shields.io/badge/Language-C-blue.svg)]()
[![Algorithm](https://img.shields.io/badge/Algorithm-Dijkstra-green.svg)]()
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)]()
[![Status](https://img.shields.io/badge/Status-Active-success.svg)]()


A terminal-based Campus Navigation Assistant that calculates:

✔ Shortest route between two campus locations 

✔ Step-by-step turn-by-turn directions 

✔ Closest 3 nearby locations 

✔ Distance between all nodes 

✔ Clean, interactive menu interface

Powered by Dijkstra’s Algorithm and an Adjacency List Graph.

## 🚀 Features

- Shortest Path Navigation 

- Closest 3 Locations

- Human-readable directions (North, South-West, etc.)

- Distance-based routing

- Error-proof input handling

- Fast menu-driven interface

## 🗺 Campus Nodes

The system includes 17 nodes representing real locations:

+ PunchGate

+ Joint01 — Joint04

+ Ground

+ WiFi

+ LectureGallery

+ MBA

+ Class107-111

+ Class112-115

+ Pigeonhole

+ PrayerHall

+ Journal

+ Auditorium

+ StairsCafe

+ Gate

The graph is fully undirected with proper compass directions.

## 🔧 Technologies

+ Language: ANSI C (C99/C11)

+ Graph Storage: Adjacency List (Linked List)

+ Path Reconstruction: Parent pointers

+ Direction Chains: Multi-hop direction sequence builder

+ Input Normalization: Whitespace removal + lowercase conversion

# 📐 Algorithm

Uses Dijkstra to compute shortest paths.

Two versions implemented:

* dijkstra()
→ Optimized to stop early for single-destination routing.

* dijkstra_all()
→ Computes all distances for “closest locations”.

## 📦 Compilation

```gcc -Wall -Wextra -O2 -o campus_nav campus_nav.c```

▶ Running 
```./campus_nav```

You will see:

```
=================================================
              CAMPUS NAVIGATION SYSTEM
=================================================

Known locations:
  • PunchGate
  • Joint01
  • Joint02
  ...
```
Menu:

```
1. Show known locations
2. Find route between two locations
3. Show 3 closest locations
4. Exit
```

## 🧱 Project Structure

Campus_navigation.c → Main program with all logic

README.md           → Documentation

LICENSE             → MIT License

## 🛠 Key Functions

+ build_graph()

+ add_edge()

+ dijkstra()

+ dijkstra_all()

+ reconstruct_path()

+ direction_chain()

+ print_route()

+ show_closest()

+ normalize_input()

+ prompt_for_node()

## 🧠 Future Improvements

+ GUI using JavaFX or GTK

+ Floor-wise navigation

+ Visual map rendering

+ Alias names (“C1”, “Punch”, “Prayer”)

+ JSON export of path results

+ A* algorithm integration

## 👨‍💻 Author

**Md. Junayed Ahmad Shanto**

Campus Navigation System — Pathfinding Project in C

## 🪪 License

Released under the MIT License.

