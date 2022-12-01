#include "filesystem.h"

FileSystem get_FS(char * path){
    // Open the file on the disk
    FILE * fs_file;

    fs_file = fopen(path, "rb");
    if (fs_file ==  NULL){
        printf("Could not open the file\n");
        printf("Exiting...\n");
        exit(-1);}

    // Initialize the empty file system struct
    FileSystem fs;
    
    // Read the superblock from the file
    if (fread(&fs.sb, sizeof(SuperBlock), 1, fs_file) !=  1){
        printf("Read SuperBlock failed\n");
        printf("Exiting...\n");
        exit(-1);}

    // Read directories
    if (fs.sb.directory_array_size > 0){
        // Initialize the directory_array
        fs.directory_array = malloc(sizeof(Directory)*fs.sb.directory_array_size);

        for (int i=0; i<fs.sb.directory_array_size; i++){
            Directory dir;

            // Read directory
            if (fread(&dir, sizeof(Directory), 1, fs_file) != 1){
                printf("Read Directory failed\n");
                printf("Exiting...\n");
                exit(-1);}

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
            if (fread(&file.inode, sizeof(Inode), 1, fs_file) != 1){
                printf("Read file inode failed\n");
                printf("Exiting...\n");
                exit(-1);}

            // Read file's bytes
            file.bytes = malloc(file.inode.size);
            if (fread(file.bytes, file.inode.size, 1, fs_file) != 1){
                printf("Read file bytes failed\n");
                printf("Exiting...\n");
                exit(-1);}

            // Save the file to the file_array
            fs.file_array[j] = file;
        }
    }

    if (fclose(fs_file) != 0){
        printf("Close file failed\n");
        printf("Exiting...\n");
        exit(-1);}

    return fs;
}

int put_FS(char * path, FileSystem fs){
    // Open the file on the disk
    FILE * fs_file;

    fs_file = fopen(path, "wb");
    if (fs_file ==  NULL){
        printf("Could not open the file\n");
        printf("Exiting...\n");
        exit(-1);}

    // Extend the file to max_size
    if (ftruncate(fileno(fs_file), fs.sb.max_size) != 0){
        printf("Extend file to max_size failed\n");
        printf("Exiting...\n");
        exit(-1);}

    // Write superblock
    if (fwrite(&fs.sb, sizeof(SuperBlock), 1, fs_file) !=  1){
        printf("Write SuperBlock failed\n");
        printf("Exiting...\n");
        exit(-1);}

    // Write directories
    if (fs.sb.directory_array_size > 0){
        for (int i=0; i<fs.sb.directory_array_size; i++){
            Directory dir = fs.directory_array[i];

            // Write directory
            if (fwrite(&dir, sizeof(Directory), 1, fs_file) !=  1){
                printf("Write directory failed\n");
                printf("Exiting...\n");
                exit(-1);}
        }
    }

    // Write files
    if (fs.sb.file_number > 0){
        for (int j=0; j<fs.sb.file_number; j++){
            File file = fs.file_array[j];

            // Write file's inode
            if (fwrite(&file.inode, sizeof(Inode), 1, fs_file) !=  1){
                printf("Write file inode failed\n");
                printf("Exiting...\n");
                exit(-1);}

            // Write file's bytes
            if (fwrite(file.bytes, file.inode.size, 1, fs_file) !=  1){
                printf("Write file bytes failed\n");
                printf("Exiting...\n");
                exit(-1);}
        }
    }
    
    if (fclose(fs_file) != 0){
        printf("Close file failed\n");
        printf("Exiting...\n");
        exit(-1);}

    return 0;
}

void free_FS(FileSystem fs){
    free(fs.directory_array);
    free(fs.file_array);
}

FileSystem create_dir_from_path(FileSystem fs, char * destination_path){   
    // Find the file parent_id
    char * dir_path = extract_dir_path(destination_path);
    long int dir_id = find_dir_from_path(fs, dir_path);

    if (dir_id == -1){
        // If the directory doesn't exist
        Splitted_path splitted_dir = split_path(dir_path);
        long int current_parent_id = 0; // Root's directory id

        // We change directory until we reach the one we need to create
        for (int i=0; i<splitted_dir.number; i++){
            char * dir_name = splitted_dir.components[i];
            long int dir_id = find_directory(fs, dir_name, current_parent_id);

            if (dir_id == -1){
                // If the intermediate directories don't exist, create it
                fs = add_directory(fs, dir_name, current_parent_id);
                current_parent_id = fs.sb.directory_array_size-1;
            } else {
                // Else, the directory exists, then we go inside
                current_parent_id = dir_id;
            }
        }
        
        free_splitted(splitted_dir);
    }

    free(dir_path);
    return fs;
}

