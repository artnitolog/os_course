#include <stdio.h>

const char template[] = "rwxrwxrwx";

void print_rights(unsigned num) {
    char res[sizeof(template)] = "---------";
    for (int i = 0; i < sizeof(template) - 1; i++) {
        if ((num >> (sizeof(template) - 2 - i)) & 1) {
            res[i] = template[i];
        }
    }
    printf("%s\n", res);
}

int main(int argc, char *argv[])
{
    unsigned cur;
    for (int i = 1; i < argc; i++) {
        sscanf(argv[i], "%o", &cur);
        print_rights(cur);
    }
    return 0;
}