int myFS_create(int size){       
    // Create a file system of size MB at the specified path
    // INPUT:
    //      int size: size of the file system in MB
    // OUTPUT:
    //      int r: 0 

    // Initialize file system
    FileSystem fs;
    fs.directory_array = malloc(sizeof(Directory)*1);
    fs.file_array = malloc(sizeof(File)*1);
    
    // Initialize superblock
    fs.sb.max_size = size*1000*1000;
    fs.sb.directory_number = 0;
    fs.sb.directory_array_size = 0;
    fs.sb.file_number =  0;
    fs.sb.current_size = sizeof(SuperBlock);

    // Initialize file_array and directory_array
    fs.directory_array = malloc(sizeof(Directory)*1);
    fs.file_array = malloc(sizeof(File)*1);

    // Add root directory
    fs = add_directory(fs, "/", -2);
    
    // Store the file system to the disk
    if (put_FS(PATH, fs) != 0){
        printf("Store file system failed\n");
        printf("Exiting...\n");
        exit(-1);}

    printf("FileSystem created\n");
    printf("Size: %ld B\n", fs.sb.current_size);
    printf("Max size: %ld B\n", fs.sb.max_size);
    printf("Files: %ld\n", fs.sb.file_number);
    printf("Directories: %ld\n", fs.sb.directory_number);

    // Free memory
    free_FS(fs);
    return 0;
}