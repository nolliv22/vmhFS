int test(){
    FileSystem fs;

    fs.sb.directory_number = 3;
    fs.directory_array = malloc(sizeof(Directory)*fs.sb.directory_number);

    Directory root;
    strcpy(root.name, "/");
    root.parent_id = 0;    
    fs.directory_array[0] = root;

    Directory dir1;
    strcpy(dir1.name, "dir1");
    dir1.parent_id = 0;
    fs.directory_array[1] = dir1;

    Directory dir2;
    strcpy(dir2.name, "dir2");
    dir2.parent_id = 1;
    fs.directory_array[2] = dir2;

    fs.sb.inode_number =  2;

    Inode foo1;
    strcpy(foo1.name, "foo1");
    foo1.parent_id = 1;
    
    Inode foo2;
    strcpy(foo2.name, "foo2");
    foo2.parent_id = 2;

    fs.inode_array = malloc(sizeof(Inode)*fs.sb.inode_number);
    fs.inode_array[0] = foo1;
    fs.inode_array[1] = foo2;

    // printf("%d\n", find_file(fs, "/dir1/foo1"));
    // printf("%d\n", find_file(fs, "/dir2/foo1"));
    // printf("%d\n", find_file(fs, "/dir1/foo2"));
    // printf("%d\n", find_file(fs, "/dir2/foo2"));

    char ** splitted = split_path("/dir1/dir2/foo1");

    printf("%s\n", splitted[0]);
    printf("%s\n", splitted[1]);
    printf("%s\n", splitted[2]);

    free(splitted);

    return 0;
}