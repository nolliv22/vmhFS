int myFS_create(int size){       // size in MB
    FileSystem fs;
    fs.sb.max_size = size*1024*1024;
    fs.sb.inode_number =  0;
    fs.sb.current_size = sizeof(SuperBlock);

    FILE * file;
    file = fopen(PATH, "wb");
    // ftruncate(fileno(file), size*1024*1024); Disabled only during testing
    fwrite(&fs.sb, sizeof(SuperBlock), 1, file);
    fclose(file);

    printf("FileSystem created\n");
    printf("-----------------------------------------------\n");
    printf("FileSystem size: %ld bytes\n", fs.sb.current_size);
    printf("FileSystem max size: %ld bytes\n", fs.sb.max_size);
    printf("Inodes: %ld\n", fs.sb.inode_number);
    printf("-----------------------------------------------\n");

    return 0;
}