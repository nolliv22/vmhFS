// Custom libraries
#include "filesystem.c"     // Custom structures for Filesystem
#include "main.h"           // Standard libraries and global variable PATH

int create(int size){       // size in megabytes
    filesystem fs;
    superblock sb;

    sb.max_size = size*1024*1024;
    sb.inode_number =  0;
    sb.current_size = sizeof(fs);

    fs.sb = sb;

    int output_fd;

    if ((output_fd = open(PATH, O_WRONLY | O_SYNC | O_CREAT, 00644)) < 0){
        printf("Could not open the output file: %s\n", PATH);
        
        if (errno == EACCES){
            printf("Permission denied\n");
            exit(0);
        } else {
            printf("Error when opening the output file, error code: %d\n", errno);
        }
        // TODO: More error handling 

    }

    int written_bytes = write(output_fd, &fs, sizeof(fs));

    if (written_bytes != sizeof(fs)){
        printf("Write error: the number of written bytes don't match the size of the buffer\n");
        // TODO: Error handling 
    }

    printf("Successfully created a vhm file system to %s with a maximum size of %d MB\n", PATH, size);
    exit(0);
}