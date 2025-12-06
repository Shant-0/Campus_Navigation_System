/*
  campus_nav_final2.c
  Final Campus Navigation System (menu-driven, plain text, warning-free)

  - Menu:
      1) Navigate between two locations
      2) Find closest amenities (Top 3)
      3) Show all known nodes
      4) Exit
  - Case- and space-insensitive input
  - Uses adjacency list + Dijkstra
  - Closest amenities: Top 3 numbered, show distance + full direction chain
  - Step-by-step navigation for full routing
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAXV 17
#define INF  (INT_MAX/4)
#define DIRLEN 24
#define NAMEBUF 64

/* Node list (original order) */
const char *names[MAXV] = {
    "PunchGate",
    "Joint01",
    "Joint02",
    "Joint03",
    "Joint04",
    "Ground",
    "WiFi",
    "LectureGallery",
    "MBA",
    "Class107-111",
    "Class112-115",
    "Pigeonhole",
    "PrayerHall",
    "Journal",
    "Auditorium",
    "StairsCafe",
    "Gate"
};

/* Indices */
enum {
    V_PUNCH = 0, V_J01, V_J02, V_J03, V_J04, V_GND, V_WIFI, V_LG,
    V_MBA, V_C107, V_C112, V_PIGE, V_PRAY, V_JOURNAL, V_AUDIT, V_STCAFE, V_GATE
};

/* Adjacency list edge */
typedef struct Edge {
    int to;
    int weight;
    char dir[DIRLEN];   /* full compass word e.g., "North-East" */
    struct Edge *next;
} Edge;

Edge *adj[MAXV] = { NULL };

/* Opposite direction */
const char *opposite_dir(const char *d) {
    if (strcmp(d, "North") == 0) return "South";
    if (strcmp(d, "South") == 0) return "North";
    if (strcmp(d, "East") == 0)  return "West";
    if (strcmp(d, "West") == 0)  return "East";
    if (strcmp(d, "North-East") == 0) return "South-West";
    if (strcmp(d, "South-West") == 0) return "North-East";
    if (strcmp(d, "North-West") == 0) return "South-East";
    if (strcmp(d, "South-East") == 0) return "North-West";
    return d;
}

/* Add directed edge */
void add_edge_dir(int from, int to, int w, const char *dir) {
    Edge *e = malloc(sizeof(Edge));
    if (!e) { perror("malloc"); exit(1); }
    e->to = to;
    e->weight = w;
    strncpy(e->dir, dir, DIRLEN-1);
    e->dir[DIRLEN-1] = '\0';
    e->next = adj[from];
    adj[from] = e;
}

/* Add undirected edge (auto reverse directions) */
void add_edge_und(int a, int b, int w, const char *dir_ab) {
    add_edge_dir(a, b, w, dir_ab);
    add_edge_dir(b, a, w, opposite_dir(dir_ab));
}

/* Build graph using your final confirmed table */
void build_graph(void) {
    for (int i = 0; i < MAXV; ++i) adj[i] = NULL;

    add_edge_und(V_PUNCH, V_J01, 35, "North");

    add_edge_und(V_J01, V_AUDIT, 55, "West");
    add_edge_und(V_J01, V_GND, 45, "North-West");
    add_edge_und(V_J01, V_J04, 70, "North");
    add_edge_und(V_J01, V_J02, 115, "West");
    add_edge_und(V_J01, V_STCAFE, 85, "West");

    add_edge_und(V_PRAY, V_J04, 15, "West");

    add_edge_und(V_J04, V_PIGE, 20, "West");
    add_edge_und(V_J04, V_GND, 45, "South-West");

    add_edge_und(V_PIGE, V_C107, 25, "West");

    add_edge_und(V_C107, V_GATE, 15, "West");

    add_edge_und(V_GND, V_GATE, 40, "North");
    add_edge_und(V_GND, V_AUDIT, 40, "South");
    add_edge_und(V_GND, V_WIFI, 20, "West");
    add_edge_und(V_GND, V_STCAFE, 45, "South-West");

    add_edge_und(V_GATE, V_C112, 15, "West");

    add_edge_und(V_C112, V_J03, 10, "West");

    add_edge_und(V_J03, V_WIFI, 45, "South");
    add_edge_und(V_J03, V_MBA, 15, "West");
    add_edge_und(V_J03, V_LG, 45, "South-West");
    add_edge_und(V_J03, V_STCAFE, 70, "South");

    add_edge_und(V_LG, V_J02, 30, "South");
    add_edge_und(V_LG, V_JOURNAL, 45, "South");

    add_edge_und(V_J02, V_JOURNAL, 15, "South");
    add_edge_und(V_J02, V_STCAFE, 30, "East");
    add_edge_und(V_J02, V_AUDIT, 60, "East");

    add_edge_und(V_STCAFE, V_WIFI, 25, "North");
}

