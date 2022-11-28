// TESTING FILESYSTEM BY ADDING MANUALLY ELEMENT IF THE
// FUNCTION NEEDED IS NOT FINISHED YET
int test(){

    // // DIRECTORY
    // fs.sb.directory_number = 0;
    // fs.directory_array = malloc(sizeof(Directory)*fs.sb.directory_number);

    // fs = add_directory(fs, "root", 0);
    // fs = add_directory(fs, "dir1", 0);
    // fs = add_directory(fs, "dir2", 1);

    // // Find dir from path
    // printf("Find dir from path:\n");
    // printf("%ld\n", find_dir_from_path(fs, "/dir1"));
    // printf("%ld\n", find_dir_from_path(fs, "/dir2"));
    // printf("%ld\n", find_dir_from_path(fs, "/dir1/dir2"));

    // // Find dir with name and parent_id
    // printf("Find dir:\n");
    // printf("%ld\n", find_directory(fs, "dir1", 0));
    // printf("%ld\n", find_directory(fs, "dir2", 0));
    // printf("%ld\n", find_directory(fs, "dir1", 1));
    // printf("%ld\n", find_directory(fs, "dir2", 1));

    // // FILE
    // fs.sb.file_number = 0;
    // fs.file_array = malloc(sizeof(File)*fs.sb.file_number);

    // File foo0;
    // strcpy(foo0.inode.name, "foo0");
    // foo0.inode.parent_id = 0;
    // foo0.bytes = "foo0";

    // File foo1;
    // strcpy(foo1.inode.name, "foo1");
    // foo1.inode.parent_id = 1;
    // foo1.bytes = "foo1";
    
    // File foo2;
    // strcpy(foo2.inode.name, "foo2");
    // foo2.inode.parent_id = 2;
    // foo2.bytes = "foo2";

    // fs = add_file(fs, foo0);
    // fs = add_file(fs, foo1);
    // fs = add_file(fs, foo2);

    // printf("Find file:\n");
    // printf("%ld\n", find_file(fs, "/foo0"));
    // printf("%ld\n", find_file(fs, "/dir1/foo0"));

    // printf("%ld\n", find_file(fs, "/dir1/foo1"));
    // printf("%ld\n", find_file(fs, "/dir1/dir2/foo1"));
    
    //printf("%ld\n", find_file(fs, "/dir1/foo2"));
    //printf("%ld\n", find_file(fs, "/dir1/dir2/foo2"));
    // dir_children DC;
    // DC=get_dir_children(fs,0);
    // printf("%ld",fs.sb.directory_number);

    // free_FS(fs);
    return 0;
}
