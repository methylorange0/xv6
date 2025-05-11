// Memory leaks remain unresolved.
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int setArgs(char* newArgv[], int index) {
    char buf[1000];
    int p = 0, flag = 0;
    
    while(1) {
        if(read(0, &buf[p++], sizeof(char)) == 0) {   // EOF
            return 0;
        }
        while(buf[p - 1] != ' ' && buf[p - 1] != '\n') {
            read(0, &buf[p++], sizeof(char));
        }
        if(buf[p - 1] == '\n') {
            flag = 1;
        }
        buf[p - 1] = 0;
        newArgv[index] = (char*)malloc(strlen(buf) + 1);
        strcpy(newArgv[index++], buf);
        p = 0;
        if(flag) {
            newArgv[index] = 0;
            break;
        }
    }
    return 1;
}

int main(int argc, const char* argv[]) {
    if(argc < 2) {
        fprintf(2, "xargs: not enough args!\n");
        exit(1);
    }

    char* command = (char*)malloc(strlen(argv[1]) + 1);   // get command
    strcpy(command, argv[1]);
    char* newArgv[MAXARG];
    int index;
    for(index = 0; index + 1 < argc; index++) {                    // set const args
        newArgv[index] = (char*)malloc(strlen(argv[index + 1] + 1));
        strcpy(newArgv[index], argv[index + 1]);
    }
    while(setArgs(newArgv, index)) {
        if(fork() == 0) {
            exec(command, newArgv);
        } else {
            wait(0);
        }
    }

    for(int i = 0; i < index; i++) {
        free(newArgv[index]);
    }
    exit(0);

}