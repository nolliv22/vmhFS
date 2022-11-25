int myFS_ls(char * path, bool r, bool d){
    FileSystem fs = get_FS(PATH);
    long int dir_index = find_dir_from_path(fs,path);
    if (fs.sb.file_number > 0){
        if(r == false){
            if(dir_index == -1){
                printf("directory don't exist");
            }
            else{
                for(int i = 0; i< fs.sb.file_number; i++){
                    if(fs.file_array[i].inode.parent_id == dir_index){
                        printf("%s %dMB",fs.file_array[i].inode.name,fs.file_array[i].inode.size/1000);
                    }
                }
            }
        }
        else{ //files inside this directory
             for(int i = 0; i< fs.sb.file_number; i++){
                    if(fs.file_array[i].inode.parent_id == dir_index){
                        printf("%s %dMB",fs.file_array[i].inode.name,fs.file_array[i].inode.size/1000);
            myFS_ls_recur(fs,dir_index); // files inside its children directory
                    }
                }
        }
    }

    else {
        printf("File system has no files\n");
    }
    // Free memory
    free_FS(fs);
    return 0;
}