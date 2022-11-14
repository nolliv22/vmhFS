int myFS_write(char * input_path, char * destination_path){
    FileSystem fs = get_FS(PATH);

    File file = getFile(input_path, destination_path);

    // Check file size, if not ok, remove the oldest file
    // To store a new file we need to check if the number of bytes of the file plus the size of one inode can fit
    if (fs.sb.current_size + file.inode.size + sizeof(Inode) <= fs.sb.max_size ){
        fs = add_inode(fs, file.inode, file.bytes);
    } else {
        if (fs.sb.inode_number == 1){
            fs.inode_array[0] = file.inode;
            fs.bytes_array[0] = file.bytes;
        } else {
            int i = 0; // i is the number of file to be removed
            // temp_size is the size after adding the file
            unsigned long int temp_size = fs.sb.current_size + file.inode.size + sizeof(Inode);
            while (temp_size > fs.sb.max_size){
                temp_size = temp_size - fs.inode_array[i].size;
                i++;
            }

            fs.sb.current_size = temp_size;
            fs.sb.inode_number = fs.sb.inode_number-i;

            for (int k=0; k<fs.sb.inode_number; k++){
                fs.bytes_array[k] = fs.bytes_array[k+i];
                fs.inode_array[k] = fs.inode_array[k+i];
            }

            fs.inode_array = (Inode*)realloc(fs.inode_array, sizeof(Inode)*fs.sb.inode_number);
            fs.bytes_array = (char **)realloc(fs.bytes_array, sizeof(char*)*fs.sb.inode_number);

            fs = add_inode(fs, file.inode, file.bytes);
        }

    }

    put_FS(PATH, fs);

    int i = fs.sb.inode_number-1;
    printf("Write file %s to filesystem %s:\n", input_path, PATH);
    printf("-----------------------------------------------\n");
    printf("Size: %ld B\n", fs.inode_array[i].size);
    printf("Name: %s\n", fs.inode_array[i].name);
    printf("Parent: %ld\n", fs.inode_array[i].parent_id);
    printf("-----------------------------------------------\n");

    free(file.bytes);
    free_FS(fs);
    return 0;
}