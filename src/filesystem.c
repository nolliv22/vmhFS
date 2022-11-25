#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variable
#define WORD_SIZE 32    // Size for file, directory names

// FILE SYSTEM STRUCTURES
typedef struct {                    // INODE: information about a file
    unsigned long int size;         // Number of bytes of the file associated with this inode
    char name[WORD_SIZE];           // Name of the file
    unsigned long int parent_id;    // The id of the file's parent directory 
} Inode;

typedef struct {    // FILE: inode (information) + bytes (data) of the file
    Inode inode;    // FInode of the file
    char * bytes;   // Bytes of the file
} File;

typedef struct {        // SPLITTED_PATH: a splitted path
    char **components;  // Components of the path (e.g. "/dir1/foo" -> ["dir1", "foo"])
    int number;         // Number of component (to get the last element which is the filename for a file path)
} Splitted_path;

typedef struct {                    // DIRECTORY
    char name[WORD_SIZE];           // Name of the directory
    unsigned long int parent_id;    // The id of the parent of this directory (CONVENTION: -1 if it's an empty directory)
} Directory;

typedef struct {                            // SUPERBLOCK
    unsigned long int inode_number;         // Number of file
    unsigned long int directory_number;     // Number of directory
    unsigned long int current_size;         // Size of the file system
    unsigned long int max_size;             // Maximum size of the file system
} SuperBlock;

typedef struct {                    // FILESYSTEM
    SuperBlock sb;                  // Superblock of the file system
    Inode * inode_array;            // Array storing inode of files in the file system
    char ** bytes_array;            // Array storing bytes of files in the file system
    Directory * directory_array;    // Array storing directories in the file system
} FileSystem;


// FILE SYSTEM FUNCTIONS

