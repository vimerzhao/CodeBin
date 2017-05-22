// get min number.
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const* argv[]) {
    if (argc >= 2) {
        double min = atof(argv[1]);
        for (int i = 2; i < argc; i++) {
            double cur = atof(argv[i]);
            min = min < cur ? min : cur;
        }
        printf("%.2lf\n", min);
    }
    return 0;
}

