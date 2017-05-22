#include<unistd.h>
int main()
{
char *argv[ ]={"ls", "-al", "/etc/passwd", NULL};
char *envp[ ]={"PATH=/bin", NULL};
execve("/bin/ls", argv, envp);
}
