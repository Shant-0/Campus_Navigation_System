# Campus Navigation System (C Project)

[![Language](https://img.shields.io/badge/Language-C-blue.svg)]()
[![Algorithm](https://img.shields.io/badge/Algorithm-Dijkstra-green.svg)]()
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)]()
[![Status](https://img.shields.io/badge/Status-Active-success.svg)]()


A terminal-based Campus Navigation Assistant that calculates

## 🚀 Features

- Shortest Path Navigation 

- Closest 3 Locations

- Human-readable directions (North, South-West, etc.)

- Distance-based routing

- Error-proof input handling

- Fast menu-driven interface


The graph is fully undirected with proper compass directions.

## 🔧 Technologies

+ Language: ANSI C (C99/C11)

+ Graph Storage: Adjacency List (Linked List)

+ Path Reconstruction: Parent pointers

+ Direction Chains: Multi-hop direction sequence builder

+ Input Normalization: Whitespace removal + lowercase conversion


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