File get_file(FileSystem fs, char * input_path, char * destination_path){
    // Read the file
    FILE * input_file;
    
    input_file = fopen(input_path, "rb");
    if (input_file ==  NULL){
        printf("Could not open the file\n");
        printf("Exiting...\n");
        exit(-1);}

    // Get file size
    if (fseek(input_file, 0, SEEK_END) != 0){
        printf("Could not move cursor in the file\n");
        printf("Exiting...\n");
        exit(-1);}

    unsigned long int input_file_size = ftell(input_file);
    if (input_file_size == -1L){
        printf("Could not get the file size\n");
        printf("Exiting...\n");
        exit(-1);}

    if (fseek(input_file, 0, SEEK_SET) != 0){
        printf("Could not move cursor in the file\n");
        printf("Exiting...\n");
        exit(-1);}

    // Store the file in the file_array
    File file;

    // Generate file's inode
    file.inode.size = input_file_size;
    Splitted_path splitted = split_path(destination_path);
    char * filename = splitted.components[splitted.number-1];
    if (strcpy(file.inode.name, filename) == NULL){
        printf("Copy string failed\n");
        printf("Exiting...\n");
        exit(-1);}
    
    // Find the parent_id of the file
    char * dir_path = extract_dir_path(destination_path);
    long int dir_id = find_dir_from_path(fs, dir_path);
    if (dir_id == -1){
        printf("Directory not found\n");
        printf("Exiting...\n");
        exit(-1);}
    file.inode.parent_id = dir_id;

    // Store file's bytes in a buffer
    file.bytes = malloc(file.inode.size);
    if (fread(file.bytes, input_file_size, 1, input_file) != 1){
        printf("Read file bytes failed\n");
        printf("Exiting...\n");
        exit(-1);}

    if (fclose(input_file) != 0){
        printf("Close file failed\n");
        printf("Exiting...\n");
        exit(-1);}

    // Free memory
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
        // If there was one file: replace it
        fs.file_array = malloc(1*sizeof(File));
        fs.file_array[0] = file;
    } else {
        // Else: increase the array to store it
        fs.file_array = (File*)realloc(fs.file_array, sizeof(File)*fs.sb.file_number);
        fs.file_array[fs.sb.file_number-1] = file;
    }

    return fs;
}

FileSystem rm_file(FileSystem fs, int i){
    // Get the file from the file_array
    File file = fs.file_array[i];

    // Update superblock
    fs.sb.file_number -= 1;
    fs.sb.current_size -= sizeof(Inode) + file.inode.size;

    // Update file_array
    if (fs.sb.file_number == 0){
        // If there is no file: re-initialize inode_array and bytes_array
        fs.file_array = malloc(0*sizeof(File));

    } else {
        // Else shift every file to fill the gap
        for (int k=i; k<fs.sb.file_number; k++){
            fs.file_array[k] = fs.file_array[k+1];
        }

        // Reallocate to remove the empty cell
        fs.file_array = (File*)realloc(fs.file_array, sizeof(File)*fs.sb.file_number);
    }

    return fs;
}

Splitted_path split_path(char * path){
    // Initialize the struct
    Splitted_path splitted;
    splitted.number = 0;

    // Copy path into a modifiable array
    char path_const[strlen(path)];
    strcpy(path_const, path);

    // If the path is only root "/"
    if (strcmp(path, "/")==0){
        splitted.components = calloc(1,sizeof(char*));
        splitted.components[0] = "/";
        return splitted;
    }
    
    // Count the number of components in the path
    for (int k=0; k<strlen(path_const); k++){
        if (path_const[k] == '/'){
            splitted.number++;
        }
    }
    
    // String cursor
    int i = 0;

    // Get components of the path
    char *p = strtok (path_const, "/");
    splitted.components = calloc(splitted.number,sizeof(char*));

    // Add components to the components array
    while (p != NULL){
        splitted.components[i] = malloc(WORD_SIZE*sizeof(char));
        if (strcpy(splitted.components[i], p) == NULL){
            printf("Copy string failed\n");
            printf("Exiting...\n");
            exit(-1);}
        i++;

        // Next component
        p = strtok(NULL, "/");
    }

    return splitted;   
}

void free_splitted(Splitted_path splitted){
    for (int i=0; i<splitted.number; i++){
        free(splitted.components[i]);
    }
    return;
}

