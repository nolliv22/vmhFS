#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int x;

char **path_array ;
int count=0;
char ** split_path(char * path) {
    char PATH[strlen(path)];
    strcpy(PATH, path); 

    
    for (int k=0; k<strlen(PATH); k++){
        if (PATH[k] == '/'){
            count++;
        }
    }
  
    int i = 0;
    char *p = strtok (PATH, "/");
    path_array = calloc(count,sizeof(char*));

    while (p != NULL) 
    {
        path_array[i] = malloc(20*sizeof(char));
        strcpy(path_array[i],p);
        i++;
        p = strtok (NULL, "/");
    }
    x=i;
    
    return path_array;   
}

int main()
{ 
    char * path_array1="/dir1/dir2/dir3/dir4";
    char ** splitted=NULL;
    splitted=split_path(path_array1);
    for(int j=0;j<x;j++){
     printf("-- %s \n",splitted[j]);
    }
    for(int j=0;j<x;j++)
    {
     free(path_array[j]);
    }
    free(path_array);

    return 0;
}