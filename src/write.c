int myFS_write(char * input_path, char * destination_path){
    FileSystem fs = get_FS(PATH);

    FILE * input_file;
    input_file = fopen(input_path, "rb");

    fseek(input_file, 0, SEEK_END);
    unsigned long int input_file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    Inode inode;
    inode.file = true;
    inode.nth = fs.sb.inode_number + 1;
    inode.size = input_file_size;
    strcpy(inode.name, destination_path);         // Need to split the path
    strcpy(inode.parent_directory, "/");    // Need to get the parent directory
                                            // Need to create recursively parent direcory to the root

    char * bytes = malloc(inode.size);
    fread(bytes, input_file_size, 1, input_file);

    fs.sb.inode_number += 1;
    fs.sb.current_size += sizeof(Inode) + inode.size;

    fs.inode_array = (Inode*)realloc(fs.inode_array, sizeof(Inode)*fs.sb.inode_number);
    fs.bytes_array = (char **)realloc(fs.bytes_array, sizeof(char*)*fs.sb.inode_number);

    fs.inode_array[fs.sb.inode_number-1] = inode;
    fs.bytes_array[fs.sb.inode_number-1] = bytes;

    put_FS(PATH, fs);

    free_FS(fs);
    return 0;
}