/* Normalize: remove spaces and lowercase */
void normalize_input(const char *raw, char *out) {
    int j = 0;
    for (int i = 0; raw[i] != '\0'; ++i) {
        if (!isspace((unsigned char)raw[i])) {
            out[j++] = (char)tolower((unsigned char)raw[i]);
        }
    }
    out[j] = '\0';
}

/* Build lookup of normalized node names */
void build_name_lookup(char lookup[MAXV][NAMEBUF]) {
    for (int i = 0; i < MAXV; ++i) {
        char buf[NAMEBUF];
        int j = 0;
        for (int k = 0; names[i][k] != '\0'; ++k) {
            if (!isspace((unsigned char)names[i][k])) buf[j++] = (char)tolower((unsigned char)names[i][k]);
        }
        buf[j] = '\0';
        strncpy(lookup[i], buf, NAMEBUF-1);
        lookup[i][NAMEBUF-1] = '\0';
    }
}

/* Find index by normalized input */
int index_of_normalized(const char *raw, char lookup[MAXV][NAMEBUF]) {
    char norm[NAMEBUF];
    normalize_input(raw, norm);
    for (int i = 0; i < MAXV; ++i) if (strcmp(norm, lookup[i]) == 0) return i;
    return -1;
}

/* Trim newline + leading/trailing spaces */
void trim_newline_and_spaces(char *s) {
    int L = (int)strlen(s);
    while (L > 0 && (s[L-1] == '\n' || s[L-1] == '\r')) s[--L] = '\0';
    int st = 0;
    while (s[st] && isspace((unsigned char)s[st])) ++st;
    if (st) memmove(s, s + st, strlen(s + st) + 1);
    /* trim trailing spaces again */
    L = (int)strlen(s);
    while (L > 0 && isspace((unsigned char)s[L-1])) s[--L] = '\0';
}

/* ---------- Dijkstra (single target) ---------- */
void dijkstra(int src, int dst, int parent[], char parent_dir[MAXV][DIRLEN], int *out_dist) {
    int dist[MAXV];
    int used[MAXV];
    for (int i = 0; i < MAXV; ++i) {
        dist[i] = INF; used[i] = 0; parent[i] = -1; parent_dir[i][0] = '\0';
    }
    dist[src] = 0;

    for (int iter = 0; iter < MAXV; ++iter) {
        int v = -1;
        for (int i = 0; i < MAXV; ++i)
            if (!used[i] && (v == -1 || dist[i] < dist[v])) v = i;
        if (v == -1 || dist[v] == INF) break;
        used[v] = 1;
        if (v == dst) break;
        for (Edge *e = adj[v]; e != NULL; e = e->next) {
            int to = e->to, w = e->weight;
            if (dist[v] + w < dist[to]) {
                dist[to] = dist[v] + w;
                parent[to] = v;
                strncpy(parent_dir[to], e->dir, DIRLEN-1);
                parent_dir[to][DIRLEN-1] = '\0';
            }
        }
    }
    *out_dist = dist[dst];
}

/* Dijkstra to all nodes */
void dijkstra_all(int src, int parent[], char parent_dir[MAXV][DIRLEN], int out_distances[MAXV]) {
    int dist[MAXV];
    int used[MAXV];
    for (int i = 0; i < MAXV; ++i) {
        dist[i] = INF; used[i] = 0; parent[i] = -1; parent_dir[i][0] = '\0';
    }
    dist[src] = 0;

    for (int iter = 0; iter < MAXV; ++iter) {
        int v = -1;
        for (int i = 0; i < MAXV; ++i)
            if (!used[i] && (v == -1 || dist[i] < dist[v])) v = i;
        if (v == -1 || dist[v] == INF) break;
        used[v] = 1;
        for (Edge *e = adj[v]; e != NULL; e = e->next) {
            int to = e->to, w = e->weight;
            if (dist[v] + w < dist[to]) {
                dist[to] = dist[v] + w;
                parent[to] = v;
                strncpy(parent_dir[to], e->dir, DIRLEN-1);
                parent_dir[to][DIRLEN-1] = '\0';
            }
        }
    }
    for (int i = 0; i < MAXV; ++i) out_distances[i] = dist[i];
}

