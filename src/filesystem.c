#include <stdbool.h>

typedef struct {
    bool file;
    unsigned long int nth;
    unsigned long int size;
    char name[64];
    char parent_directory[64];
} Inode;

typedef struct {
    unsigned long int inode_number;
    unsigned long int current_size;
    unsigned long int max_size;
} Superblock;