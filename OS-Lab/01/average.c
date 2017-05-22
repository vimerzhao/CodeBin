// get average number.
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const* argv[]) {
    if (argc >= 2) {
        double sum = 0;
        for (int i = 1; i < argc; i++) {
            double cur = atof(argv[i]);
            sum += cur;
        }
        printf("%.2lf\n", sum/(argc-1));
    }
    return 0;
}

