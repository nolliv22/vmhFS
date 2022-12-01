/*
    Compute recursively size of file inside directory
    INPUT:
        FileSystem fs: file system in which we find files
        long int dir_id: the start directory index in the directory_array
    OUTPUT:
        long int size: Total size

*/
long int size_accumulator(FileSystem fs, int dir_id);

int myFS_size(bool r, char *size_unit, bool stat, char *path_to_dir){
    // Get the file system
    FileSystem fs = get_FS(PATH);

    // Check if the directory exists
    long int dir_index = find_dir_from_path(fs,path_to_dir);
    if(dir_index==-1){
        printf("Directory doesn't exist\n");
        return -1;
    }

    // If -stat flag is provided
    if (stat==true){    
        long int used_storage = 0;
        printf("Current size of the file system %ld\n",fs.sb.current_size);
        
        // Compute size of all file
        for(int j=0;j<fs.sb.file_number;j++){
           used_storage += fs.file_array[j].inode.size;
        }
        printf("Total size of the files: %ld\n", used_storage);

        // Compute ratio
        float ratio = (float)used_storage/fs.sb.current_size;
        printf("Ratio Total files/Total filesystem: %.5f\n", ratio);
    }

    long int size;
    // Handle -r flag
    if (r==false){
        // Not recusive
        size = size_dir_files(fs,dir_index);
        float converted_size = convert_size(size, size_unit);
        size_unit = extract_size_unit(size_unit);
        printf("Total size of files in %s: %.0f %s [Not recursive]\n", path_to_dir, converted_size, size_unit);

    } else {
        // Recursive
        size = size_accumulator(fs,dir_index);
        float converted_size = convert_size(size,size_unit);
        size_unit=extract_size_unit(size_unit);
        printf("Total size of files in %s: %.0f %s [Recursive]\n", path_to_dir, converted_size, size_unit);
    }
    
    // Free memory
    free_FS(fs);
    return 0;

}

long int size_accumulator(FileSystem fs, int dir_id){
    Dir_children dc = get_dir_children(fs, dir_id);

    // Stop condition: no child directories in the directory
    if (dc.children_ids == 0){  
        return size_dir_files(fs, dir_id);
    } else {
        // Compute the total size inside the direcory
        long int size = size_dir_files(fs, dir_id);

        // Compute the total size for each child directory
        for(int l=0; l<dc.children_number; l++){
            size += size_accumulator(fs, dc.children_ids[l]);
        }
        return size;
    }
}