int myFS_read(char * file_path){
    FileSystem fs = get_FS(PATH);

    // Find the file in the file system
    int file_id = find_file_from_path(fs, file_path);

    if (file_id == -1){
        // File doesn't exist at this path
        printf("File doesn't exist\n");
        return -1;
    } else {
        // Print the content of the file
        File file = fs.file_array[file_id];
        printf("%s", file.bytes);
    }

    // Free memory
    free_FS(fs);
    return 0;
}