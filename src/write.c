int myFS_write(char * input_path, char * destination_path){
    FileSystem fs = get_FS(PATH);

    File file = get_file(fs, input_path, destination_path);

    // Check file size, if not ok, remove the oldest file
    // To store a new file we need to check if the number of bytes of the file plus the size of one inode can fit
    if (fs.sb.current_size + file.inode.size + sizeof(Inode) <= fs.sb.max_size ){
        fs = add_file(fs, file);
    } else {
        if (fs.sb.file_number == 1){
            fs.file_array[0] = file;
        } else {
            int i = 0; // i is the number of file to be removed
            // temp_size is the size after adding the file
            unsigned long int temp_size = fs.sb.current_size + file.inode.size + sizeof(Inode);
            while (temp_size > fs.sb.max_size){
                temp_size = temp_size - fs.file_array[i].inode.size;
                i++;
            }

            fs.sb.current_size = temp_size;
            fs.sb.file_number = fs.sb.file_number-i;

            for (int k=0; k<fs.sb.file_number; k++){
                fs.file_array[k] = fs.file_array[k+i];
            }

            fs.file_array = (File*)realloc(fs.file_array, sizeof(File)*fs.sb.file_number);

            fs = add_file(fs, file);
        }
    }

    put_FS(PATH, fs);

    int i = fs.sb.file_number-1;
    printf("Write file %s to filesystem %s:\n", input_path, PATH);
    printf("-----------------------------------------------\n");
    printf("Size: %ld B\n", fs.file_array[i].inode.size);
    printf("Name: %s\n", fs.file_array[i].inode.name);
    printf("Parent: %ld\n", fs.file_array[i].inode.parent_id);
    printf("-----------------------------------------------\n");

    free(file.bytes);
    free_FS(fs);
    return 0;
}