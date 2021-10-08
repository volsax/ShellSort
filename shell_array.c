#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "shell_array.h"

long *Array_Load_From_File(char *filename, int *size){
    FILE *fp;
    char *content;
    long fileSize = 0;

    //open file for the reading
    fp = fopen(filename, "rb");
    if (fp==NULL) {
        fclose(fp);
        return NULL;
    }

    //find the size of the data
    fseek(fp, 0, SEEK_END); //move to the end of the file
    fileSize = ftell(fp);      //return end postion
    rewind(fp);             //back to the head of the file

    //read the data from file to content
    content = (char *)malloc(fileSize); //Assign memory to foe the file
    if(content == NULL){
        fclose(fp);
        return NULL;
    }

    //Start reading
    *size = (fread(content, 1, fileSize, fp))/8;
    fclose(fp);
    return (long*)content;
}

int Array_Save_To_File(char *filename, long *array, int size){
    FILE *fp;
    int count;

    fp = fopen(filename, "wb");
    count = fwrite(array, sizeof(long), size, fp);
    fclose(fp);
    return count;
}

void Array_Shellsort(long *array, int size, long *n_comp){
    long interval = 1;
    long insertValue;
    int i, j;

    //find the interval (k)
    while(interval <= (size/3)){
        interval = interval * 3 + 1;
    }

     //Start sorting
    while(interval){
        
        for (j = interval; j < size; j++){
            insertValue = array[j];
            i = j;

            while((i >= interval) && (array[i-interval] > insertValue)){
                array[i] = array[i-interval];
                i = i - interval;
                *(n_comp) += 1;
            }

            array[i] = insertValue;
        }

        //reduce the interval (k)
        interval = (interval-1)/3;
    }
    
}
