long int size_accumulator(FileSystem fs, int id);

int myFS_size(bool r, char *size_unit, bool stat, char *path_to_dir){
    FileSystem fs = get_FS(PATH);
    long int dir_index = find_dir_from_path(fs,path_to_dir);

    if(dir_index==-1){
        printf("Directory doesn't exist\n");
        return -1;
    }

    if (stat==true){    
        long int used_storage = 0;
        printf("Current size of the file system %ld\n",fs.sb.current_size);
        
        for(int j=0;j<fs.sb.file_number;j++){
           used_storage += fs.file_array[j].inode.size;
        }
        printf("Total size of the files: %ld\n", used_storage);

        float ratio = (float)used_storage/fs.sb.current_size;
        printf("Bytes used for file data / Total bytes used by the filesystem: %.2f\n", ratio);
    }

    long int size;
    if (r==false){
        size = size_dir_files(fs,dir_index);
        float converted_size = convert_size(size, size_unit);
        size_unit = extract_size_unit(size_unit);
        printf("Total size of files in %s: %.0f %s\n", path_to_dir, converted_size, size_unit);

    } else {
        size = size_accumulator(fs,dir_index);
        float converted_size = convert_size(size,size_unit);
        size_unit=extract_size_unit(size_unit);
        printf("Total size of files in %s: %.0f %s\n", path_to_dir, converted_size, size_unit);
    }
    
    
    free_FS(fs);
    return 0;

}

//Here we implement the recursive version of size
//Our base condition is to stop at the directory with no children 
long int size_accumulator(FileSystem fs, int id){
    Dir_children dc = get_dir_children(fs,id);

    if (dc.children_ids == 0){  
        return size_dir_files(fs, id);
    } else {   
        long int size = size_dir_files(fs, id);
        for(int l=0; l<dc.children_number; l++){
            size += size_accumulator(fs, dc.children_ids[l]);
        }
        return size;
    }
}