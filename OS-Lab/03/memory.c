#include <stdio.h>
#include <string.h>
#define N 50
#define M 50

void display(const int in[], const int res[][N], int n, int m);
void FIFO(const int in[], int n, int m);
void LRU(int in[], int n, int m);

int main(int argc, char const* argv[]) {
    int m, n;
    int in[N];
    scanf("%d%d", &m, &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", in+i);
    }

    printf("M=3\n");
    FIFO(in, n, 3);
    printf("\nM=4\n");
    FIFO(in, n, 4);

    printf("\nM=3\n");
    LRU(in, n, 3);
    printf("\nM=4\n");
    LRU(in, n, 4);
    return 0;
}

void display(const int in[], const int res[][N], int n, int m) {
    int cnt = 0;
    printf("页号\t");
    for (int j = 0; j < n; j++) {
        printf("%-4d", in[j]);
    }
    printf("\n");

    printf("缺页?\t");
    for (int j = 0; j < n; j++) {
        if (res[0][j] == 0) {
            printf("    ");
        } else {
            cnt++;
            printf("\033[;31m√\033[0m   ");
        }
    }

    printf("\n");
    for (int i = 1; i <= m; i++) {
        printf("Time%-4d", m+1-i);
        for (int j = 0; j < n; j++) {
            if (res[i][j] == 0) {
                printf("    ");
            } else {
                printf("%-4d", res[i][j]);
            }
        }
        printf("\n");
    }

    printf("被换出:\t");
    for (int j = 0; j < n; j++) {
        if (res[0][j] <= 0) {
            printf("    ");
        } else {
            printf("%-4d", res[0][j]);
        }
    }
    printf("\n");
    printf("缺页次数:\033[;32m%d\033[0m,缺页率:\033[;32m%.2f\033[0m\n", cnt, cnt*1.0/n);

}

void FIFO(const int in[], int n, int m) {
    int res[M][N];
    memset(res, 0, M*N*sizeof(int));

    // initial
    res[m][0] = in[0];
    res[0][0] = -1;

    for (int j = 1; j < n; j++) {
        int find = 0;
        for (int i = 1; i <= m;  i++) {
            if (res[i][j-1] == in[j]) {
                find = 1;
                break;
            }
        }
        if (find) {
            for (int i = 1; i <= m; i++) {
                res[i][j] = res[i][j-1];
            }
        } else {
            for (int i = 0; i < m; i++) {
                res[i][j] = res[i+1][j-1];
            }
            res[m][j] = in[j];
            if (res[0][j] == 0) {
                res[0][j] = -1;// not change out, but still not find!
            }
        }
    }
    display(in, res, n, m);
}

void LRU(int in[], int n, int m) {
    int res[M][N];
    memset(res, 0, M*N*sizeof(int));

    // initial
    res[m][0] = in[0];
    res[0][0] = -1;

    for (int j = 1; j < n; j++) {
        int find = 0;
        for (int i = 1; i <= m;  i++) {
            if (res[i][j-1] == in[j]) {
                find = i;
                break;
            }
        }
        for (int i = 1; i < find; i++) {
            res[i][j] = res[i][j-1];
        }
        for (int i = find; i < m; i++) {
            res[i][j] = res[i+1][j-1];
        }
        res[m][j] = in[j];
        if (res[0][j] == 0) {
            res[0][j] = -1;// not change out, but still not find!
        }
        if (find) {
            res[0][j] = 0;
        }
    }
    display(in, res, n, m);
}

