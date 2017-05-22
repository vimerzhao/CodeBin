#include "os.h"

void push(heap *h, double priority, Job *job) {
    if (h->len+1 >= h->size) {
        h->size = h->size<<1;
        h->nodes =(node *)realloc(h->nodes, h->size*sizeof(node));
    }
    int i = h->len + 1;
    int j = i>>1;
    while (i > 1 && h->nodes[j].priority > priority) {
        h->nodes[i] = h->nodes[j];
        i = j;
        j >>= 1;
    }

    h->nodes[i].priority = priority;
    h->nodes[i].job = job;
    h->len++;
}

Job *pop(heap *h) {
    int i, j, k;
    if (!h->len) {
        return NULL;
    }
    Job *job = h->nodes[1].job;
    h->nodes[1] = h->nodes[h->len];
    h->len--;
    i = 1;
    while (1) {
        k = i;
        j = i<<1;
        if (j <= h->len && h->nodes[j].priority < h->nodes[k].priority) {
            k = j;
        }
        if (j+1 <= h->len && h->nodes[j+1].priority < h->nodes[k].priority) {
            k = j+1;
        }
        if (k == i) {
            break;
        }
        h->nodes[i] = h->nodes[k];
        i = k;
    }
    h->nodes[i] = h->nodes[h->len+1];
    return job;
}

void copy(Job jobs[], Job in[], int cnt) {
    for (int i = 0; i < cnt; i++) {
        strcpy(jobs[i].name, in[i].name);
        jobs[i].arrive_time = in[i].arrive_time;
        jobs[i].serve_time = in[i].serve_time;
        jobs[i].begin_time = 0;
        jobs[i].end_time= 0;
        jobs[i].progress = 0;
        jobs[i].turnover_time= 0;
        jobs[i].weight_turnover_time= 0;
    }
}

void display(Job jobs[], int cnt) {
    printf("Name\tArrive\tServe\tBegin\tEnd \tTurn\twTurn\t\n");
    for (int i = 0; i < cnt; i++) {
        printf("%-8s%-8d%-8d%-8d%-8d%-8d%-8.2f\n",
                jobs[i].name,
                jobs[i].arrive_time,
                jobs[i].serve_time,
                jobs[i].begin_time,
                jobs[i].end_time,
                jobs[i].turnover_time,
                jobs[i].weight_turnover_time);
    }
    printf("\n");
}

int cmp_fcfs(const void *a, const void *b) {
    return ((Job *)a)->arrive_time - ((Job *)b)->arrive_time;
}

void FCFS(Job jobs[], int cnt) {
    qsort(jobs, cnt, sizeof(Job), cmp_fcfs);

    int clock = 0;
    for (int i = 0; i < cnt; i++) {
        clock = clock < jobs[i].arrive_time ? jobs[i].arrive_time : clock;
        jobs[i].begin_time = clock;
        clock += jobs[i].serve_time;
        jobs[i].end_time = clock;
        jobs[i].turnover_time = jobs[i].end_time - jobs[i].arrive_time;
        jobs[i].weight_turnover_time = jobs[i].turnover_time*1.0/jobs[i].serve_time;
    }
}


void RR(Job jobs[], int cnt, int slice) {
    qsort(jobs, cnt, sizeof(Job), cmp_fcfs);

    Job *ready[N];// ready queue
    int head = 0, tail = 0;
    int n = cnt;
    int clock = 0;
    int index = 0;
    while (n) {
        if (head == tail && index < cnt) {// no process ready.
            ready[tail] = jobs+index;
            clock = jobs[index].arrive_time;
            index++;
            tail = (tail+1+N)%N;
        } else {
            int exe = 0;
            do {
                if (ready[head]->progress == 0) {
                    ready[head]->begin_time = clock;
                }

                exe++;
                clock++;
                // index < cnt is important
                while (index < cnt && jobs[index].arrive_time <= clock) {
                    ready[tail] = jobs+index;
                    index++;
                    tail = (tail+1+N)%N;
                }
                ready[head]->progress++;
                if (ready[head]->progress == ready[head]->serve_time) {
                    break;
                }
            } while (exe < slice);
            if (ready[head]->progress < ready[head]->serve_time) {
                ready[tail] = ready[head];
                tail = (tail+1+N)%N;
                head = (head+1+N)%N;
            } else {
                n--;
                ready[head]->end_time = clock;
                head = (head+1+N)%N;
            }
        }
    }
    for (int i = 0; i < cnt; i++) {
        jobs[i].turnover_time = jobs[i].end_time - jobs[i].arrive_time;
        jobs[i].weight_turnover_time = jobs[i].turnover_time*1.0/jobs[i].serve_time;
    }

}

void SJF(Job jobs[], int cnt) {
    qsort(jobs, cnt, sizeof(Job), cmp_fcfs);
    int clock = 0;
    int n = 0;//number of jobs that finished!
    int index = 0;
    heap* h = (heap *)calloc(cnt, sizeof(heap));
    h->len = 0;
    h->size = 1;

    Job *cur = NULL;
    while (1) {
        if (n == cnt) break;
        if ((cur = pop(h)) == NULL) {
            clock = jobs[index].arrive_time;
            push(h, jobs[index].serve_time, jobs+index);
            index++;
        } else {
            cur->begin_time = clock;
            clock += cur->serve_time;
            n++;

            // update arrive queue
            while (index < cnt && jobs[index].arrive_time <= clock) {
                push(h, jobs[index].serve_time, jobs+index);
                index++;
            }
        }
    }

    for (int i = 0; i < cnt; i++) {
        jobs[i].end_time = jobs[i].begin_time + jobs[i].serve_time;
        jobs[i].turnover_time = jobs[i].end_time - jobs[i].arrive_time;
        jobs[i].weight_turnover_time = jobs[i].turnover_time*1.0/jobs[i].serve_time;
    }

    // if you want to resort by execute order, code here.
}

void HRN(Job jobs[], int cnt) {
    qsort(jobs, cnt, sizeof(Job), cmp_fcfs);
    int left_time[N];
    for (int i = 0; i < cnt; i++) {
        left_time[i] = jobs[i].serve_time;
    }
    int n = cnt;
    int clock = 0;
    while (n) {
        // find highest weight
        int max = -1;
        int index = -1;
        for (int i = 0; i < cnt; i++) {
            if (jobs[i].arrive_time <= clock && left_time[i]) {
                double weight = 1 + (clock-jobs[i].arrive_time)*1.0/jobs[i].serve_time;
                if (weight > max) {
                    max = weight;
                    index = i;
                }
            }
        }
        if (index == -1) {
            clock++;
        } else {
            jobs[index].begin_time = clock;
            clock += jobs[index].serve_time;
            left_time[index] = 0;
            n--;
        }
    }
    for (int i = 0; i < cnt; i++) {
        jobs[i].end_time = jobs[i].begin_time + jobs[i].serve_time;
        jobs[i].turnover_time = jobs[i].end_time - jobs[i].arrive_time;
        jobs[i].weight_turnover_time = jobs[i].turnover_time*1.0/jobs[i].serve_time;
    }
}
