int ls_accumulator(FileSystem fs, long int dir_id, int level, int max_level);

int myFS_ls(char * dir_path, bool r){
    FileSystem fs = get_FS(PATH);

    long int dir_id = find_dir_from_path(fs, dir_path);

    if (dir_id == -1){
        printf("Directory not found\n");
        return -1;
    }

    int return_value;

    printf("List segment:\n");
    printf("%s\n", fs.directory_array[dir_id].name);
    if (!r){
        return_value = ls_accumulator(fs, dir_id, 0, 0);
    } else {
        return_value = ls_accumulator(fs, dir_id, 0, 256);
    }    

    // Free memory
    free_FS(fs);
    return return_value;
}

int ls_accumulator(FileSystem fs, long int dir_id, int level, int max_level){
    int return_value = 0;
    
    if (dir_id == -1){
        printf("The specified path does not refer to a existing directory in the file system\n");
        return -1;
    }

    Dir_files df = get_dir_files(fs, dir_id);
    if (df.file_number > 0){
        for (int i=0; i<df.file_number; i++){

            for (int i_=0; i_<level*4; i_++){
                printf(" ");
            }
            printf("└──");
            printf("%s\n", fs.file_array[i].inode.name);
        }
    }

    Dir_children dc = get_dir_children(fs, dir_id);
    if (dc.children_number > 0){
        for (int j=0; j<dc.children_number; j++){
            long int child_id = dc.children_ids[j]; 

            for (int j_=0; j_<level*4; j_++){
                printf(" ");
            }
            printf("└──");
            printf("%s\n", fs.directory_array[child_id].name);

            if (level < max_level){
                return_value += ls_accumulator(fs, child_id, level+1, max_level);
            }
        }
    }

    return return_value;
}