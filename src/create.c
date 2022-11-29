int myFS_create(int size){       // size in MB
    // Initialize file system
    FileSystem fs;
    fs.directory_array = malloc(sizeof(Directory)*1);
    fs.file_array = malloc(sizeof(File)*1);
    
    // Initialize superblock
    fs.sb.max_size = size*1024*1024;
    fs.sb.directory_number = 0;
    fs.sb.directory_array_size = 0;
    fs.sb.file_number =  0;
    fs.sb.current_size = sizeof(SuperBlock);

    // Initialize root directory
    fs.directory_array = malloc(sizeof(Directory)*1);
    fs.file_array = malloc(sizeof(File)*1);
    fs = add_directory(fs, "/", -2);
    
    // Store the file system to the disk
    put_FS(PATH, fs);
    // FILE * fs_file;
    // file = fopen(PATH, "wb");
    // // ftruncate(fileno(file), size*1024*1024); Disabled only during testing
    // fwrite(&fs.sb, sizeof(SuperBlock), 1, fs_file);

    printf("FileSystem created\n");
    printf("-----------------------------------------------\n");
    printf("FileSystem size: %ld bytes\n", fs.sb.current_size);
    printf("FileSystem max size: %ld bytes\n", fs.sb.max_size);
    printf("Files: %ld\n", fs.sb.file_number);
    printf("-----------------------------------------------\n");

    // Free memory
    free_FS(fs);
    return 0;
}