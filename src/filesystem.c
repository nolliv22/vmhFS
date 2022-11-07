#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned long int id;       //TODO: remove it
    bool file;
    unsigned long int size;     // Size of the file associated to this inode 
    char name[64];
    char parent_directory[64];
} Inode;

typedef struct {
    Inode inode;
    char * bytes;
} File;

typedef struct {
    unsigned long int inode_number;
    unsigned long int current_size;
    unsigned long int max_size;
} SuperBlock;

typedef struct {
    SuperBlock sb;
    Inode * inode_array;
    char ** bytes_array;
    // Directory * directory_array;
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

File getFile(char * input_path, char * destination_path){
    File file;

    FILE * input_file;
    input_file = fopen(input_path, "rb");
    // TODO: Check if file exist

    fseek(input_file, 0, SEEK_END);
    unsigned long int input_file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    file.inode.file = true;
    file.inode.size = input_file_size;
    strcpy(file.inode.name, destination_path);  // Need to split the path
    strcpy(file.inode.parent_directory, "/");   // Need to get the parent directory
                                                // Need to create recursively parent direcory to the root

    file.bytes = malloc(file.inode.size);
    fread(file.bytes, input_file_size, 1, input_file);

    fclose(input_file);

    return file;
}

FileSystem add_inode(FileSystem fs, Inode inode, char * bytes){
    inode.id = fs.sb.inode_number + 1;

    fs.sb.inode_number += 1;
    fs.sb.current_size += sizeof(Inode) + inode.size;

    fs.inode_array = (Inode*)realloc(fs.inode_array, sizeof(Inode)*fs.sb.inode_number);
    fs.bytes_array = (char **)realloc(fs.bytes_array, sizeof(char*)*fs.sb.inode_number);

    fs.inode_array[fs.sb.inode_number-1] = inode;
    fs.bytes_array[fs.sb.inode_number-1] = bytes;

    return fs;
}

// FileSystem rm_inode(FileSystem fs, unsigned long int inode_id){
//     fs.sb.inode_number -= 1;
//     Inode * new_inode_array = malloc(sizeof(Inode)*fs.sb.inode_number);
//     char ** new_bytes_array = malloc(sizeof(char*)*fs.sb.inode_number);


//     for (unsigned long int i=inode_id-1; i<fs.sb.inode_number; i++){

//         fs.inode_array[i] = fs.inode_array[i+1];
//         fs.inode_array[i].id -= 1;

//         fs.bytes_array[i] = malloc(fs.inode_array[i].size);
//         fs.bytes_array[i] = fs.bytes_array[i+1];
//     }

//     fs.inode_array = (Inode*)realloc(fs.inode_array, sizeof(Inode)*fs.sb.inode_number);
//     fs.bytes_array = (char **)realloc(fs.bytes_array, sizeof(char*)*fs.sb.inode_number);

// }

int free_FS(FileSystem fs){
    free(fs.bytes_array);
    free(fs.inode_array);

    return 0;
}