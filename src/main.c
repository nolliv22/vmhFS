// Include all the commands
#include "create.c"

// Export global variable
extern char * PATH;
char * PATH;

void print_help(){
    printf(
    "\e[1m""vhmFS""\e[0m" " - a simple file system named after: Villon, Hamza and Moaad\n"
    "Usage: vhmFS FILE COMMAND\n"
    "\n"
    "\e[1m""OPTIONS:\n""\e[0m"
    "\tFILE\n"
    "\t\tPath to the file system (e.g. /tmp/myFS)\n"
    "\tCOMMAND\n"
    "\t\tAvailable commands: create\n"
    "\n"
    );
}

int main(int argc, char * argv[]){
    if (argc <  3){
        print_help();
        exit(0);

    } else {
        PATH = argv[1];
        char * command = argv[2];

        if (strcmp("create", command) == 0){
            if (argc == 4){
                int size = atoi(argv[3]);
                create(size);
            } else {
                printf("Usage: vhmFS FILE create SIZE\n");
                // TODO: Help for the command "create"
                exit(0);
            }

        } else {
            print_help();
            exit(0);
        }
    }

    exit(0);
}