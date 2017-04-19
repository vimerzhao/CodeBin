choose = get(handles.popupmenu3, 'Value');
% 原论文为1，我的工作为2
switch choose
    case 1
        PlayGame;
    case 2
        PlayNewGame;
end