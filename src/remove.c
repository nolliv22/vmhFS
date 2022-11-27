int myFS_remove(char * path){
    FileSystem fs = get_FS(PATH);

    int return_value = -1;
    long int file_id = find_file_from_path(fs, path);
    long int dir_id = find_dir_from_path(fs, path);

    if (file_id != -1){
        fs = rm_file(fs, file_id);
        printf("File %s has been remove from the file system\n", path);
        return_value = 0;

    } else if (dir_id != -1){
        Dir_files df = get_dir_files(fs, dir_id);
        Dir_children dc = get_dir_children(fs, dir_id);

        if (df.file_number > 0 || dc.children_number > 0){
            printf("Directory is not empty\n");
            return_value = -1;
        } else {
            fs = rm_directory(fs, dir_id);
            printf("Directory %s has been remove from the file system\n", path);
            return_value = 0;
        }
    } else {
        printf("There is not a file/directory at this path\n");
        return_value = -1;
    }

    put_FS(PATH, fs);

    // Disabled because causes segmentation fault
    // free_FS(fs);
    return return_value;
}