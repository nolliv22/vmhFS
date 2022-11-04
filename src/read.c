int myFS_read(char * input_path, char * destination_path){
    Superblock sb;
    Inode inode;

    FILE * fs_file;
    fs_file = fopen(PATH, "rb");
    fread(&sb, sizeof(sb), 1, fs_file);
    fread(&inode, sizeof(inode), 1, fs_file);

    char * bytes = malloc(inode.size);
    fread(bytes, inode.size, 1, fs_file);
    fclose(fs_file);

    printf("Filesystem size: %ld\n", sb.current_size);
    printf("Byte size: %ld\n", inode.size);
    printf("Name: %s\n", inode.name);
    printf("Bytes: %s\n", bytes);

    return 0;
}