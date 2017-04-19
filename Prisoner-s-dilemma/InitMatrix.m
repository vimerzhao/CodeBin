function Init = InitMatrix(flag, L, random)
% 接受参数flag，flag为1=0时为随机初始化，flag为1时为初始化中心
if flag==0
    % 获取合作者指定的比例
    % 获取方格大小
    Init = rand(L);
    % 根据比例分配网格数值，1表示合作，0表示背叛
    random = (random-1)/10;%合作者的比例
    Init(Init<random) = 1;
    Init(Init ~= 1) = 0;
elseif flag==1
    % 获取网格大小
    Init = ones(L);
    if mod(L, 2)==1
        Init((L+1)/2, (L+1)/2) = 0;
    elseif mod(L, 2)==0
        Init(L/2, L/2) = 0;
        Init(L/2 + 1, L/2 + 1) = 0;
        Init(L/2 + 1, L/2) = 0;
        Init(L/2, L/2 + 1) = 0;
    end
end