#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#define N 50
#define SSTF 1
#define SCAN 2

// for SCAN algorithm,1 out && -1 inner
int dir = 1;
int sum = 0;

void run(const int queue[],
        int order[],
        int begin,
        int cnt,
        int (* next)(const int queue[], const int order[], int cur, int cnt));
void display(const int queue[], const int order[], int begin, int cnt);
int SSTFnext(const int queue[], const int order[], int cur, int cnt);
int SCANnext(const int queue[], const int order[], int cur, int cnt);

int main(int argc, char const* argv[]) {
    int queue[N];
    int order[N];
    int begin;
    int cnt;
    while (scanf("%d%d", &begin, &cnt) == 2) {
        for (int i = 0; i < cnt; i++) {
            scanf("%d", queue+i);
        }
        printf("SSTF:\n");
        run(queue, order, begin, cnt, SSTFnext);
        display(queue, order, begin, cnt);

        dir = 1;
        printf("\nSCAN(in-->out):\n");
        run(queue, order, begin, cnt, SCANnext);
        display(queue, order, begin, cnt);

        dir = -1;
        printf("\nSCAN(out-->in):\n");
        run(queue, order, begin, cnt, SCANnext);
        display(queue, order, begin, cnt);

        printf("\n");
    }
    return 0;
}

int cmp(const void *a, const void *b) {
    return *((int *)a) - *((int *)b);
}

int SSTFnext(const int queue[], const int order[], int cur, int cnt) {
    // binary search if data is large
    int r[N];
    for (int i = 0; i < cnt; i++) {
        r[i] = cur - queue[i];
        r[i] = r[i] < 0 ? -r[i] : r[i];
    }
    int min = INT_MAX;
    int index = 0;
    for (int i = 0; i < cnt; i++) {
        if (r[i] < min && !order[i]) {
            min = r[i];
            index = i;
        }
    }
    return index;
}

int SCANnext(const int queue[], const int order[], int cur, int cnt) {
    int r[N];
    for (int i = 0; i < cnt; i++) {
        r[i] = cur - queue[i];
        r[i] = r[i] < 0 ? -r[i] : r[i];
    }
    int min = INT_MAX;
    int index = 0;
    for (int i = 0; i < cnt; i++) {
        if (r[i] < min && !order[i]) {
            if (dir > 0 && cur <= queue[i]) {
                min = r[i];
                index = i;
            }
            if (dir < 0 && cur >= queue[i]) {
                min = r[i];
                index = i;
            }
        }
    }
    if (min == INT_MAX) {
        for (int i = 0; i < cnt; i++) {
            if (r[i] < min && !order[i]) {
                min = r[i];
                index = i;
            }
        }
    }
    return index;
}

void display(const int queue[], const int order[], int begin, int cnt) {
    int dis[N];
    dis[0]= begin;
    for (int i = 1; i <= cnt; i++) {
        dis[order[i-1]] = queue[i-1];
    }

    printf("%-8d%-8d%-8d\n", 0, begin, 0);
    for (int i = 1; i <= cnt; i++) {
        printf("%-8d%-8d%-8d\n", i, dis[i], (int)abs(dis[i]-dis[i-1]));
    }
    printf("移臂总量:\033[;32m%d\033[0m,平均寻道长度:\033[;32m%.2f\033[0m\n", sum, sum*1.0/cnt);
}

void run(const int queue[],
        int order[],
        int begin,
        int cnt,
        int (* next)(const int queue[], const int order[], int cur, int cnt)) {
    memset(order, 0, N*sizeof(int));
    int n = cnt;
    int cur = begin;
    sum = 0;
    while (n) {
        int index = next(queue, order, cur, cnt);
        order[index] = cnt-n+1;
        sum += (int)abs(cur - queue[index]);
        cur = queue[index];
        n--;
    }
}

