// TESTING FILESYSTEM BY ADDING MANUALLY ELEMENT IF THE
// FUNCTION NEEDED IS NOT FINISHED YET
int test(){
    FileSystem fs;

    // DIRECTORY
    fs.sb.directory_number = 0;
    fs.directory_array = malloc(sizeof(Directory)*fs.sb.directory_number);

    fs = add_directory(fs, "root", 0);
    fs = add_directory(fs, "dir1", 0);
    fs = add_directory(fs, "dir2", 1);

    // Find dir from path
    // printf("Find dir from path:\n");
    // printf("%d\n", find_dir_from_path(fs, "/dir1"));
    // printf("%d\n", find_dir_from_path(fs, "/dir2"));
    // printf("%d\n", find_dir_from_path(fs, "/dir1/dir2"));

    // Find dir with name and parent_id
    // printf("Find dir:\n");
    // printf("%d\n", find_directory(fs, "dir1", 0));
    // printf("%d\n", find_directory(fs, "dir2", 0));
    // printf("%d\n", find_directory(fs, "dir1", 1));
    // printf("%d\n", find_directory(fs, "dir2", 1));

    // FILE
    fs.sb.inode_number = 2;
    fs.inode_array = malloc(sizeof(Inode)*fs.sb.inode_number);
    fs.bytes_array = malloc(sizeof(char*)*fs.sb.inode_number);

    Inode foo1;
    strcpy(foo1.name, "foo1");
    foo1.parent_id = 1;
    char * bytes1 = "foo1";
    
    Inode foo2;
    strcpy(foo2.name, "foo2");
    foo2.parent_id = 2;
    char * bytes2 = "foo2";

    fs = add_inode(fs, foo1, bytes1);
    fs = add_inode(fs, foo2, bytes2);

    printf("Find file:\n"); 
    printf("%d\n", find_file(fs, "/dir1/foo1"));
    printf("%d\n", find_file(fs, "/dir2/foo1"));
    printf("%d\n", find_file(fs, "/dir1/foo2"));
    printf("%d\n", find_file(fs, "/dir1/dir2/foo2"));

    // free_FS(fs);
    return 0;
}
