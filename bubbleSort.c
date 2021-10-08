#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "shell_list.h"
#include "list_of_list.h"

Node *List_Load_From_File(char *filename){
    FILE *fp = fopen(filename, "rb");
    if(fp==NULL){
        return NULL;
    }
    Node* headptr = (Node*)malloc(sizeof(Node));
    Node* currentnode = (Node*)malloc(sizeof(Node));
    if(headptr==NULL){
        fclose(fp);
        return NULL;
   }
    int64_t k;
    int element_check;
    element_check = fread(&k, sizeof(int64_t), 1, fp);
    if(element_check==0){
        free(headptr);
        fclose(fp);
        return NULL;
    }
    headptr->next = NULL;
    headptr->value = k;
    currentnode = headptr;
    while(fread(&k, sizeof(int64_t), 1, fp)){
        Node* newnode = (Node*)malloc(sizeof(Node));
        newnode->next = NULL;
        newnode->value = k;
        currentnode->next = newnode;
        currentnode = newnode;
    }
    fclose(fp);
    return headptr;
}


int List_Save_To_File(char *filename, Node *list){
    FILE *fp = fopen(filename, "wb");
    Node* currentnode = list;
    int howmanyelement = 0;
    if(fp==NULL){
        return 0;
    }
    if(list==NULL){
        fclose(fp);
        return 0;
    }
    while(currentnode) {
        howmanyelement += fwrite(&currentnode->value, sizeof(int64_t), 1, fp);
        currentnode = currentnode->next;
    }
    fclose(fp);
    return howmanyelement;
}

Node *movePtr(Node *ptr, int64_t distance){
    int64_t count=0;
    while(ptr && count<distance){
        ptr = ptr->next;
        count++;
    }
    if(ptr==NULL) printf("fail");
    return ptr;
}

Node *swap(Node *temp, int now){
    Node *prev_a=NULL,*now_a=NULL,*next_a=NULL,*prev_b=NULL,*now_b=NULL,*next_b=NULL;
    prev_a = temp;
    now_a = prev_a->next;
    next_a = now_a->next;
    prev_b = movePtr(prev_a, now);
    now_b = prev_b->next;
    //check and swap
    if(now_a->value > now_b->value){
        if(now==1){
            prev_a->next = now_b;
            prev_b->next = now_a;
            now_a->next = now_b->next;
            now_b->next = now_a;
        }
        else{
            prev_a->next = now_b;
            prev_b->next = now_a;
            now_a->next = now_b->next;
            now_b->next = next_a;
        }
        //Move temp to the bigger node
        temp = prev_a;
    }
    //Move temp to the bigger node
    else temp = prev_b;
    return temp;
}

Node *List_Shellsort(Node *list, int64_t *n_comp){
    Node *temp=NULL, *countnode=list;
    Node fakehead = {0, list};
    int64_t i, j, now, size=0;
    int newpostion;
    *n_comp = 0;
    //Find the size of the list
    while(countnode){
        size++;
        countnode = countnode->next;
    }
    //Find the first gap
    for(now=1; now<size; now = now*3 +1);
    now /=3;
    //bubblesort
    while(now){
        for(i=0; i<size-now; i++){
            newpostion = i%now;
            //printf("%d\n", newpostion);
            temp = movePtr(&fakehead, newpostion);
            for(j=newpostion; j<size-now; j+=now){
                    //Place pointer on the two nodes I want to check and their previous and next nodes
                (*n_comp)++;
                temp = swap(temp, now);
            }
        }
        
        //New gap
        now /=3;
    }
    return fakehead.next;
}
