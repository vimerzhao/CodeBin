#include <cstdio>
//利用深度优先搜索
//构造OPEN表和CLOSE表
typedef struct {
    int State[5][5];
    int FatherState[5][5];
}OPEN;
typedef struct {
    int Order;
    int State[5][5];
    int FatherState[5][5];
}CLOSE;
OPEN OpenList[500000];
CLOSE CloseList[500000];
int OpenCount = -1, CloseCount = -1;
//判断位置是否合法的辅助数组
int Position[5][5] = {
    {-1, -1, -1, -1, -1},
    {-1,  1,  1,  1, -1},
    {-1,  1,  1,  1, -1},
    {-1,  1,  1,  1, -1},
    {-1, -1, -1, -1, -1}
};
//将某一状态存入OPEN表
void Push(int a[][5], int b[][5]){
    OpenCount++;
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            OpenList[OpenCount].State[i][j] = a[i][j];
            OpenList[OpenCount].FatherState[i][j] = b[i][j];
        }
    }
}
//比较连个状态是否完全相同
int Compare(int a[][5], int b[][5]){
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            if (a[i][j] != b[i][j]){
                return 0;
            }
        }
    } 
    return 1;
}

void Move(int a[][5]){
    int temp[5][5];
    int i, j, m, n, flag1 = 0, flag2;
    //找到0的位置
    for (i = 0; i < 5; i++){
        for (j = 0; j < 5; j++){
            if (0 == a[i][j]){
                flag1 = 1;
                break;
            }
        } 
        if (flag1){
            break;
        }
    }
    //复制数组
    for (m = 0; m < 5; m++){
        for (n = 0; n < 5; n++){
            temp[m][n] = a[m][n];
        }
    }
    if (-1 != Position[i][j-1]){                 //方向 1
        a[i][j] = a[i][j-1];
        a[i][j-1] = 0;                           //移动数字
        flag2 = 0;
        for (int k = 0; k <= OpenCount; k++){	 //检查当前状态是否在OPEN表中
            if (Compare(OpenList[k].State, a)){
                flag2 = 1;
                break;
            }
        }
        if (!flag2){
            for (int k = 0; k <= CloseCount; k++){//检查当前状态是否在CLOSE表中
                if (Compare(CloseList[k].State, a)){
                    flag2 = 1;
                    break;
                }
            }
        }
        if (!flag2){					           	//若果都不在就加入OPEN表
            Push(a, temp);
        }
        for (m = 0; m < 5; m++){				    //复原数组a,做下一移动方向的判断
            for (n = 0; n < 5; n++){
                a[m][n] = temp[m][n];
            }
        }
    }


    if (-1 != Position[i][j+1]){                 //方向 2
        a[i][j] = a[i][j+1];
        a[i][j+1] = 0;                           //移动数字
        flag2 = 0;
        for (int k = 0; k <= OpenCount; k++){
            if (Compare(OpenList[k].State, a)){
                flag2 = 1;
                break;
            }
        }
        if (!flag2){
            for (int k = 0; k <= CloseCount; k++){
                if (Compare(CloseList[k].State, a)){
                    flag2 = 1;
                    break;
                }
            }
        }
        if (!flag2){
            Push(a, temp);
        }
        for (m = 0; m < 5; m++){
            for (n = 0; n < 5; n++){
                a[m][n] = temp[m][n];
            }
        }
    }

    if (-1 != Position[i+1][j]){                 //方向 3
        a[i][j] = a[i+1][j];
        a[i+1][j] = 0;                           //移动数字
        flag2 = 0;
        for (int k = 0; k <= OpenCount; k++){
            if (Compare(OpenList[k].State, a)){
                flag2 = 1;
                break;
            }
        }
        if (!flag2){
            for (int k = 0; k <= CloseCount; k++){
                if (Compare(CloseList[k].State, a)){
                    flag2 = 1;
                    break;
                }
            }
        }
        if (!flag2){
            Push(a, temp);
        }
        for (m = 0; m < 5; m++){
            for (n = 0; n < 5; n++){
                a[m][n] = temp[m][n];
            }
        }
    }

    if (-1 != Position[i-1][j]){                 //方向 4
        a[i][j] = a[i-1][j];
        a[i-1][j] = 0;                           //移动数字
        flag2 = 0;
        for (int k = 0; k <= OpenCount; k++){
            if (Compare(OpenList[k].State, a)){
                flag2 = 1;
                break;
            }
        }
        if (!flag2){
            for (int k = 0; k <= CloseCount; k++){
                if (Compare(CloseList[k].State, a)){
                    flag2 = 1;
                    break;
                }
            }
        }
        if (!flag2){
            Push(a, temp);
        }
        for (m = 0; m < 5; m++){
            for (n = 0; n < 5; n++){
                a[m][n] = temp[m][n];
            }
        }
    }
}

//主函数
int main(int argc, char const *argv[])
{
	//输出位置
    freopen("ans.txt", "w", stdout);
    //起始状态
    int a[5][5], w[5][5]={
        {-1, -1, -1, -1, -1},
        {-1,  1,  3,  4, -1},
        {-1,  8,  2,  5, -1},
        {-1,  7,  0,  6, -1},
        {-1, -1, -1, -1, -1}
    };
    //目标状态
    int result[5][5] = {
        {-1, -1, -1, -1, -1},
        {-1,  1,  2,  3, -1},
        {-1,  8,  0,  4, -1},
        {-1,  7,  6,  5, -1},
        {-1, -1, -1, -1, -1}
    };

    Push(w, w);
    while (-1 != OpenCount){//OPEN表不为空
        CloseCount++;
        //每完成一次移动，该状态即可放入CLOSE表中
        for (int i = 0; i < 5; i++){
            for (int j = 0; j < 5; j++){
                CloseList[CloseCount].State[i][j] = OpenList[OpenCount].State[i][j];                
                CloseList[CloseCount].FatherState[i][j] = OpenList[OpenCount].FatherState[i][j];                
            }
        }
        CloseList[CloseCount].Order = CloseCount;
        //更新当前状态
        for (int i = 0; i < 5; i++){
            for (int j = 0; j < 5; j++){
                a[i][j] = OpenList[OpenCount].State[i][j];                
            }
        }
        OpenCount--;
        if (Compare(result, a)){	//如果达到了目标状态则技术循环
            break;
        } else {
            Move(a);				//继续移动
        }
    }
    //输出到达目标状态的着法
    for (int k = 0; k <= CloseCount; k++){
        printf("No: %d\n", CloseList[k].Order);
        printf("  Parent\t\tChild\n");
        for (int i = 1; i < 4; i++){
            for (int j = 1; j < 4; j++){
                printf("%3d", CloseList[k].FatherState[i][j]);                           
            }
            printf("\t");
            for (int j = 1; j < 4; j++){
                printf("%3d", CloseList[k].State[i][j]);               
            }
            printf("\n");
        }
    }
    return 0;
}