int myFS_read(char * file_path){
    FileSystem fs = get_FS(PATH);

    int file_id = find_file_from_path(fs, file_path);

    if (file_id == -1){
        printf("File doesn't exist\n");
        return -1;
    } else {
        File file = fs.file_array[file_id];
        printf("%s", file.bytes);
    }

    // Free memory
    free_FS(fs);
    return 0;
}