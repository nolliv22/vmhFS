#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variable
#define WORD_SIZE 32    // Size for file, directory names

// FILE SYSTEM STRUCTURES
typedef struct {           // INODE: information about a file
    long int size;         // Number of bytes of the file associated with this inode
    char name[WORD_SIZE];  // Name of the file
    long int parent_id;    // The id of the file's parent directory 
} Inode;

typedef struct {    // FILE: inode (information) + bytes (data) of the file
    Inode inode;    // Inode of the file
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

typedef struct {
    int file_ids[20];       // Array of file's ids
    long int file_number;
} Dir_files;

typedef struct {        
    int children_ids[20];         // Array of children id (Maximum 20 children)
    int children_number;         
} Dir_children;

typedef struct {                    // SUPERBLOCK
    long int file_number;           // Number of file
    long int directory_number;      // Number of directory
    long int directory_array_size;  // Size of the directory array (!!! != directory_number, it counts also empty Directory (parent_id = -1))        
    long int current_size;          // Size of the file system
    long int max_size;              // Maximum size of the file system
} SuperBlock;

typedef struct {                    // FILESYSTEM
    SuperBlock sb;                  // Superblock of the file system
    File * file_array;              // Array storing inode of files in the file system
    Directory * directory_array;    // Array storing directories in the file system
} FileSystem;


// FILE SYSTEM FUNCTIONS

// Function definitions
FileSystem get_FS(char * path);
int put_FS(char * path, FileSystem fs);
void free_FS(FileSystem fs);

FileSystem create_dir_from_path(FileSystem fs, char * destination_path);
File get_file(FileSystem fs, char * input_path, char * destination_path);

FileSystem add_file(FileSystem fs, File file);
FileSystem rm_file(FileSystem fs, int i);

Splitted_path split_path(char * input_path);
int free_splitted(Splitted_path splitted);
char * extract_dir_path(char * file_path);

unsigned long int find_directory(FileSystem fs, char *name, unsigned long int parent_id);
long int find_dir_from_path(FileSystem fs, char * dir_path);

long int find_file_from_path(FileSystem fs, char * file_path);

Dir_children get_dir_children(FileSystem fs, unsigned long int id);
Dir_files get_dir_files(FileSystem fs, long int dir_id);

FileSystem add_directory(FileSystem fs, char * name, unsigned long int parent_id);
FileSystem rm_directory(FileSystem fs, unsigned long int id);

int num_of_files(FileSystem fs, long int id);
long int size_dir_files(FileSystem fs, int dir_index);

char* extract_size_unit(char * size_unit);
long int convert_size(long int size3,char * size_unit);

FileSystem get_FS(char * path){
    // Read a file system stored on the disk to the memory
    // INPUT: 
    //      path: the path of the file system on the disk
    // OUTPUT:
    //      fs: the file system structure
    // TODO: handle file return code

    // Open the file on the disk
    FILE * fs_file;
    fs_file = fopen(path, "rb");

    // Initialize the empty file system struct
    FileSystem fs;
    
    // Read the superblock from the file
    fread(&fs.sb, sizeof(SuperBlock), 1, fs_file);

    // Read directories
    if (fs.sb.directory_array_size > 0){
        fs.directory_array = malloc(sizeof(Directory)*fs.sb.directory_array_size);

        for (int i=0; i<fs.sb.directory_array_size; i++){
            Directory dir;

            // Read directory
            fread(&dir, sizeof(Directory), 1, fs_file);

            // Save the directory to the directory_array
            fs.directory_array[i] = dir;
        }
    }

    // Read files
    if (fs.sb.file_number > 0){
        fs.file_array = malloc(sizeof(File)*fs.sb.file_number);

        for (int j=0; j<fs.sb.file_number; j++){
            File file;

            // Read file's inode
            fread(&file.inode, sizeof(Inode), 1, fs_file);

            // Read file's bytes
            file.bytes = malloc(file.inode.size);
            fread(file.bytes, file.inode.size, 1, fs_file);

            // Save the file to the file_array
            fs.file_array[j] = file;
        }
    }

    fclose(fs_file);
    return fs;
}

int put_FS(char * path, FileSystem fs){
    // Write a file system stored on the memory to the disk
    // INPUT: 
    //      path: the path of the file system on the disk
    //      fs: the file system structure on the memory
    // TODO: handle file return code

    // Open the file on the disk
    FILE * fs_file;
    fs_file = fopen(path, "wb");

    // Write superblock
    fwrite(&fs.sb, sizeof(SuperBlock), 1, fs_file);

    // Write directories
    if (fs.sb.directory_array_size > 0){
        for (int i=0; i<fs.sb.directory_array_size; i++){
            Directory dir = fs.directory_array[i];

            // Read directory
            fwrite(&dir, sizeof(Directory), 1, fs_file);
        }
    }

    // Write files
    if (fs.sb.file_number > 0){
        for (int j=0; j<fs.sb.file_number; j++){
            File file = fs.file_array[j];

            // Write file's inode
            fwrite(&file.inode, sizeof(Inode), 1, fs_file);

            // Write file's bytes
            fwrite(file.bytes, file.inode.size, 1, fs_file);
        }
    }

    fclose(fs_file);
    return 0;
}

void free_FS(FileSystem fs){
    free(fs.directory_array);
    free(fs.file_array);
}

FileSystem create_dir_from_path(FileSystem fs, char * destination_path){
    // Create all intermediate directories from a path
    
    // Find the file parent_id
    char * dir_path = extract_dir_path(destination_path);
    long int is_dir = find_dir_from_path(fs, dir_path);

    // TODO: Create all intermediate directories if the directory doesn't exist
    if (is_dir == -1){
        Splitted_path splitted_dir = split_path(dir_path);
        long int current_parent_id = 0; // Root's directory id

        for (int i=0; i<splitted_dir.number; i++){
            char * dir_name = splitted_dir.components[i];
            long int dir_id = find_directory(fs, dir_name, current_parent_id);

            if (dir_id == -1){
                // If the directory doesn't exist, then create if
                fs = add_directory(fs, dir_name, current_parent_id);
                current_parent_id = fs.sb.directory_array_size-1;
            } else {
                // Else, the directory exists, then we go inside
                current_parent_id = dir_id;
            }
        }
        
        free_splitted(splitted_dir);
    }
    
    // Else: nothing to change

    free(dir_path);
    return fs;
}

File get_file(FileSystem fs, char * input_path, char * destination_path){
    // Get a file on the disk to work with it on the memory
    // INPUT:
    //      fs: file system
    //      input_path: path of the file on the disk
    //      destination_path: path of the file in the file system
    // OUTPUT:
    //      file: File structure on the memory

    // Read the file
    FILE * input_file;
    input_file = fopen(input_path, "rb");
    // TODO: Check if file exist

    // Get file size
    fseek(input_file, 0, SEEK_END);
    unsigned long int input_file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    // Store the file in the file_array
    File file;

    // Generate file's inode
    file.inode.size = input_file_size;
    Splitted_path splitted = split_path(destination_path);
    char * filename = splitted.components[splitted.number-1];
    strcpy(file.inode.name, filename);
    
    char * dir_path = extract_dir_path(destination_path);
    long int dir_id = find_dir_from_path(fs, dir_path);
    file.inode.parent_id = dir_id;

    // Store file's bytes in a buffer
    file.bytes = malloc(file.inode.size);
    fread(file.bytes, input_file_size, 1, input_file);

    fclose(input_file);

    free_splitted(splitted);
    free(dir_path);
    return file;
}

FileSystem add_file(FileSystem fs, File file){
    // Update superblock
    fs.sb.file_number += 1;
    fs.sb.current_size += sizeof(Inode) + file.inode.size;

    // Update file_array
    if (fs.sb.file_number == 1){
        fs.file_array = malloc(1*sizeof(File));
        fs.file_array[0] = file;
    } else {
        fs.file_array = (File*)realloc(fs.file_array, sizeof(File)*fs.sb.file_number);
        fs.file_array[fs.sb.file_number-1] = file;
    }

    return fs;
}

FileSystem rm_file(FileSystem fs, int i){
    File file = fs.file_array[i];

    // Update superblock
    fs.sb.file_number -= 1;
    fs.sb.current_size -= sizeof(Inode) + file.inode.size;

    // Update file_array
    if (fs.sb.file_number == 0){
        // Re-initialize inode_array and bytes_array
        fs.file_array = malloc(0*sizeof(File));

    } else {
        for (int k=i; k<fs.sb.file_number; k++){
            fs.file_array[k] = fs.file_array[k+1];
        }

        fs.file_array = (File*)realloc(fs.file_array, sizeof(File)*fs.sb.file_number);
    }

    return fs;
}

Splitted_path split_path(char * path){
    Splitted_path splitted;
    splitted.number = 0;

    char path_const[strlen(path)];
    strcpy(path_const, path);

    if (strcmp(path, "/")==0){
        splitted.components = calloc(1,sizeof(char*));
        splitted.components[0] = "/";
        return splitted;
    }
    
    for (int k=0; k<strlen(path_const); k++){
        if (path_const[k] == '/'){
            splitted.number++;
        }
    }
    
    int i = 0;
    char *p = strtok (path_const, "/");
    splitted.components = calloc(splitted.number,sizeof(char*));

    while (p != NULL){
        splitted.components[i] = malloc(WORD_SIZE*sizeof(char));
        strcpy(splitted.components[i], p);
        i++;
        p = strtok(NULL, "/");
    }

    return splitted;   
}

int free_splitted(Splitted_path splitted){
    for (int i=0; i<splitted.number; i++){
        free(splitted.components[i]);
    }
    return 0;
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

    char * dir_path;
    if (last_slash == 0){
        dir_path = malloc(2*sizeof(char));
        strcpy(dir_path, "/");
    } else {
        dir_path = malloc(last_slash*sizeof(char));
        strncpy(dir_path, file_path, last_slash);
        dir_path[last_slash] = '\0';
    }

    return dir_path;
}

// Find a directory knowing its name and parent_in inside directory_array
// Return -1 if the directory doesn't exist
unsigned long int find_directory(FileSystem fs, char *name, unsigned long int parent_id){
    for (int i=0; i<fs.sb.directory_array_size; i++){
        Directory dir = fs.directory_array[i];

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

    free_splitted(splitted);
    return current_parent_id;
}

// Find a file using its path and return its index
// Return -1 if the file doesn't exist
long int find_file_from_path(FileSystem fs, char * file_path){
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
    for (int i=0; i<fs.sb.file_number; i++){
        File file = fs.file_array[i];
        if (strncmp(file.inode.name, file_name, strlen(file_name)) == 0 && file.inode.parent_id == parent_id){
            return_value = i;
            break;
        }
    }

    // Free memory
    free_splitted(splitted);
    free(dir_path);    
    return return_value;
}

/*Returns a struct that contains the indeces of a directory's 
children and their id knowing the index of the directory*/
Dir_children get_dir_children(FileSystem fs, unsigned long int dir_id){
    Dir_children dc;
    dc.children_number=0;

    for (int i=0; i<fs.sb.directory_array_size;i++){ 
        if (fs.directory_array[i].parent_id == dir_id){
            dc.children_number++; 
            dc.children_ids[dc.children_number-1]=i;
        }
    }
    return dc; 
}

// Get all the file that belong to a directory
Dir_files get_dir_files(FileSystem fs, long int dir_id){
    Dir_files df;
    df.file_number = 0;
    for (int i=0; i<fs.sb.file_number;i++){
        if (fs.file_array[i].inode.parent_id==dir_id){
            df.file_number++; 
            df.file_ids[df.file_number-1]=i;
        }
    }
    return df;
}

// Store a new Directory (supposing it doesn't exist) in the directory_array
FileSystem add_directory(FileSystem fs, char * name, unsigned long int parent_id){
    // Update superblock
    fs.sb.directory_number += 1;
    fs.sb.current_size = fs.sb.current_size + sizeof(Directory);

    // Create Directory struct
    Directory dir;
    strcpy(dir.name,name);
    dir.parent_id = parent_id;

    // Find empty space to overwrite it
    long int empty_space = -1;
    for(int i=0; i<fs.sb.directory_array_size; i++){
        if(fs.directory_array[i].parent_id == -1){
            empty_space = i;
        }
    }

    // Add the directory
    if (empty_space != -1){
        // If an empty space is found
        fs.directory_array[empty_space] = dir;
        fs.sb.directory_array_size = fs.sb.directory_array_size; // !!! The array size does NOT change
    } else {
        // Else we extend the director_array
        fs.sb.directory_array_size += 1; // !!! The array size CHANGES
        fs.directory_array = (Directory*)realloc(fs.directory_array, sizeof(Directory)*fs.sb.directory_array_size);
        fs.directory_array[fs.sb.directory_number-1] = dir;
    }

    return fs;
}


// Remove a directory (supposing it exists) knowing its index
FileSystem rm_directory(FileSystem fs, unsigned long int dir_id){
    // Empty the cell
    strcpy(fs.directory_array[dir_id].name, "TRASHED");
    fs.directory_array[dir_id].parent_id=-1;

    fs.sb.directory_number = fs.sb.directory_number-1;
    fs.sb.directory_array_size = fs.sb.directory_array_size; // !!! The array size does NOT change
    fs.sb.current_size=fs.sb.current_size-sizeof(Directory);
    return fs;
}

// returns the number of files in a directory
int num_of_files(FileSystem fs, long int id){
    int num_files=0;
    for (int i=0;i<fs.sb.file_number;i++){
        if (fs.file_array[i].inode.parent_id==id){
                num_files++;
            }
        }
    return num_files;
}

//returns the size of files imside a directory knowing its index
long int size_dir_files(FileSystem fs, int dir_index){
    if (num_of_files(fs,dir_index)==0){
        return 0;
    }

    long int size = 0;
    for (int i=0;i<fs.sb.file_number;i++){
        if (fs.file_array[i].inode.parent_id==dir_index){
            size += fs.file_array[i].inode.size;
        }
    }
    return size;
}

//Convert the size to the desired unit
char * extract_size_unit(char * size_unit){ 
   if (strncmp(size_unit,"-b",2)==0){
       strcpy(size_unit,"B");        
    }
    else if (strncmp(size_unit,"-k",2)==0){
       strcpy(size_unit,"KB");

    } else if (strncmp(size_unit,"-m",2)==0){
       strcpy(size_unit,"MB");

    } else if (strncmp(size_unit,"-g",2)==0){
       strcpy(size_unit,"GB");

    }
    return size_unit;
}


//Convert the size to the desired unit
long int convert_size(long int size3,char * size_unit){
    float number;
    if (strncmp(size_unit,"-b",2)==0){
        number=size3;

    } else if (strncmp(size_unit,"-k",2)==0){
        strcpy(size_unit,"KB");
        number=(float)size3/(1e3);

    } else if (strncmp(size_unit,"-m",2)==0){
        strcpy(size_unit,"MB");
        number=(float)size3/(1e6);

    } else if (strncmp(size_unit,"-g",2)==0){
        strcpy(size_unit,"GB");
        number=(float)size3/(1e9);

    }
    return number;
}
