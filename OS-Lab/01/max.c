// get max number.
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const* argv[]) {
    if (argc >= 2) {
        double max = atof(argv[1]);
        for (int i = 2; i < argc; i++) {
            double cur = atof(argv[i]);
            max = max > cur ? max : cur;
        }
        printf("%.2lf\n", max);
    }
    return 0;
}
