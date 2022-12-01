// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Global variable
#define WORD_SIZE 32        // Size for file, directory names
#define MAX_CHILDREN 20     // Maximum number of child directory and files

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
    long int parent_id;             // The id of the parent of this directory (CONVENTION: -1 if it's an empty directory)
} Directory;

typedef struct {                        // DIR_FILES: files contained in a directory
    int file_ids[MAX_CHILDREN];         // Array of file's ids
    long int file_number;               // Number of files
} Dir_files;

typedef struct {                        // DIR_CHILDREN: Directories contained in a directory
    int children_ids[MAX_CHILDREN];     // Array of children id (Maximum 20 children)
    int children_number;                // Number of child directories
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
/* 
    Read a file system stored on the disk to the memory
    INPUT: 
        char * path: the path of the file system on the disk
    OUTPUT:
        FileSystem fs: the file system structure if success
        Otherwise exit(-1)
*/
FileSystem get_FS(char * path);

/* 
    Write a file system stored on the memory to the disk
    INPUT: 
        char * path: the path of the file system on the disk
        FileSystem fs: the file system structure
    OUTPUT:
        int r: 0 if success, 
        Otherwise exit(-1)
*/
int put_FS(char * path, FileSystem fs);

/*
    Free memory allocated to store the filesystem
    INPUT:
        FileSystem fs: filesystem to free
    OUTPUT:
        void
*/
void free_FS(FileSystem fs);

/*
    Create all intermediate directories from a path
    INPUT:
        FileSystem fs: filesystem in which we want to create the directories
        char * destination_path: path of the directory we need to create
    OUTPUT: FileSystem fs: modified file system
*/
FileSystem create_dir_from_path(FileSystem fs, char * destination_path);

/*
    Get a file on the disk to work with it on the memory
    INPUT:
        fs: file system
        input_path: path of the file on the disk
        destination_path: path of the file in the file system
    OUTPUT:
        file: File structure on the memory
*/
File get_file(FileSystem fs, char * input_path, char * destination_path);

/*
    Add a file to the filesystem's file_array
    INPUT:
        FileSystem fs: file system in which the file will be stored
        char * input_path: path of the file on the disk
        char * destination_path: path of the file in the file system
    OUTPUT:
        File file: file struct that contains the inode and the bytes of the file
*/
FileSystem add_file(FileSystem fs, File file);

/*
    Remove a file from the filesystem's file_array
    INPUT:
        FileSystem fs: file system in which the file will be removed
        int i: the index of the file in the file_array
    OUTPUT:
        FileSystem fs: modified file system
*/
FileSystem rm_file(FileSystem fs, int i);

/*
    Split a path into components
    INPUT:
        char * path: path to be splitted
    OUTPUT:
        Splitted_path splitted: structure that contains the components and the number of component
*/
Splitted_path split_path(char * input_path);

/*
    Free the Splitted_path struct
    INPUT:
        Splitted_path splitted: structure to be freed
    OUTPUT:
        int r: Return 0
*/
void free_splitted(Splitted_path splitted);

/*
    Extract only directory path from file path
    (e.g. /dir1/dir2/foo -> /dir1/dir2)
    INPUT:
        char * file_path: file path to extract directory path
    OUTPUT:
        char * dir_path: extracted directory path if success
        Otherwise NULL
*/
char * extract_dir_path(char * file_path);

/*
    Find a directory knowing its name and parent_in inside directory_array
    INPUT:
        FileSystem fs: file system in which we find the directory
        char * name: name of the directory
        long int parent_id: parent_id of the directory we are finding
    OUTPUT:
        int dir_id: index of the directory in the directory_array
        Otherwise -1 if the directory doesn't exist

*/
long int find_directory(FileSystem fs, char * name, long int parent_id);

/*
    Find a directory knowing its path only
    INPUT:
         FileSystem fs: file system in which we find the directory
         char * dir_path: path of the directory
    OUTPUT:
         long int dir_id: Index of the directory in the directory_array on success
         Otherwise return -1;
*/
long int find_dir_from_path(FileSystem fs, char * dir_path);

/*
    Find a file knowing its path only
    INPUT:
         FileSystem fs: file system in which we find the directory
         char * file_path: path of the file
    OUTPUT:
         long int file_id: Index of the file in the file_array on success
         Otherwise return -1;
*/
long int find_file_from_path(FileSystem fs, char * file_path);


/*
    Get all the child directories of a directory
    INPUT:
         FileSystem fs: file system in which we find the child directories
         long int dir_id: the index of the direcory in the directory_array
    OUTPUT:
         Dir_children: Structure contains the children and the number
*/
Dir_children get_dir_children(FileSystem fs, long int id);

/*
    Get all the child files of a directory
    INPUT:
         FileSystem fs: file system in which we find the child files
         long int file_id: the index of the file in the file_array
    OUTPUT:
         Dir_files: Structure contains the children and the number
*/
Dir_files get_dir_files(FileSystem fs, long int dir_id);

/*
    Store a new Directory (supposing it doesn't exist) in the directory_array
    INPUT:
         FileSystem fs: file system in which we store the directory
         char * name: the name of the directory
         long int parent_id: parent_id of the directory
    OUTPUT:
         Dir_files: Structure contains the children and the number
*/
FileSystem add_directory(FileSystem fs, char * name, long int parent_id);

/*
    Remove a directory (supposing it exists) in the directory_array
    INPUT:
         FileSystem fs: file system in which we remove the directory
         long int dir_id: index of the directory in the directory
    OUTPUT:
         FileSystem fs: Structure contains the children and the number
*/
FileSystem rm_directory(FileSystem fs, unsigned long int id);

/*
    Get the number of files in a directory
    INPUT:
         FileSystem fs: file system in which we find the directory
         long int dir_id: index of the directory in the directory
    OUTPUT:
         int num_files: the number of files in a directory
*/
int num_of_files(FileSystem fs, long int dir_id);

/*
    Get the size of files inside a directory knowing its index
    INPUT:
         FileSystem fs: file system in which we find the directory
         long int dir_id: index of the directory in the directory
    OUTPUT:
         long int size: size of files
*/
long int size_dir_files(FileSystem fs, int dir_id);

/*
    Convert the size flag to the desired unit
    INPUT:
         char * size_unit: unit flag
    OUTPUT:
         char * size_unit: unit string

*/
char * extract_size_unit(char * size_unit);

/*
    Convert the size to the desired unit
    INPUT:
         long int size: size in bytes
         char * size_unit: destination unit
    OUTPUT:
         long int number: size in desired unit
*/
long int convert_size(long int size, char * size_unit);