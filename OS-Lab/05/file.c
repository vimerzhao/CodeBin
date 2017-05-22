#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N 500//number of block
#define SIZE 2//size of a block
#define FILE_NUM 50
#define LEN 80// >= floor(N/8.0);
#define C_SIZE (8*sizeof(char))
typedef struct {
    char name[20];
    int id;
    int start;
    int len;
}File;


int getLoc(unsigned char bit[], int len) {
    int start;
    for (int i = 1; i <= N; i++) {
        start = i;
        int l = 0;
        for (;l < (len+1)/2; l++) {
            if (bit[(start+l-1)/C_SIZE] & (0x80u>>((start+l-1)%C_SIZE))) {
                break;
            }
        }
        if (start + l > N) {
            printf("overflow\n");
        }
        if (l == (len+1)/2) {
            for (int j = 0; j < l; j++) {
                bit[(start+j-1)/C_SIZE] = bit[(start+j-1)/C_SIZE] | (0x80u >> ((start+j-1)%C_SIZE));
            }
            return start;
        }
    }
}
void showdisk(unsigned char bit[]) {
    int line = 100;
    for (int i = 1; i <= N; i++) {
        if (bit[(i-1)/C_SIZE] & (0x80u>>((i-1)%C_SIZE))) {
            printf("\033[;41m*\033[0m");
        } else {
            printf("\033[;42m+\033[0m");
        }
        if (!(i%100)) printf("\n");
    }
    printf("\n");
}

void removefile(unsigned char bit[], int start, int len) {
    for (int j = 0; j < (len+1)/2; j++) {
        bit[(start+j-1)/C_SIZE] = bit[(start+j-1)/C_SIZE] & (0xFF7Fu >> ((start+j-1)%C_SIZE));
    }
}


int main(int argc, char const* argv[]) {
    unsigned char bit[LEN];
    memset(bit, 0, LEN*sizeof(char));
    showdisk(bit);

    File file[FILE_NUM+5];
    srand((unsigned)time(NULL));
    printf("Create File\n");
    for (int i = 1; i <= FILE_NUM; i++) {
        file[i].id = i;
        sprintf(file[i].name, "%d.txt", i);
        file[i].len = rand()%9+2;
        file[i].start = getLoc(bit, file[i].len);
        printf("file id:\033[;32m%4d\033[0m, file start:\033[;32m%4d\033[0m, file len:\033[;32m%4d\033[0m\n", i, file[i].start, file[i].len);
    }
    showdisk(bit);

    printf("delete odd file\n");
    for (int i = 1; i <= FILE_NUM; i++) {
        if (i%2) {
            removefile(bit, file[i].start, file[i].len);
        }
    }
    showdisk(bit);


    printf("insert new file\n");
    File A = {"A.txt", 0, 0, 7};
    File B = {"B.txt", 0, 0, 5};
    File C = {"C.txt", 0, 0, 2};
    File D = {"D.txt", 0, 0, 9};
    File E = {"E.txt", 0, 0, 40};
    A.start = getLoc(bit, A.len);
    B.start = getLoc(bit, B.len);
    C.start = getLoc(bit, C.len);
    D.start = getLoc(bit, D.len);
    E.start = getLoc(bit, E.len);
    printf("A.txt, start: %4d, len: %4d\n", A.start, A.len);
    printf("B.txt, start: %4d, len: %4d\n", B.start, B.len);
    printf("C.txt, start: %4d, len: %4d\n", C.start, C.len);
    printf("D.txt, start: %4d, len: %4d\n", D.start, D.len);
    printf("E.txt, start: %4d, len: %4d\n", E.start, E.len);
    showdisk(bit);
    return 0;
}
