int myFS_write(char * input_path, char * destination_path){
    FileSystem fs = get_FS(PATH);

    // Check if a file exists already at this path
    long int file_id = find_file_from_path(fs, destination_path);
    if (file_id != -1){
        printf("There is already a file at the path %s\n", destination_path);
        free_FS(fs);
        return -1;
    }
    
    // Generate File struct from file
    fs = create_dir_from_path(fs, destination_path);
    File file = get_file(fs, input_path, destination_path);
    
    // If the file size is greater than the file system max size
    if (file.inode.size > fs.sb.max_size/2){
        printf("The file is greater than the maximum allowed size (%.2f MB)\n", fs.sb.max_size/1e6/2);
        free_FS(fs);
        return -1;
    }
    
    // Check file size, if not ok, remove the oldest file
    // To store a new file we need to check if the number of bytes of the file plus the size of one inode can fit
    // If there is enought space without needing to remove old files
    if (fs.sb.current_size + file.inode.size + sizeof(Inode) <= fs.sb.max_size ){
        fs = add_file(fs, file);
    // Need to remove old files to add the new file
    } else {
        if (fs.sb.file_number == 1){
            fs = rm_file(fs, 0);
            fs = add_file(fs, file);
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

            // Shift files to fill the gap from the removed file
            for (int k=0; k<fs.sb.file_number; k++){
                fs.file_array[k] = fs.file_array[k+i];
            }
            
            fs.file_array = (File*)realloc(fs.file_array, sizeof(File)*fs.sb.file_number);

            fs = add_file(fs, file);
        }
    }
    
    if (put_FS(PATH, fs) != 0){
        printf("Store file system failed\n");
        printf("Exiting...\n");
        exit(-1);}

    printf("Write file %s to filesystem at: %s\n", input_path, destination_path);

    // Free memory
    free_FS(fs);
    return 0;
}