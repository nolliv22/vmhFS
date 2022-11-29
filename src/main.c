// Libraries
#include "filesystem.c"     // Custom structures for Filesystem
#include "main.h"           // Standard libraries and global variable PATH

// Include all the commands
#include "create.c"
#include "write.c"
#include "read.c"
#include "remove.c"
#include "ls.c"
#include "size.c"

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
    "\t\tAvailable commands: create, write, read, remove, ls and size\n"
    "\t\tvhmFS FILE COMMAND to get help on the COMMAND\n"
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
                exit(0);
            }
        }

        else if (strcmp("read", command) == 0){
            if (argc == 4){
                char * file_path = argv[3];
                myFS_read(file_path);
            } else {
                printf("Usage: vhmFS FILE read FILE_PATH\n");
                exit(0);
            }
        }

        else if (strcmp("remove", command) == 0){
            if (argc == 4){
                char * path = argv[3];
                myFS_remove(path);
            } else {
                printf( "Usage: vhmFS FILE remove [DIR_PATH or FILE_PATH]\n");
                exit(0);
            }
        }

        else if (strcmp("ls", command) == 0){
            if (argc == 4){
                myFS_ls(argv[3], false);
            } else if (argc == 5){
                char * flag = argv[4];
                if (strcmp(flag, "-r") == 0){
                    myFS_ls(argv[3], true);
                }
            } else { 
                printf( "Usage: vhmFS FILE ls [OPTIONS] DIR_PATH\n"
                        "OPTIONS:\n"
                        "\t-r: recurse to all sub-directories\n"
                        "\t-d: sort files by date\n"); 
                exit(0);
            }
        }

        else if (strcmp("size", command) == 0){
            if (argc >= 5 && argc <= 7){
                char * path = argv[3];
                bool r;
                bool unit =  false;
                char * size_unit = malloc(2*sizeof(char));
                bool stat;

                for (int i=4; i<argc; i++){
                    if (strcmp(argv[i], "-r")==0){
                        r = true;
                    } else if ( strcmp(argv[i], "-b")==0 || 
                                strcmp(argv[i], "-k")==0 ||
                                strcmp(argv[i], "-m")==0 ||
                                strcmp(argv[i], "-g")==0){
                        strcpy(size_unit, argv[i]);
                        unit = true;
                    } else if (strcmp(argv[i], "-stat")==0) {
                        stat = true;
                    }
                }

                if (unit == true){
                    myFS_size(r, size_unit, stat, path);
                } else {
                    printf( "Missing PREFIX:\n"
                            "-b/-k/-m/-g: display size in B/KB/MB/GB\n");
                }
                
            } else {
                printf( "Usage: vhmFS FILE size [PREFIX] [OPTIONS] DIR_PATH\n"
                    "PREFIX:\n"
                    "\t-b/-k/-m/-g: display size in B/KB/MB/GB\n"
                    "OPTIONS:\n"
                    "\t-r: recurse to all sub-directories\n"
                    "\t-stat: advanced information about current file system\n");
                // TODO: Help for the command "size"
                exit(0);
            }
        }

        else {
            print_help();
            exit(0);
        }
    }

    exit(0);
}