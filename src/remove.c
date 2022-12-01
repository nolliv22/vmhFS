int myFS_remove(char * path){
    // get the file system
    FileSystem fs = get_FS(PATH);

    int return_value = -1;
    
    // Check all the case: if the path is a file path or directory path
    long int file_id = find_file_from_path(fs, path);
    long int dir_id = find_dir_from_path(fs, path);

    // if it's a file path
    if (file_id != -1){
        // Remove the file
        fs = rm_file(fs, file_id);
        printf("File %s has been remove from the file system\n", path);
        return_value = 0;

    // Else if it's a directory path
    } else if (dir_id != -1){
        Dir_files df = get_dir_files(fs, dir_id);
        Dir_children dc = get_dir_children(fs, dir_id);

        // if the directory is empty
        if (df.file_number > 0 || dc.children_number > 0){
            printf("Directory is not empty\n");
            return_value = -1;

        // Else it's empty
        } else {
            // Remove the file
            fs = rm_directory(fs, dir_id);
            printf("Directory %s has been remove from the file system\n", path);
            return_value = 0;
        }
    } else {
        printf("There is not a file/directory at this path\n");
        return_value = -1;
    }

    if (put_FS(PATH, fs) != 0){
        printf("Store file system failed\n");
        printf("Exiting...\n");
        exit(-1);}

    // Disabled because causes segmentation fault
    // free_FS(fs);
    return return_value;
}