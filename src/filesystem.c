#include <stdbool.h>

typedef struct {
    int data_size;
    char * content;
} data;

typedef struct inode {
    bool file;
    char * name;
    char * parent_directory;
    data * data_pointer;
} inode;

typedef struct {
    int inode_number;
    int current_size;
    int max_size;
} superblock;

typedef struct {
    superblock sb;
    data * data_array;
    struct inode * inode_array;
} filesystem;