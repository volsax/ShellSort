#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "shell_list.h"
#include "list_of_list.h"
int swap = 1;
long count = 0;


static Node *insertNode(Node *cur, long x){
    //x is the value of node
    Node *temp = NULL;
    temp = (Node *)malloc(sizeof(Node));
    temp -> value = x;
    temp -> next = NULL;
    cur -> next = temp;
    cur = cur -> next;
    return cur;
}

static long getSize(Node *list){
    long size = 0;
    Node *cur = list;
    while(cur != NULL){
        size += 1;
        cur = cur -> next;
    }
    return size;
}


Node *List_Load_From_File(char *filename){
    FILE *fp;
    Node *head  = NULL;
    Node *cur = NULL;
    int countLoad = 2;
    long val;

    //Open the file
    fp = fopen(filename, "rb");
    if (fp==NULL) {
        fclose(fp);
        return NULL;
    }

    //Read the first element
    if(fread(&val, sizeof(long), 1, fp)){
        head = (Node *)malloc(sizeof(Node));
        head -> next = NULL;
        head -> value = val;
    }

    //read the second element
    if(fread(&val, sizeof(long), 1, fp)){
        cur = (Node *)malloc(sizeof(Node));
        head -> next = cur;
        cur -> value = val;
        cur -> next = NULL;
    }
    //if there is no second element return and free
    else{
        free(cur);
        fclose(fp);
        return head;
    }

    //start reading file (cur will always be the last element)
    while (fread(&val, sizeof(val), 1, fp)){
        cur = insertNode(cur, val);
        countLoad ++;
    }
    printf("Element load from file: %d\n", countLoad);
    fclose(fp);
    return head;
}

int List_Save_To_File(char *filename, Node *list){
    FILE *fp;
    long val = 0;
    int countSave = 0;
    Node *cur = list;

    fp = fopen(filename, "wb");
    if(fp == NULL){
        fclose(fp);
        return NULL;
    }

    //Start writing data to file
    while(cur != NULL){
        val = cur -> value;
        fwrite(&val, sizeof(val), 1, fp);
        cur = cur -> next;
        countSave += 1;
    }

    //Close and return the count
    fclose(fp);
    return countSave;
}

//The function to swap node when gap == 1
static Node *swapNode(Node* prev1){
    Node *p2 = NULL, *p1 = NULL, *temp = NULL;
    p1 = prev1 -> next;
    p2 = p1 -> next;
    if(p1 -> value > p2 -> value){
        count++;
        swap = 1;
        p2 = p1 -> next;
        p1 = prev1 -> next;
        p1 -> next = p2 -> next;
        p2 -> next = p1;
        prev1 -> next = p2;
        temp = p2;
    }
    else temp = p1;
    return temp;
}

static Node *moveNode(Node *start, long k){
    int i;
    for(i=0; i<k; i++){
        start = start -> next;
    }
    return start;
}

//The function to swap node when gap > 1
static Node *swapGapNodeP2(Node *temp, long k){
    Node *next1 = NULL, *p1 = NULL, *p2 = NULL, *prev2 = NULL, *prev1 = NULL;
    prev1 = temp;
    p1 = prev1 -> next;
    next1 = p1 -> next;
    prev2 = moveNode(prev1, k);
    p2 = prev2 -> next;

    //Start swap
    if(p1 -> value > p2 -> value){
        count++;
        swap = 1;
        prev1 -> next =  p2;
        prev2 -> next =  p1;
        p1 -> next = p2 -> next;
        p2 -> next = next1;
        temp = prev1;
    }
    else temp = prev2;
    return temp;
}


Node *List_Shellsort(Node *list, long *n_comp){
    long interval = 1;
    int i,j;
    Node *head = list;
    Node  prevHead = {0, list};
    Node *temp = NULL;

    //get the size of the link list
    long size = getSize(head);
    //printf("\nGet size return size: %ld\n", size);

    //calculate the k
    while(interval <= (size/3)){
        interval = interval * 3 + 1;
    }
    //interval = 1;
    //Start sorting
    while(interval){

       if(interval > 1){ 
            //Start sorting using bubble sort
            for(i=0; i<size-interval; i++){
                swap = 0;
                temp = &prevHead;
                for(j=i; j<size-interval; j+=interval){
                    temp = swapGapNodeP2(temp, interval);
                }
                if(!swap) break;              
            }
        }
        //Bubble sort when interval == 1
        if(interval == 1){
            for(i=0; i<size-interval; i++){
                swap = 0;
                temp = &prevHead;
                for(j=0; j<size-interval; j+=interval){
                    temp = swapNode(temp);
                }
                if(!swap) break;              
            }
        }
        //reduce the interval (k)
        interval = (interval-1)/3;
    }
    *n_comp = count;
    printf("List Compare: %ld\n", *n_comp);
    return prevHead.next;
}
