function Init = FixedInit(L, random, FixedRandom)
% 接受参数flag，flag为1=0时为随机初始化，flag为1时为初始化中心
% 获取合作者指定的比例
% 获取方格大小
Init = rand(L);
% 根据比例分配网格数值，1表示合作，0表示背叛
random = (random-1)/10;%合作者的比例
Init(Init<random) = 1;
Init(Init ~= 1) = 0;
if FixedRandom== 11
    Init(round(L/2), :) = -1;
    Init(:, round(L/2)) = -1;
elseif FixedRandom==12

else
    for i=1:L
        for j=1:L
           if rand()<=((FixedRandom-1)/100)
              Init(i, j) = -1;% -1表示坚定地背叛者
           end
        end
    end    
end
    
