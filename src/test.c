int test(){
    FileSystem fs;

    fs.sb.directory_number = 1;
    fs.directory_array = malloc(sizeof(Directory)*fs.sb.directory_number);

    Directory root;
    strcpy(root.name, "/");
    root.parent_id = 0;    
    fs.directory_array[0] = root;

    fs = add_directory(fs, "dir1", 0);
    fs = add_directory(fs, "dir2", 1);

    fs.sb.inode_number = 2;

    Inode foo1;
    strcpy(foo1.name, "foo1");
    foo1.parent_id = 1;
    
    Inode foo2;
    strcpy(foo2.name, "foo2");
    foo2.parent_id = 2;

    fs.inode_array = malloc(sizeof(Inode)*fs.sb.inode_number);
    fs.inode_array[0] = foo1;
    fs.inode_array[1] = foo2;

    printf("%d\n", find_file(fs, "/dir1/foo1"));
    printf("%d\n", find_file(fs, "/dir2/foo1"));
    printf("%d\n", find_file(fs, "/dir1/foo2"));
    printf("%d\n", find_file(fs, "/dir1/dir2/foo2"));

    // free_FS(fs);
    return 0;
}
