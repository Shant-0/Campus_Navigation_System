#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAXV 17
#define INF  (INT_MAX/6)
#define DIRLEN 24
#define NAMEBUF 64

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

enum {
    V_PUNCH,
    V_J01,
    V_J02,
    V_J03,
    V_J04,
    V_GND,
    V_WIFI,
    V_LG,
    V_MBA,
    V_C107,
    V_C112,
    V_PIGE,
    V_PRAY,
    V_JOURNAL,
    V_AUDIT,
    V_STCAFE,
    V_GATE
};

typedef struct Edge {
    int to;
    int weight;
    char direction[DIRLEN];
    struct Edge *next;
} Edge;

Edge *adj[MAXV] = { NULL };

void print_header() {
    printf("=================================================\n");
    printf("              CAMPUS NAVIGATION SYSTEM           \n");
    printf("=================================================\n\n");
}

void print_nodes(){
    printf("\n--------------------------------------------\n");
    printf("Known locations:\n");
    printf("--------------------------------------------\n");
    for (int i = 0; i < MAXV; ++i)
        printf("  • %s\n", names[i]);
    printf("--------------------------------------------\n\n");
}

void add_edge_oneway(int from, int to, int w, const char *dir) {
    Edge *e = malloc(sizeof(Edge));
    if (!e) { perror("malloc"); exit(1); }

    e->to = to;
    e->weight = w;
    strncpy(e->direction, dir, DIRLEN - 1);
    e->direction[DIRLEN - 1] = '\0';

    e->next = adj[from];
    adj[from] = e;  
}

const char *opposite_direction(const char *d) {
    if (strcmp(d, "North") == 0) return "South";
    if (strcmp(d, "South") == 0) return "North";
    if (strcmp(d, "East") == 0) return "West";
    if (strcmp(d, "West") == 0) return "East";
    if (strcmp(d, "North-East") == 0) return "South-West";
    if (strcmp(d, "South-West") == 0) return "North-East";
    if (strcmp(d, "South-East") == 0) return "North-West";
    if (strcmp(d, "North-West") == 0) return "South-East";
    return d;
}

void add_edge(int from, int to, int w, const char *dir) {
    add_edge_oneway(from, to, w, dir);
    add_edge_oneway(to, from, w, opposite_direction(dir));
}

void build_graph() {
    for (int i = 0; i < MAXV; ++i)
        adj[i] = NULL;

    add_edge(V_PUNCH, V_J01, 35, "North");

    add_edge(V_J01, V_AUDIT, 55, "West");
    add_edge(V_J01, V_GND, 45, "North-West");  
    add_edge(V_J01, V_J04, 70, "North");
    add_edge(V_J01, V_J02, 115, "West");
    add_edge(V_J01, V_STCAFE, 85, "West");

    add_edge(V_PRAY, V_J04, 15, "West");

    add_edge(V_J04, V_PIGE, 20, "West");
    add_edge(V_J04, V_GND, 45, "South-West");  
    add_edge(V_PIGE, V_C107, 25, "West");

    add_edge(V_C107, V_GATE, 15, "West");

    add_edge(V_GND, V_GATE, 35, "North");
    add_edge(V_GND, V_AUDIT, 35, "South");
    add_edge(V_GND, V_WIFI, 20, "West");
    add_edge(V_GND, V_STCAFE, 45, "South-West");

    add_edge(V_GATE, V_C112, 15, "West");

    add_edge(V_C112, V_J03, 10, "West");

    add_edge(V_J03, V_WIFI, 45, "South");
    add_edge(V_J03, V_MBA, 15, "West");
    add_edge(V_J03, V_LG, 45, "South-West");
    add_edge(V_J03, V_STCAFE, 70, "South");

    add_edge(V_J02, V_STCAFE, 30, "East");
    add_edge(V_J02, V_AUDIT, 60, "East");

    add_edge(V_LG, V_J02, 30, "South");
    add_edge(V_LG, V_JOURNAL, 45, "South");

    add_edge(V_J02, V_JOURNAL, 15, "South");
}

void normalize_input(const char *raw, char *out) {
    int j = 0;
    for (int i = 0; raw[i]; ++i)
        if (!isspace((unsigned char)raw[i]))
            out[j++] = tolower((unsigned char)raw[i]);
    out[j] = '\0';
}