FileSystem get_FS(char * path){
    // Read a file system stored on the disk to the memory
    // INPUT: 
    //      path: the path of the file system on the disk
    // OUTPUT:
    //      fs: the file system structure

    // Open the file on the disk
    FILE * file;
    file = fopen(path, "rb");

    // Initialize the empty file system struct
    FileSystem fs;
    fs.directory_array = malloc(sizeof(Directory)*1);
    fs.inode_array = malloc(sizeof(Inode)*1);
    fs.bytes_array = malloc(sizeof(char *)*1);
    
    // Filling the file system
    // Read the superblock from the file
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

void free_FS(FileSystem fs){
    free(fs.bytes_array);
    free(fs.inode_array);
    free(fs.directory_array);
}

File getFile(char * input_path, char * destination_path){
    File file;

    FILE * input_file;
    input_file = fopen(input_path, "rb");
    // TODO: Check if file exist

    fseek(input_file, 0, SEEK_END);
    unsigned long int input_file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    file.inode.size = input_file_size;
    strcpy(file.inode.name, destination_path);  // Need to split the path
    file.inode.parent_id = 0;                   // Need to get the parent directory
                                                // Need to create recursively parent direcory to the root

    file.bytes = malloc(file.inode.size);
    fread(file.bytes, input_file_size, 1, input_file);

    fclose(input_file);

    return file;
}

FileSystem add_inode(FileSystem fs, Inode inode, char * bytes){
    fs.sb.inode_number += 1;
    fs.sb.current_size += sizeof(Inode) + inode.size;

    fs.inode_array = (Inode*)realloc(fs.inode_array, sizeof(Inode)*fs.sb.inode_number);
    fs.bytes_array = (char **)realloc(fs.bytes_array, sizeof(char*)*fs.sb.inode_number);

    fs.inode_array[fs.sb.inode_number-1] = inode;
    fs.bytes_array[fs.sb.inode_number-1] = bytes;

    return fs;
}

FileSystem rm_inode(FileSystem fs, int i){
    fs.sb.inode_number -= 1;

    if (fs.sb.inode_number == 0){
        // Re-initialize inode_array and bytes_array
        free(fs.inode_array);
        free(fs.bytes_array);
        fs.inode_array = malloc(sizeof(Inode));
        fs.bytes_array = malloc(sizeof(char*));

    } else {
        for (int k=i; k<fs.sb.inode_number; k++){
            fs.bytes_array[k] = fs.bytes_array[k+1];
            fs.inode_array[k] = fs.inode_array[k+1];
        }

        fs.inode_array = (Inode*)realloc(fs.inode_array, sizeof(Inode)*fs.sb.inode_number);
        fs.bytes_array = (char**)realloc(fs.bytes_array, sizeof(char*)*fs.sb.inode_number);
    }

    return fs;
}

Splitted_path split_path(char * input_path) {
    Splitted_path splitted;
    splitted.components = malloc(1*sizeof(char*));

    if (input_path[0] != '/'){
        printf("Path must start with /\n");
        exit(1);
    }

    int i = 0;
    splitted.number = 0;

    while (input_path[i] != '\0'){
        if (input_path[i] == '/'){
            i++;
            splitted.number++;
            splitted.components = (char**)realloc(splitted.components, splitted.number*sizeof(char*));
            splitted.components[splitted.number-1] = malloc(WORD_SIZE*sizeof(char));

        } else {
            strncat(splitted.components[splitted.number-1], &input_path[i], 1);
            i++;
        }
    }

    return splitted;   
}

// Extract only directory path from file path (e.g. /dir1/dir2/foo -> /dir1/dir2)
char * extract_dir_path(char * file_path){
    int i = 0;
    int last_slash;
    while (file_path[i] != '\0'){
        if (file_path[i] == '/'){
            last_slash = i;
        }
        i++;
    }
    
    char * dir_path = malloc(last_slash*sizeof(char));
    memmove(dir_path, file_path, last_slash);

    return dir_path;
}

// Find a directory knowing its name and parent_in inside directory_array
// Return -1 if the directory doesn't exist
unsigned long int find_directory(FileSystem fs, char *name, unsigned long int parent_id){
    for (int i=0; i<fs.sb.directory_number; i++){
        Directory dir;
        dir = fs.directory_array[i];

        if (strncmp(dir.name, name, strlen(name)) == 0 && dir.parent_id == parent_id){
            return i;
        }
    }

    return -1;
};

// Find a directory knowing its path only
// Return -1 if the directory doesn't exist
long int find_dir_from_path(FileSystem fs, char * dir_path){
    Splitted_path splitted = split_path(dir_path);

    unsigned long int current_parent_id = 0;    // Root

    for (int k=0; k<splitted.number; k++){
        unsigned long int dir_id = find_directory(fs, splitted.components[k], current_parent_id);
    
        if (dir_id == -1){
            // Dir not found
            return -1;
        } else {
            current_parent_id = dir_id;
        }
    }

    return current_parent_id;
}

// Find a file using its path and return its index
// Return -1 if the file doesn't exist
int find_file(FileSystem fs, char * file_path){
    // Default value set to -1 (Not found)
    int return_value = -1;

    Splitted_path splitted = split_path(file_path);
    char * file_name = splitted.components[splitted.number-1];

    // Check if the parent directory exists
    char * dir_path = extract_dir_path(file_path);
    long int parent_id = find_dir_from_path(fs, dir_path);
    if (parent_id == -1){
        return_value = -1;  // Parent directory doesn't exist
    }

    // Find a file in inode_array which matches the name and the parent_id
    for (int l=0; l<fs.sb.inode_number; l++){
        if (strncmp(fs.inode_array[l].name, file_name, strlen(file_name)) == 0 && fs.inode_array[l].parent_id == parent_id){
            return_value = l;
            break;
        }
    }

    // Free memory
    free(splitted.components);
    free(dir_path);    
    return return_value;
}

// Store a new Directory (supposing it doesn't exist) in the directory_array
FileSystem add_directory(FileSystem fs, char * name, unsigned long int parent_id){

    // Find empty to overwrite
    for(int i=0; i<fs.sb.directory_number; i++){
        if(fs.directory_array[i].parent_id == -1){
            Directory dir;
            dir.parent_id = parent_id;
            fs.directory_array[i] = dir;
            return fs;
        }
    }

    fs.sb.directory_number += 1;
    fs.directory_array = (Directory*)realloc(fs.directory_array, sizeof(Directory)*fs.sb.directory_number);

    Directory dir;
    strcpy(dir.name,name);
    dir.parent_id = parent_id;

    fs.directory_array[fs.sb.directory_number-1] = dir;
    fs.sb.current_size = fs.sb.current_size + sizeof(Directory);

    return fs;
}


// Remove a directory (supposing it exists) knowing its index
FileSystem rm_directory(FileSystem fs, unsigned long int id){
    fs.directory_array[id].parent_id=-1;
    fs.sb.directory_number=fs.sb.directory_number-1;
    fs.sb.current_size=fs.sb.current_size-sizeof(Directory);
    return fs;
}