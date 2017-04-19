# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <stdbool.h>
# include <windows.h>

# define N0 1000    //初始时刻种群数量为1000
# define Nmax 100000    //种群最大数量为100000
# define R 6    //最低繁殖年龄为8
# define B 32   //老死年龄即位串长度为32
# define b 1    //每次繁殖子代个数为1
# define T 2    //最大疾病次数为2
# define M 2    //突变强度为2
# define LEN sizeof(GENE)   //宏替换一个结构体的大小
clock_t start, finish;
double duration;
int t;
typedef struct Gene
{
    bool Gene[B];   //用布尔类型数组表示位串
    int Age;    //年龄
    int SickNum;    //患病次数
    struct Gene *next;  //存放一个连接链表的节点指针
}GENE;
int Nt = N0;    //定义一个全局变量，表示t时刻种群的数量
double M_rate = (double)M/B;    //M_rate表示基因遗传突变率
GENE *pHead = NULL, *pFore = NULL, *pNext = NULL, *p = NULL, *pTail = NULL;
//定义五个指针：pHead指向头节点pFore为节点的前驱指针，pNext建立新节点，p游标指针，pTail尾节点指针
GENE Gene_Str;  //定义一个GENE类型的结构体Gene_Str

void Initialize(void)   //构造链表及种群初始状态
{
    pHead = (GENE *)malloc(LEN);//为头节点指针分配空间
    p = pHead;  //头节点指针赋值给游标指针
    for (int i=1; i<N0; i++)
    {
        p->Age = 0; //初始时刻年龄为0（t=0）
        p->SickNum = 0; //患病次数为0
        for (int j=0; j<B; j++)
        {
            p->Gene[j] = (rand()%2);    //  对第一代个体基因随机规定
        }
        pNext = (GENE *)malloc(LEN);    //为新节点开辟存储空间
        p->next = pNext;    //将前驱节点指向下一结构体地址
        p = pNext;  //将新节点赋值给游标指针
    }
    pTail = p;
    pNext = p = NULL;
    pTail->next = NULL; //尾节点下一指向为空
}
void Aging(void)    //执行年龄增长机制，每次调用，所有个体年龄加一
{
    p = pHead;  //游标指针指向头指针
    while (p != NULL)   //遍历每个个体
    {
        if (p->Gene[p->Age] == 1)
        {
            p->SickNum++;
        }   //如果该年龄位置为坏基因，则患病次数加一
        p->Age++;   //每个个体年龄加一
        p = p->next;    //游标指针指向下一个体
    }
}
void Die(void)  //执行死亡机制，每次调用，满足条件的个体死去，链表节点删除
{
    pFore = p = pHead;  //游标指针指向头指针，并创建一个和游标指针同步的前驱指针
    while (p != NULL)   //遍历每个个体
    {
        double RandOfLive = (double)rand()/(double)RAND_MAX;    //构造一个随机概率
        if (p->SickNum>=T || p->Age>=(B-1) || RandOfLive>=(1-(double)Nt/Nmax))
        //满足三种情况之一即死亡：年龄到达老死年龄，达到最大患病次数，因环境压力随机死亡
        {
            Nt--;   //每次死亡导致种群数量减一          
            if (p == pHead)//当p为头节点
            {
                pHead = p->next;
                free(p);    //节点删除后释放占用的空间
                pFore = p = pHead;
                p = p->next;    //重新初始化
            }
            else if (p->next == NULL)//当p为尾节点
            {
                pFore->next = NULL; //将前一个节点设置为尾节点
                free(p);    //释放删除后的节点
                p = NULL;
            }
            else    //既不是头节点又不是尾节点
            {
                pFore->next = p->next;  //跳过p节点，前后相连接
                free(p);    //释放删除了的节点
                p = pFore;
                pFore = p;
                p = p->next;    //重新初始化
            }
        }
        else    //如果个体不满足死亡条件
        {
            pFore = p;
            p = p->next;    //指针向下一节点移动并始终存储前一节点以备删除中间节点
        }
    }   //结束后p == NULL,是尾节点的节点指针
    pNext = NULL;
    pFore = NULL;   //遍历之后将这两个指针置为空，防止错误调用
}
void RePreduction(void) //执行繁殖机制
{
    p = pHead;  //p指向链表的头指针
    while (p != NULL)   //遍历每个个体
    {
        double RandOfChange = (double)rand()/(double)RAND_MAX;  //构造一个随机数
        if (p->Age >= R)    //如果年龄达到最低繁殖年龄
        {
            pNext = p;  //为了方便操作，将每一个新生个体对应的节点连在尾节点后
            while (pNext->next != NULL)
                pNext = pNext->next;    //此时pNext为尾节点
            
            for (int k=0; k<b; k++) //每次繁殖将产生b个新个体
            {
                while (pNext->next != NULL)
                    pNext = pNext->next;
                
                pTail = (GENE *)malloc(LEN);    //开辟一个存储空间
                pNext->next = pTail;    //尾节点的next指向该存储空间
                pTail->next = NULL; //将这个新节点设为尾节点         
                for (int i=0; i<B; i++)
                {
                    if (RandOfChange < M_rate)  //达到要求则发生突变
                        pTail->Gene[i] = !p->Gene[i];
                    else    //否则执行拷贝复制
                        pTail->Gene[i] = p->Gene[i];
                }
                pTail->Age = 0;
                pTail->SickNum = 0; //新生个体的这两项也要初始化，开始一直忘了！！！！！！！！
                Nt+=1;  //个体数加一
            }
            p = p->next;
        }
        else
            p = p->next;    //执行完继续下一个体
    }
}

int main(void)
{
    FILE* fp = fopen("result.txt", "w");
    if (fp == NULL)
        printf("error\n");
    Sleep(2000);    //挂起两秒钟
    srand((unsigned)time(NULL));    //随机种子
    Initialize();   //对初始个体进行初始化操作
    for (t=0; t<1000; t++)  //种群在该机制下演化一千个时间步
    {
        Aging();    //执行年龄增长机制
        Die();  //执行死亡机制
        RePreduction(); //执行繁殖机制
        printf("A");
        fprintf(fp, "%d\t", Nt);
    }
    fclose(fp);
    return 0;
}
