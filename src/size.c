int myFS_size(bool r, char *size_unit,bool stat,char *path_to_dir)
{
    FileSystem fs = get_FS(PATH);
    long int dir_index=find_dir_from_path(fs,path_to_dir);
    long int size;
    char* dir_name = fs.directory_array[dir_index].name;
    size_info SZ;
    SZ.number=0;
    SZ.unit=NULL;
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
        printf(" bytes used for file data / total bytes used the filesystem: %ld\n",used_storage/fs.sb.current_size);
    }
    if (r==false)
    {
    size=size_dir_files(fs,dir_index);
    SZ=convert_size(size,size_unit);
    printf("Total size of files in %s: %ld %s\n",dir_name,SZ.number,SZ.unit);
    }
    else
    {
    size=myFS_size_recur(fs,dir_index);
    SZ=convert_size(size,size_unit);
    printf("Total size of files in %s: %ld %s\n",dir_name,SZ.number,SZ.unit);
    }
    
    
    free_FS(fs);
    return 0;

}
  