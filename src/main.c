// Libraries
#include "filesystem.c"     // Custom structures for Filesystem
#include "main.h"           // Standard libraries and global variable PATH

// Include all the commands
#include "create.c"
#include "write.c"
#include "read.c"
#include "remove.c"
#include "ls.c"

#include "test.c"

// Export global variable to share across file system functions
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
                myFS_create(size);
            } else {
                printf("Usage: vhmFS FILE create SIZE\n");
                // TODO: Help for the command "create"
                exit(0);
            }
        } 
        
        else if (strcmp("write", command) == 0){
            if (argc == 5){
                char * input_path = argv[3];
                char * destination_path = argv[4];
                myFS_write(input_path, destination_path);
            } else {
                printf("Usage: vhmFS FILE write INPUT_PATH DESTINATION_PATH\n");
                // TODO: Help for the command "write"
                exit(0);
            }
        }

        else if (strcmp("read", command) == 0){
            if (argc == 4){
                char * file_path = argv[3];
                myFS_read(file_path);
            } else {
                printf("Usage: vhmFS FILE read FILE_PATH\n");
                // TODO: Help for the command "read"
                exit(0);
            }
        }

        else if (strcmp("remove", command) == 0){
            if (argc == 4){
                char * path = argv[3];
                myFS_remove(path);
            } else {
                printf( "Usage: vhmFS FILE remove [DIR_PATH or FILE_PATH]\n");
                // TODO: Help for the command "remove"
                exit(0);
            }
        }

        else if (strcmp("ls", command) == 0){
            if (argc == 3){
            
                myFS_ls(argv[3],false,true);
            } else if(argc == 4){
                myFS_ls(argv[4], true, true);
            }
        }

        else if (strcmp("size", command) == 0){
            if (argc == 3){
                //myFS_size();
            } else {
                printf( "Usage: vhmFS FILE size [OPTIONS] DIR_PATH\n"
                        "\tOPTIONS:\n"
                        "\t\t-r: recurse to all sub-directories\n"
                        "\t\t-b/-k/-m/-g: display size in B/KB/MB/GB\n"
                        "\t\t-stat: advanced information about current file system\n");
                // TODO: Help for the command "size"
                exit(0);
            }
        }

        else if (strcmp("test", command) == 0){
            if (argc == 3){
                test();
            }
        }

        else {
            print_help();
            exit(0);
        }
    }

    exit(0);
}