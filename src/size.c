int myFS_size(bool r, char *size_unit,bool stat,char *path_to_dir)
{
    FileSystem fs = get_FS(PATH);
    long int dir_index=find_dir_from_path(fs,path_to_dir);
    long int size;
    char* dir_name = fs.directory_array[dir_index].name;
    if(dir_index==-1)
    {
        printf("Directory doesn't exist");
        return -1;
    }

    else if (r==false)
    {
    size=size_dir_files(fs,dir_index);
    //Make the necessary conversions in size according to size unit
    printf("Total size of files in %s: %ld %s\n",dir_name,size,size_unit);
    }
    else
    {
    size=myFS_size_recur(fs,dir_index);
    printf("Total size of files in %s: %ld %s\n",dir_name,size,size_unit);
    }
    return 0;
}
  