char * extract_dir_path(char * file_path){
    // String cursor
    int i = 0;

    // Find the last slash
    int last_slash;
    while (file_path[i] != '\0'){
        if (file_path[i] == '/'){
            last_slash = i;
        }
        i++;
    }

    // Store the dir_path
    char * dir_path;
    char * return_pointer;
    if (last_slash == 0){
        // If the only slash is the root's one
        dir_path = malloc(2*sizeof(char));
        return_pointer = strcpy(dir_path, "/");
    } else {
        // Else it's not the root
        dir_path = malloc(last_slash*sizeof(char));
        return_pointer = strncpy(dir_path, file_path, last_slash);
        dir_path[last_slash] = '\0';
    }

    // Check if strcpy or strncpy failed
    if (return_pointer == NULL){
        return NULL;
    } else {
        return dir_path;
    }
}


long int find_directory(FileSystem fs, char * name, long int parent_id){
    for (int i=0; i<fs.sb.directory_array_size; i++){
        Directory dir = fs.directory_array[i];

        if (strncmp(dir.name, name, strlen(name)) == 0 && dir.parent_id == parent_id){
            return i;
        }
    }
    return -1;
};


long int find_dir_from_path(FileSystem fs, char * dir_path){    
    Splitted_path splitted = split_path(dir_path);

    // Set the current parent id to the root (every path should start with the root)
    long int current_parent_id = 0;

    // Go down in directory level
    for (int k=0; k<splitted.number; k++){
        // Find the next directory
        long int dir_id = find_directory(fs, splitted.components[k], current_parent_id);

        if (dir_id == -1){
            // If the directory doesn't exist
            return -1;
        } else {
            // Else we change directory 
            current_parent_id = dir_id;
        }
    }

    // Free splitted memory
    free_splitted(splitted);

    // At the end, current_parent_id is the id of the directory we are searching for
    return current_parent_id;
}


long int find_file_from_path(FileSystem fs, char * file_path){
    // Default value set to -1 (Not found)
    int return_value = -1;

    Splitted_path splitted = split_path(file_path);
    char * file_name = splitted.components[splitted.number-1];

    // Check if the parent directory exists
    char * dir_path = extract_dir_path(file_path);
    long int parent_id = find_dir_from_path(fs, dir_path);
    if (parent_id == -1){
        // Parent directory doesn't exist
        return_value = -1;
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
Dir_children get_dir_children(FileSystem fs, long int dir_id){
    Dir_children dc;
    dc.children_number=0;

    for (int i=0; i<fs.sb.directory_array_size;i++){ 
        // If the parent of the directory matches
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
        // If the parent of the file matches
        if (fs.file_array[i].inode.parent_id==dir_id){
            df.file_number++; 
            df.file_ids[df.file_number-1]=i;
        }
    }
    return df;
}

FileSystem add_directory(FileSystem fs, char * name, long int parent_id){
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
    if (strcpy(fs.directory_array[dir_id].name, "TRASHED") == NULL){
        printf("Copy string failed\n");
        printf("Exiting...\n");
        exit(-1);}
    fs.directory_array[dir_id].parent_id=-1;

    // Update the superblock
    fs.sb.directory_number = fs.sb.directory_number-1;
    // !!! The array size does NOT change
    fs.sb.directory_array_size = fs.sb.directory_array_size; 
    fs.sb.current_size -= sizeof(Directory);
    return fs;
}

int num_of_files(FileSystem fs, long int dir_id){
    int num_files=0;
    for (int i=0;i<fs.sb.file_number;i++){
        if (fs.file_array[i].inode.parent_id == dir_id){
                num_files++;
            }
        }
    return num_files;
}


long int size_dir_files(FileSystem fs, int dir_id){
    if (num_of_files(fs,dir_id)==0){
        return 0;
    }

    long int size = 0;
    for (int i=0;i<fs.sb.file_number;i++){
        if (fs.file_array[i].inode.parent_id==dir_id){
            size += fs.file_array[i].inode.size;
        }
    }
    return size;
}


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


long int convert_size(long int size, char * size_unit){
    float number;
    if (strncmp(size_unit,"-b",2)==0){
        number = size;

    } else if (strncmp(size_unit,"-k",2)==0){
        strcpy(size_unit,"KB");
        number=(float)size/(1e3);

    } else if (strncmp(size_unit,"-m",2)==0){
        strcpy(size_unit,"MB");
        number=(float)size/(1e6);

    } else if (strncmp(size_unit,"-g",2)==0){
        strcpy(size_unit,"GB");
        number=(float)size/(1e9);

    }
    return number;
}
