#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    bool file;
    unsigned long int nth;
    unsigned long int size;
    char name[64];
    char parent_directory[64];
    // TODO: Date
} Inode;

typedef struct {
    unsigned long int inode_number;
    unsigned long int current_size;
    unsigned long int max_size;
} SuperBlock;

typedef struct {
    SuperBlock sb;
    Inode * inode_array;
    char ** bytes_array;
} FileSystem;

FileSystem get_FS(char * path){
    FILE * file;
    file = fopen(path, "rb");

    FileSystem fs;
    fs.inode_array = malloc(sizeof(Inode)*1);
    fs.bytes_array = malloc(sizeof(char *)*1);

    fread(&fs.sb, sizeof(SuperBlock), 1, file);
    
    if (fs.sb.inode_number > 0){
        fs.inode_array = malloc(sizeof(Inode)*fs.sb.inode_number);
        fs.bytes_array = malloc(sizeof(char*)*fs.sb.inode_number);

        for (int i=0; i<fs.sb.inode_number; i++){
            fread(&fs.inode_array[i], sizeof(Inode), 1, file);

            fs.bytes_array[i] = malloc(fs.inode_array[i].size);
            fread(fs.bytes_array[i], fs.inode_array[i].size, 1, file);
        }
    }
    
    fclose(file);
    return fs;
}

int put_FS(char * path, FileSystem fs){
    FILE * file;
    file = fopen(path, "wb");

    fwrite(&fs.sb, sizeof(SuperBlock), 1, file);

    if (fs.sb.inode_number > 0){
        for (int i=0; i<fs.sb.inode_number; i++){
            fwrite(&fs.inode_array[i], sizeof(Inode), 1, file);
            fwrite(fs.bytes_array[i], fs.inode_array[i].size, 1, file);
        }
    }
    
    return 0;
}

int free_FS(FileSystem fs){
    free(fs.bytes_array);
    free(fs.inode_array);

    return 0;
}