int myFS_read(char * file_path){
    FileSystem fs = get_FS(PATH);

    int file_id = find_file(fs, file_path);

    if (file_id == -1){
        printf("File doesn't exist");
        return -1;
    } else {
        printf("%s", fs.bytes_array[file_id]);
    }

    // Free memory
    free_FS(fs);
    return 0;
}