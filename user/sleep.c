#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void 
main(int argc, const char *argv[]) {
    if(argc < 2) {
        fprintf(2, "error: ");
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}