/* Reconstruct path from src to dst into out_path (returns length), 0 if no path */
int reconstruct_path(int src, int dst, int parent[], int out_path[]) {
    int tmp[MAXV], t = 0;
    int cur = dst;
    while (cur != -1) {
        tmp[t++] = cur;
        if (cur == src) break;
        cur = parent[cur];
    }
    if (t == 0 || tmp[t-1] != src) return 0;
    for (int i = 0; i < t; ++i) out_path[i] = tmp[t-1-i];
    return t;
}

/* Get direction of edge from 'from' to 'to' (empty string if not direct) */
const char* edge_dir(int from, int to) {
    for (Edge *e = adj[from]; e; e = e->next) if (e->to == to) return e->dir;
    return "";
}

/* Print known nodes in original order */
void print_known_nodes_beautiful(void) {
    printf("\n--------------------------------------------\n");
    printf("Known nodes (original order):\n");
    printf("--------------------------------------------\n");
    for (int i = 0; i < MAXV; ++i) printf("  • %s\n", names[i]);
    printf("--------------------------------------------\n\n");
}

/* Header */
void print_header(void) {
    printf("=================================================\n");
    printf("              CAMPUS NAVIGATION SYSTEM           \n");
    printf("=================================================\n\n");
}

/* Read a trimmed line */
int read_trimmed_line(char *buf, int size) {
    if (!fgets(buf, size, stdin)) return 0;
    trim_newline_and_spaces(buf);
    return 1;
}

/* Prompt for valid node (re-ask until valid or 'exit' typed) */
int prompt_for_node(const char *prompt, char lookup[MAXV][NAMEBUF]) {
    char line[128];
    while (1) {
        printf("%s", prompt);
        if (!read_trimmed_line(line, sizeof(line))) return -1;
        if (strlen(line) == 0) continue;
        if (strcmp(line, "exit") == 0 || strcmp(line, "EXIT") == 0) return -1;
        int idx = index_of_normalized(line, lookup);
        if (idx >= 0) return idx;
        printf("\n[ERROR] Unknown location: '%s'\n", line);
        print_known_nodes_beautiful();
    }
}

/* Pretty print route (uses adjacency to show directions) */
void print_route_pretty(int src, int dst, int parent[], int dist_total) {
    if (dist_total >= INF) {
        printf("\n[ERROR] No possible path between %s and %s.\n\n", names[src], names[dst]);
        return;
    }
    int path[MAXV];
    int n = reconstruct_path(src, dst, parent, path);
    if (n == 0) {
        printf("\n[ERROR] No path found between %s and %s.\n\n", names[src], names[dst]);
        return;
    }

    printf("\nTotal distance: %d units\n\n", dist_total);
    printf("Step-by-step navigation:\n");
    for (int i = 0; i < n - 1; ++i) {
        int from = path[i], to = path[i+1];
        const char *dir = edge_dir(from, to);
        int w = -1;
        for (Edge *e = adj[from]; e; e = e->next) if (e->to == to) { w = e->weight; break; }
        if (w < 0) printf("  → From %-15s ??? to %-15s (unknown)\n", names[from], names[to]);
        else printf("  → From %-15s go %-13s to %-15s (%d)\n", names[from], dir[0]?dir:"—", names[to], w);
    }
    printf("\n-------------- ROUTE COMPLETE --------------\n\n");
}

/* Build direction chain string for a path (ASCII arrow '->'). Caller provides buffer */
void build_direction_chain_for_path(int path[], int n, char *outbuf, int bufsz) {
    outbuf[0] = '\0';
    if (n < 2) return;
    int pos = 0;
    for (int i = 0; i < n - 1; ++i) {
        const char *d = edge_dir(path[i], path[i+1]);
        if (!d || d[0] == '\0') d = "—";
        int needed = (int)strlen(d);
        if (i > 0) {
            /* add arrow separator */
            if (pos + 4 < bufsz) { strcat(outbuf, " -> "); pos += 4; }
            else break;
        }
        if (pos + needed + 1 < bufsz) { strcat(outbuf, d); pos += needed; }
        else break;
    }
}

