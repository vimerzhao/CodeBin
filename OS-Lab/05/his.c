#include <stdio.h>
#include <stdlib.h>
#define BLOCKN 500
#define FILEN 500
#define FMAXBLOCK 5
#include <math.h>
#include <string.h>
#include <time.h>

typedef enum bool {false, true} bool;

typedef
struct File {
    int block[FMAXBLOCK];
    int blockn;
    char fileName[1<<5];
} File;

bool used[BLOCKN];
File files[FILEN];
int filen;

void init() {
    filen = 0;
    memset(used, 0, sizeof(used));
}

void create(const char* filename, float size) { // size k
    // 2k perblock
    int need = (int)ceil(size/2.0);
    strcpy(files[filen].fileName, filename);
    int i, cur =0;
    for (i=0; i<BLOCKN; i++) {
        if (!used[i]) {
            files[filen].block[cur++] = i;
            used[i] = true;
            if (cur == need) break;
        }
    }
    if (cur != need) {
        printf("Error, no room for a new file.\n");
        return;
    }
    files[filen].blockn = need;
    filen = filen + 1;
}

bool rm(const char* filename) {
    int i;
    for (i=0; i<filen; i++) {
        if (!strcmp(files[i].fileName, filename)) {
            int j;
            for (j=0; j<files[i].blockn; j++) {
                used[files[i].block[j]] = false;
            }
            // remove the instance from array
            memmove((void*)(files + i), (void*)(files + i + 1), sizeof(File) * (filen-i-1));
            filen -= 1;
            return true;
        }
    }
    return false;
}

float randSize(float b, float e) {
    #define N 999
    srand((unsigned)time(NULL));
    double real = rand()%(N+1) * 1.0/(float)(N+1);
    return b + (e - b) * real;
}

void showFileInfo(const char* filename) {
    int i, found = 0;
    for (i=0; i<filen; i++) {
        if (!strcmp(files[i].fileName, filename)) {
            int j;
            printf("%s blocknumbers : \n", filename);
            for (j=0; j<files[i].blockn; j++) {
                printf("%d ", files[i].block[j]);
            }printf("\n");

            found = 1;
        }
    }
    if (!found) {
        printf("file not found!\n");
    }
}

void showAllInfo() {
    int i;
    printf("all infomations:\n");
    for (i=0; i<BLOCKN; i++) {
        printf("%c", (used[i]? '*' : '_'));
    }
    printf("\n");
}


int main() {

    // create 1.txt, 2.txt, .... 50.txt
    char tmpName[1<<5];
    int i;
    for (i=1; i<=50; i++) {
        tmpName[0] = i + 48;
        strcpy(tmpName+1, ".txt");
        create(tmpName, randSize(2.0, 10.0));
    }

    for (i=1; i<=50; i++) {
        if (i%2 == 0) continue;
        tmpName[0] = i + 48;
        strcpy(tmpName+1, ".txt");
        rm(tmpName);
    }

    char c;
    for (c='A'; c<='E'; c++) {
        tmpName[0] = c;
        strcpy(tmpName+1, ".txt");
        create(tmpName, randSize(2.0, 10.0));
        showFileInfo(tmpName);
    }
    showAllInfo();
    return 0;
}