void name_lookup(char table[MAXV][NAMEBUF]) {
    for (int i = 0; i < MAXV; ++i) {
        char buf[NAMEBUF];
        int j = 0;
        for (int k = 0; names[i][k]; ++k)
            if (!isspace((unsigned char)names[i][k]))
                buf[j++] = tolower((unsigned char)names[i][k]);
        buf[j] = '\0';
        strcpy(table[i], buf);
    }
}

int index_of_normalized(const char *raw, char lookup[MAXV][NAMEBUF]) {
    char norm[NAMEBUF];
    normalize_input(raw, norm);
    for (int i = 0; i < MAXV; ++i)
        if (strcmp(norm, lookup[i]) == 0)
            return i;
    return -1;
}

void trim_newline_spaces(char *s) {
    int L = strlen(s);
    while (L > 0 && (s[L-1] == '\n' || s[L-1] == '\r'))
        s[--L] = '\0';

    int st = 0;
    while (isspace((unsigned char)s[st]))
        st++;

    if (st) memmove(s, s+st, strlen(s+st)+1);

    L = strlen(s);
    while (L > 0 && isspace((unsigned char)s[L-1]))
        s[--L] = '\0';
}

int read_trimmed_line(char *buf, int size) {
    if (!fgets(buf, size, stdin))
        return 0;
    trim_newline_spaces(buf);
    return 1;
}

int prompt_for_node(const char *prompt, char lookup[MAXV][NAMEBUF]) {
    char line[128];
    while (1) {
        printf("%s", prompt);

        if (!read_trimmed_line(line, sizeof(line)))
            return -1;

        if (strlen(line) == 0)
            continue;

        if (!strcasecmp(line, "exit"))
            return -1;

        int idx = index_of_normalized(line, lookup);
        if (idx >= 0)
            return idx;

        printf("\n[ERROR] Unknown location: '%s'\n", line);
        print_nodes();
    }
}

void dijkstra(int src, int dst, int parent[], char pdir[][DIRLEN], int *outdist) {
    int dist[MAXV], used[MAXV];

    for (int i = 0; i < MAXV; ++i) {
        dist[i] = INF;
        used[i] = 0;
        parent[i] = -1;
        pdir[i][0] = '\0';
    }

    dist[src] = 0;

    for (int iter = 0; iter < MAXV; ++iter) {
        int v = -1;
        for (int i = 0; i < MAXV; ++i)
            if (!used[i] && (v == -1 || dist[i] < dist[v]))
                v = i;

        if (v == -1 || dist[v] == INF)
            break;

        used[v] = 1;
        if (v == dst)
            break;

        for (Edge *e = adj[v]; e; e = e->next) {
            int to = e->to;
            if (dist[v] + e->weight < dist[to]) {
                dist[to] = dist[v] + e->weight;
                parent[to] = v;
                strcpy(pdir[to], e->direction);
            }
        }
    }

    *outdist = dist[dst];
}

void dijkstra_all(int src, int parent[], char pdir[][DIRLEN], int outdist[]) {
    int dist[MAXV], used[MAXV];

    for (int i = 0; i < MAXV; ++i) {
        dist[i] = INF;
        used[i] = 0;
        parent[i] = -1;
        pdir[i][0] = '\0';
    }

    dist[src] = 0;

    for (int iter = 0; iter < MAXV; ++iter) {
        int v = -1;
        for (int i = 0; i < MAXV; ++i)
            if (!used[i] && (v == -1 || dist[i] < dist[v]))
                v = i;

        if (v == -1 || dist[v] == INF)
            break;

        used[v] = 1;

        for (Edge *e = adj[v]; e; e = e->next)
            if (dist[v] + e->weight < dist[e->to]) {
                dist[e->to] = dist[v] + e->weight;
                parent[e->to] = v;
                strcpy(pdir[e->to], e->direction);
            }
    }

    for (int i = 0; i < MAXV; ++i)
        outdist[i] = dist[i];
}

int reconstruct_path(int src, int dst, int parent[], int out[]) {
    int stack[MAXV], top = 0;
    int cur = dst;

    while (cur != -1) {
        stack[top++] = cur;
        if (cur == src)
            break;
        cur = parent[cur];
    }

    if (top == 0 || stack[top-1] != src)
        return 0;

    for (int i = 0; i < top; ++i)
        out[i] = stack[top-1-i];

    return top;
}


const char* edge_dir(int from, int to) {
    for (Edge *e = adj[from]; e; e = e->next)
        if (e->to == to)
        return e->direction;
    return "";
}