/* Show Top 3 closest amenities, numbered, multi-line (distance+direction chain) */
void show_top3_closest(int src) {
    int parent[MAXV];
    char parent_dir[MAXV][DIRLEN];
    int dist_all[MAXV];
    dijkstra_all(src, parent, parent_dir, dist_all);

    typedef struct { int idx; int d; } Pair;
    Pair arr[MAXV];
    int cnt = 0;
    for (int i = 0; i < MAXV; ++i) {
        if (i == src) continue;
        if (dist_all[i] >= INF) continue;
        arr[cnt].idx = i; arr[cnt].d = dist_all[i]; cnt++;
    }
    if (cnt == 0) { printf("\n[INFO] No reachable locations from %s.\n\n", names[src]); return; }

    int topk = (cnt < 3) ? cnt : 3;
    /* selection sort topk */
    for (int i = 0; i < topk; ++i) {
        int best = i;
        for (int j = i+1; j < cnt; ++j) if (arr[j].d < arr[best].d) best = j;
        if (best != i) { Pair tmp = arr[i]; arr[i] = arr[best]; arr[best] = tmp; }
    }

    printf("\nTop %d closest locations to %s:\n", topk, names[src]);
    printf("--------------------------------------------\n");
    for (int i = 0; i < topk; ++i) {
        int idx = arr[i].idx;
        int d = arr[i].d;

        /* reconstruct path to build direction chain */
        int parent_tmp[MAXV];
        char parent_dir_tmp[MAXV][DIRLEN];
        int dist_tmp;
        dijkstra(src, idx, parent_tmp, parent_dir_tmp, &dist_tmp);
        int path[MAXV];
        int n = reconstruct_path(src, idx, parent_tmp, path);

        char chain[512];
        chain[0] = '\0';
        if (n >= 2) build_direction_chain_for_path(path, n, chain, sizeof(chain));

        printf("%d. %s\n", i+1, names[idx]);
        printf("    Distance:   %d units\n", d);
        printf("    Directions: %s\n\n", chain[0] ? chain : "—");
    }
    printf("--------------------------------------------\n\n");
}

/* Menu loop with re-ask behavior for invalid nodes */
void menu_loop(void) {
    char lookup[MAXV][NAMEBUF];
    build_name_lookup(lookup);

    char choice_line[32];
    while (1) {
        printf("1) Navigate between two locations\n");
        printf("2) Find closest amenities (Top 3)\n");
        printf("3) Show all known nodes\n");
        printf("4) Exit\n");
        printf("\nEnter your choice (1-4): ");

        if (!fgets(choice_line, sizeof(choice_line), stdin)) break;
        trim_newline_and_spaces(choice_line);
        if (strlen(choice_line) == 0) continue;

        if (strcmp(choice_line, "1") == 0) {
            int s = prompt_for_node("Enter start location (or 'exit' to return): ", lookup);
            if (s < 0) continue;
            int t = prompt_for_node("Enter destination     (or 'exit' to return): ", lookup);
            if (t < 0) continue;
            if (s == t) { printf("\n[INFO] You are already at %s.\n\n", names[s]); continue; }
            int parent[MAXV];
            char parent_dir[MAXV][DIRLEN];
            int dist_total;
            dijkstra(s, t, parent, parent_dir, &dist_total);
            print_route_pretty(s, t, parent, dist_total);
        }
        else if (strcmp(choice_line, "2") == 0) {
            int s = prompt_for_node("Enter your current location (or 'exit' to return): ", lookup);
            if (s < 0) continue;
            show_top3_closest(s);
        }
        else if (strcmp(choice_line, "3") == 0) {
            print_known_nodes_beautiful();
        }
        else if (strcmp(choice_line, "4") == 0) {
            printf("\nExiting. Goodbye!\n");
            break;
        }
        else {
            printf("\n[ERROR] Invalid menu choice. Please enter 1, 2, 3 or 4.\n\n");
            continue;
        }
    }
}

int main(void) {
    build_graph();
    print_header();
    print_known_nodes_beautiful();
    menu_loop();
    return 0;
}
