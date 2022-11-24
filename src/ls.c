int myFS_ls(){
    FileSystem fs = get_FS(PATH);

    printf("FileSystem size: %ld B\n", fs.sb.current_size);
    printf("FileSystem max size: %ld B\n", fs.sb.max_size);
    printf("Inodes: %ld\n", fs.sb.inode_number);
    printf("-----------------------------------------------\n");

    if (fs.sb.inode_number > 0){
        for (int i=0; i<fs.sb.inode_number; i++){
            printf("Size: %ld B\n", fs.inode_array[i].size);
            printf("Name: %s\n", fs.inode_array[i].name);
            printf("Parent: %ld\n", fs.inode_array[i].parent_id);
            printf("-----------------------------------------------\n");
        }
    } else {
        printf("Empty\n");
    }

    free_FS(fs);
    return 0;
}