// commit: change >= in scanf

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
    double rate;
    if (argc <= 1 || sscanf(argv[1], "%lf", &rate) != 0){
        exit(1);
    }
    for (int i = 2; i < argc; i++) {
        double change;
        if (sscanf(argv[i], "%lf", &change) != 0) {
            exit(1);
        }
        rate *= 1 + change / 100;
        rate = round(rate * 10000);
        rate /= 10000;
    }
    printf("%.4lf\n", rate);
    return 0;
}