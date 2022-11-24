#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned long int size;     // Size of the file associated to this inode 
    char name[64];
    unsigned long int parent_id;
} Inode;

typedef struct {
    Inode inode;
    char * bytes;
} File;

typedef struct {
    char name[64];
    unsigned long int parent_id;
} Directory;

typedef struct {
    unsigned long int inode_number;
    unsigned long int directory_number;
    unsigned long int current_size;
    unsigned long int max_size;
} SuperBlock;

typedef struct {
    SuperBlock sb;
    Inode * inode_array;
    char ** bytes_array;
    Directory * directory_array; 
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
        free_FS(fs);
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
/*
char ** split_path(char * path) {
    char PATH[strlen(path)];
    strcpy(PATH, path); 

    int count = 0;
    for (int k=0; k<strlen(PATH); k++){
        if (PATH[k] == '/'){
            count++;
        }
    }

    int i = 0;
    char *p = strtok (PATH, "/");
    char **path_array = malloc(sizeof(char*)*3);

    while (p != NULL) {
        path_array[i] = malloc(sizeof(char*));
        printf("%s\n", p);
        path_array[i] = p;
        i++;
        p = strtok (NULL, "/");
    }

    return path_array;   
}
*/

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

// Find the file stored in this path and return its index 
int find_file(FileSystem fs, char * path){
    char ** path_array = split_path(path);

    char * file_name = path_array[0];
    unsigned long int current_parent_id = 0;

    printf("%s\n", file_name);

    for (int k=0; k<sizeof(path_array)-1; k++){
        unsigned long int dir_id = find_directory(fs, fs.directory_array[k].name, current_parent_id);

        if (dir_id == -1){
            // Dir not found
            return -1;
        } else {
            current_parent_id = dir_id;
        }
    }

    for (int l=0; l<fs.sb.inode_number; l++){
        if (strncmp(fs.inode_array[l].name, file_name, strlen(file_name)) && fs.inode_array[l].parent_id == current_parent_id){
            return l;
        }
    }

    return -1;
}

// Store a new Directory in the directory_array
FileSystem add_directory(FileSystem fs, char * name, unsigned long int parent_id){

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


// Remove a directory knowing its index
FileSystem rm_directory(FileSystem fs, unsigned long int id){
    fs.directory_array[id].parent_id=-1;
    fs.sb.directory_number=fs.sb.directory_number-1;
    fs.sb.current_size=fs.sb.current_size-sizeof(Directory);
    return fs;
}