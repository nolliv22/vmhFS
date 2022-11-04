int myFS_create(int size){       // size in megabytes
    // Test
    char * bytes = "123456789azertyuiop";

    Inode inode;
    inode.file = false;
    inode.nth = 1;
    inode.size = strlen(bytes);
    strcpy(inode.name, "testfile");
    strcpy(inode.parent_directory, "/");

    Superblock sb;
    sb.max_size = size*1024*1024;
    sb.inode_number =  0;
    sb.current_size = sizeof(sb) + sizeof(inode) + inode.size;

    FILE * fs_file;
    fs_file = fopen(PATH, "wb");
    fwrite(&sb, sizeof(sb), 1, fs_file);
    fwrite(&inode, sizeof(inode), 1, fs_file);
    fwrite(bytes, inode.size, 1, fs_file);
    fclose(fs_file);

    printf("Filesystem size: %ld\n", sb.current_size);
    printf("Byte size: %ld\n", inode.size);
    printf("Name: %s\n", inode.name);
    printf("Bytes: %s\n", bytes);

    printf("Successfully created a vhm file system to %s with a maximum size of %d MB\n", PATH, size);

    return 0;
}