#include "shell_array.h"
#include "shell_list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]){
    long n_comp = 0;
    Node *head = NULL;

    if(strcmp("-l", argv[1])==0){
        head = List_Load_From_File(argv[2]);
        if(head == NULL){
            printf("NO this file");
            return EXIT_FAILURE;
        }

        head = List_Shellsort(head, &n_comp);

        int b = List_Save_To_File(argv[3], head);
        printf("Element save to file: %d\n", b);
    }
    
    else if(strcmp("-a", argv[1])==0){
        long *array = NULL;
        int size = 0;

        array = Array_Load_From_File(argv[2], &size);

        //Check if the array is NULL
        if (array == NULL){
            return EXIT_FAILURE;
        }

        Array_Shellsort(array, size, &n_comp);
        printf("Array compare: %ld", n_comp);
        int a = Array_Save_To_File(argv[3], array, size);
        free(array);

    }
    return 0;
}
