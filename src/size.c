int myFS_size(bool r, char *size_unit,bool stat,char *path_to_dir)
{   //printf("size_unit: %s\n",size_unit);
    //printf("path_to_dir: %s\n",path_to_dir);
    FileSystem fs = get_FS(PATH);
    long int dir_index=find_dir_from_path(fs,path_to_dir);
    //printf("dir_index:%ld\n",dir_index);
    long int size;
    //char* dir_name = fs.directory_array[dir_index].name;

    if(dir_index==-1)
    {
        printf("Directory doesn't exist\n");
        return -1;
    }
    if (stat==true)
    {    
        long int used_storage=0;
        printf("current size of the file system %ld\n",fs.sb.current_size);
        for(int j=0;j<fs.sb.file_number;j++)
        {
           used_storage=used_storage+fs.file_array[j].inode.size;
        }
        printf("total size of the files: %ld\n",used_storage);
        float ratio=(float)used_storage/fs.sb.current_size;
        printf(" bytes used for file data / total bytes used the filesystem: %.2f\n",ratio);
    }
    if (r==false)
    {
    size=size_dir_files(fs,dir_index);
    float converted_size=convert_size(size,size_unit);
    size_unit=extract_size_unit(size_unit);
    printf("Total size of files in %s: %.0f %s\n",path_to_dir,converted_size,size_unit);
    }
    else
    {
    size=myFS_size_recur(fs,dir_index);
    float converted_size=convert_size(size,size_unit);
    size_unit=extract_size_unit(size_unit);
    printf("Total size of files in %s: %.0f %s\n",path_to_dir,converted_size,size_unit);
    }
    
    
    free_FS(fs);
    return 0;

}
  