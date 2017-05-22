#include "os.h"

int main(void) {
    Job in[N];
    Job jobs[N];
    int cnt = 0;
    while (scanf("%s%d%d", in[cnt].name, &in[cnt].arrive_time, &in[cnt].serve_time) == 3) cnt++;

    copy(jobs, in, cnt);
    printf("FCFS\n");
    FCFS(jobs, cnt);
    display(jobs, cnt);

    copy(jobs, in, cnt);
    printf("RR-1\n");
    RR(jobs, cnt, 1);
    display(jobs, cnt);

    copy(jobs, in, cnt);
    printf("RR-4\n");
    RR(jobs, cnt, 4);
    display(jobs, cnt);

    copy(jobs, in, cnt);
    printf("SJF\n");
    SJF(jobs, cnt);
    display(jobs, cnt);

    copy(jobs, in, cnt);
    printf("HRN\n");
    HRN(jobs, cnt);
    display(jobs, cnt);

    return 0;
}


