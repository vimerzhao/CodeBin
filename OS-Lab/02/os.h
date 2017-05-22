#ifndef OS_H_
#define OS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 50

typedef struct {
    char name[20];
    int arrive_time;
    int serve_time;
    int begin_time;
    int end_time;
    int progress;
    int turnover_time;
    double weight_turnover_time;
}Job;

void copy(Job jobs[], Job in[], int cnt);
void display(Job jobs[], int cnt);
void FCFS(Job jobs[], int cnt);
void RR(Job jobs[], int cnt, int slice);
void SJF(Job jobs[], int cnt);
void HRN(Job jobs[], int cnt);


/* priority queue | min heap */
typedef struct {
    Job *job;
    double priority;
}node;
typedef struct {
    node *nodes;
    int len;
    int size;
}heap;

void push(heap *h, double priority, Job *job);
Job *pop(heap *h);

#endif

