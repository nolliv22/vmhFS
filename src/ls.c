int myFS_ls(){
    FileSystem fs = get_FS(PATH);

    printf("FileSystem size: %ld B\n", fs.sb.current_size);
    printf("FileSystem max size: %ld B\n", fs.sb.max_size);
    printf("Files: %ld\n", fs.sb.file_number);
    printf("-----------------------------------------------\n");

    // For debugging only

    printf("FILES\n");
    if (fs.sb.file_number > 0){
        for (int i=0; i<fs.sb.file_number; i++){
            File file = fs.file_array[i];
            printf("Size: %ld B\n", file.inode.size);
            printf("Name: %s\n", file.inode.name);
            printf("Parent: %ld\n", file.inode.parent_id);
            printf("-----------------------------------------------\n");
        }
    } else {
        printf("No file\n");
    }

    printf("DIRECTORIES\n");
    for (int j=0; j<fs.sb.directory_number; j++){
        Directory dir = fs.directory_array[j];
        printf("Name: %s\n", dir.name);
        printf("Parent: %ld\n", dir.parent_id);
        printf("-----------------------------------------------\n");
    }

    // Free memory
    free_FS(fs);
    return 0;
}