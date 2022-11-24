#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char **components;
    int number;
} Path;

Path split_path(char * input_path) {
    Path path;
    path.components = malloc(1*sizeof(char*));

    if (input_path[0] != '/'){
        printf("Path must start with /");
        exit(1);
    }

    int i = 0;
    path.number = 0;

    while (input_path[i] != '\0'){
        if (input_path[i] == '/'){
            i++;
            path.number++;
            path.components = (char**)realloc(path.components, path.number*sizeof(char*));
            path.components[path.number-1] = malloc(32*sizeof(char));

        } else {
            strncat(path.components[path.number-1], &input_path[i], 1);
            i++;
        }
    }

    return path;   
}

int main(){
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    for (int i=0; i<100000; i++){
        Path path = split_path("/dir/foo/bruh/anc");
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC / 100000;

    printf("%f\n", cpu_time_used);

    Path path = split_path("/dir/foo/bruh/anc");
    for (int i=0; i<path.number; i++){
        printf("%s\n", path.components[i]);
    }

    return 0; 
}