void direction_chain(int path[], int n, char *outbuf, int bufsz) {
    outbuf[0] = '\0';
    int pos = 0;

    for (int i = 0; i < n - 1; ++i) {
        const char *d = edge_dir(path[i], path[i+1]);
        if (!d || !*d)
            d = "—";

        int need = strlen(d);

        if (i > 0) {
            if (pos + 4 >= bufsz)
                break;
            strcat(outbuf, " -> ");
            pos += 4;
        }

        if (pos + need + 1 >= bufsz)
            break;

        strcat(outbuf, d);
        pos += need;
    }
}

void print_route(int src, int dst, int parent[], int dist_total) {
    if (dist_total >= INF) {
        printf("\n[ERROR] No possible path between %s and %s.\n\n",
               names[src], names[dst]);
        return;
    }

    int path[MAXV];
    int n = reconstruct_path(src, dst, parent, path);

    if (n == 0) {
        printf("\n[ERROR] No path found.\n\n");
        return;
    }

    printf("\nTotal distance: %d units\n\n", dist_total);
    printf("Step-by-step navigation:\n");

    for (int i = 0; i < n - 1; ++i) {
        int from = path[i], to = path[i+1];
        const char *dir = edge_dir(from, to);
        int w = -1;

        for (Edge *e = adj[from]; e; e = e->next)
            if (e->to == to) { w = e->weight; break; }

        printf("  → From %-15s go %-13s to %-15s (%d)\n",
               names[from], dir, names[to], w);
    }

    printf("\n-------------- ROUTE COMPLETE --------------\n\n");
}

void show_closest(int src) {
    int parent[MAXV];
    char pdir[MAXV][DIRLEN];
    int dist[MAXV];

    dijkstra_all(src, parent, pdir, dist);

    typedef struct { 
        int idx, d; 
    } Pair;

    Pair arr[MAXV];

    int cnt = 0;

    for (int i = 0; i < MAXV; ++i)
        if (i != src && dist[i] < INF)
        arr[cnt++] = (Pair){i, dist[i]};

    int topk = (cnt < 3 ? cnt : 3);

    for (int i = 0; i < topk; ++i) {
        int best = i;
        for (int j = i + 1; j < cnt; ++j)
            if (arr[j].d < arr[best].d)
                best = j;
        Pair tmp = arr[i]; 
        arr[i] = arr[best]; 
        arr[best] = tmp;
    }

    printf("\nTop %d closest locations to %s:\n", topk, names[src]);

    for (int i = 0; i < topk; ++i) {
        int node = arr[i].idx;
        int d    = arr[i].d;

        int parent2[MAXV];
        char pdir2[MAXV][DIRLEN];
        int dist2;
        dijkstra(src, node, parent2, pdir2, &dist2);

        int path[MAXV];
        int n = reconstruct_path(src, node, parent2, path);

        char chain[300];
        chain[0] = '\0';
        direction_chain(path, n, chain, sizeof(chain));

        printf("%d. %s\n", i+1, names[node]);
        printf("    Distance:   %d units\n", d);
        printf("    Directions: %s\n\n", chain);
    }

    printf("--------------------------------------------\n\n");
}

void menu_loop(){
    char lookup[MAXV][NAMEBUF];
    name_lookup(lookup);

    while (1) {
        char line[16];

        printf("Menu:\n");
        printf("  1. Show known locations\n");
        printf("  2. Find route between two locations\n");
        printf("  3. Show 3 closest locations\n");
        printf("  4. Exit\n");
        printf("Enter choice (1-4): ");

        if (!read_trimmed_line(line, sizeof(line))) break;

        if (!strcmp(line, "1")) {
            print_nodes();
        }
        else if (!strcmp(line, "2")) {
            int src = prompt_for_node("Enter source: ", lookup);
            if (src < 0) 
            break;

            int dst = prompt_for_node("Enter destination: ", lookup);
            if (dst < 0) 
            break;

            int parent[MAXV];
            char pdir[MAXV][DIRLEN];
            int dist;

            dijkstra(src, dst, parent, pdir, &dist);
            print_route(src, dst, parent, dist);
        }
        else if (!strcmp(line, "3")) {
            int src = prompt_for_node("Enter location: ", lookup);
            if (src < 0) break;
            show_closest(src);
        }
        else if (!strcmp(line, "4")) {
            printf("\nGoodbye!\n");
            break;
        }
        else {
            printf("\n[ERROR] Invalid choice.\n\n");
        }
    }
}

int main() {
    build_graph();
    print_header();
    print_nodes();
    menu_loop();
    return 0;
}