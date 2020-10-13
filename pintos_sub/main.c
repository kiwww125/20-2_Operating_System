#include "bitmap.h"
#include "list.h"
#include "hash.h"
#include "round.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTOKENLEN 60

int tokenSz;
char* cmdToken[7];
struct list *LIST[10];
struct hash *HASH[10];
struct bitmap *BM[10];

static void parse_cmd(char* cmd){
    
    tokenSz = 0;
    char *ptr = strtok(cmd, " ");     
    while(ptr!=NULL){
        strcpy(cmdToken[tokenSz++], ptr);
        ptr = strtok(NULL, " ");
    }
}

int main(){

    //init for tokenizing
    char *cmd = malloc(sizeof(char) * MAXTOKENLEN);
    for(int i = 0; i < 7; i++)
        cmdToken[i] = malloc(sizeof(char) * MAXTOKENLEN);
    
    while(1)
    {
        fgets(cmd, MAXTOKENLEN, stdin);
        cmd[strlen(cmd) - 1] = '\0'; 
        parse_cmd(cmd);

        if(strcmp(cmdToken[0], "quit") == 0) break;
        if(strcmp(cmdToken[0], "create") == 0)
        {
            //list create
            if(strcmp(cmdToken[1], "list") == 0)
            {
                LIST[cmdToken[2][4] - '0'] = malloc(sizeof(struct list));
                list_init(LIST[cmdToken[2][4] - '0']);
            }

            //hash create
            if(strcmp(cmdToken[1], "hashtable") == 0)
            {
                HASH[cmdToken[2][4]- '0'] = malloc(sizeof (struct hash));
                hash_init(HASH[cmdToken[2][4]-'0'], hash_func, hash_cmp, (void*)0);    
            }
            
            //bitamp create
            if(strcmp(cmdToken[1], "bitmap") == 0){
                size_t bit_cnt;
                sscanf(cmdToken[3], "%zu", &bit_cnt);
                BM[cmdToken[2][2] - '0'] = bitmap_create(bit_cnt); 
            }
        }

        if(strcmp(cmdToken[0],"dumpdata") == 0)
        {
            if(strncmp(cmdToken[1], "list", 4) == 0)
            {
                struct list* tList= LIST[cmdToken[1][4] -'0']; 
                if(tList == NULL) 
                {
                    printf("Cannot exec below command(init list first))\n%s", cmd);
                }
                else
                {
                    struct list_elem *e;
                    for (e = list_begin (tList); e != list_end (tList); e = list_next (e))
                        printf("%d ", (list_entry(e, struct list_item, elem))->data);
                    if(!list_empty(tList)) printf("\n");
                }
            }

            if(strncmp(cmdToken[1], "hash", 4) == 0)
            {
                struct hash* tHash= HASH[cmdToken[1][4] -'0']; 
                struct hash_iterator i;
                hash_first(&i, tHash);

                while (hash_next (&i))
                {
                    struct hash_item *cur = hash_entry (hash_cur(&i), struct hash_item, elem);
                    printf("%d ", cur->value);
                }
                if(!hash_empty(tHash)) printf("\n");
            }

            if(strncmp(cmdToken[1], "bm", 2) == 0){
                struct bitmap* bm = BM[cmdToken[1][2] -'0']; 
                size_t bsz = bitmap_size(bm);
                
                for(int idx =0; idx < bsz; idx++)
                    printf("%d", bitmap_test(bm, idx) ? 1 : 0); 
                printf("\n");
            }
        }

        if(strcmp(cmdToken[0], "delete") == 0){
            if(strncmp(cmdToken[1], "list", 4) == 0)
            {
                struct list *tList= LIST[cmdToken[1][4] -'0']; 
                struct list_elem *e;
                if(tList == NULL) 
                {
                    printf("Cannot exec below command(init list first))\n%s", cmd);
                }
                for (e = list_begin (tList); e != list_end (tList); e = list_remove (e))
                {}
            }

            if(strncmp(cmdToken[1], "hash", 4) == 0)
            {
                struct hash* tHash = HASH[cmdToken[1][4] -'0'];
                hash_clear(tHash, hash_destructor);
            }
            // if(strncmp(cmdToken[1], "bitmap", 6) == 0){
            // }

        }

        //list opertaion block
        if(strncmp(cmdToken[0], "list", 4) == 0)
        {
            
            struct list *tList= LIST[cmdToken[1][4] -'0']; 
            struct list_elem *e = malloc(sizeof(struct list_elem));
            if(tList == NULL) 
            {
                printf("Cannot exec below command(init list first))\n%s", cmd);
                continue;
            }

            if(strcmp(cmdToken[0], "list_push_back") == 0)
            {
                list_entry(e, struct list_item, elem)->data = atoi(cmdToken[2]);
                list_push_back(tList, e);
            }

            if(strcmp(cmdToken[0], "list_push_front") == 0)
            {
                list_entry(e, struct list_item, elem)->data = atoi(cmdToken[2]);
                list_push_front(tList, e);   
            }
            
            if(strcmp(cmdToken[0], "list_pop_back") == 0)
            {
                if(list_empty(tList)){
                    printf("%s is empty.\n", cmdToken[1]);
                }
                else
                {
                    list_pop_back(tList);
                }
            }
            
            if(strcmp(cmdToken[0], "list_pop_front") == 0)
            {
                if(list_empty(tList)){
                    printf("%s is empty.\n", cmdToken[1]);
                }
                else
                {
                    list_pop_front(tList);
                }
            }

            if(strcmp(cmdToken[0], "list_front") == 0)
            {
                printf("%d\n", list_entry(list_front(tList), struct list_item, elem)->data);
            }

            if(strcmp(cmdToken[0], "list_back") == 0)
            {
                printf("%d\n", list_entry(list_back(tList), struct list_item, elem)->data);
            }

            if(strcmp(cmdToken[0], "list_insert") == 0)
            {
                list_entry(e, struct list_item, elem)->data = atoi(cmdToken[3]);
                struct list_elem * before = list_begin(tList);
                int ind = atoi(cmdToken[2]);
                for(int i = 0; i < ind; i++) 
                    before = list_next(before);
                list_insert(before, e);
            }

            if(strcmp(cmdToken[0], "list_insert_ordered") == 0)
            {
                list_entry(e, struct list_item, elem)->data = atoi(cmdToken[2]);
                list_insert_ordered(tList, e , cmp, (void*)0);
            }

            if(strcmp(cmdToken[0], "list_min") == 0)
            {
                printf("%d\n", list_entry(list_min(tList, cmp, (void*)0), struct list_item, elem)->data);
            }
            
            if(strcmp(cmdToken[0], "list_max") == 0)
            {
                printf("%d\n", list_entry(list_max(tList, cmp, (void*)0), struct list_item, elem)->data);
            }

            if(strcmp(cmdToken[0], "list_empty") == 0)
            {
                printf("%s\n", list_empty(tList) ? "true" : "false");
            }
            
            if(strcmp(cmdToken[0], "list_size") == 0)
            {
                printf("%zu\n", list_size(tList));
            }
            
            if(strcmp(cmdToken[0], "list_remove") == 0)
            {
                e = list_begin(tList);
                int ind = atoi(cmdToken[2]);
                for(int i = 0; i < ind; i++) 
                    e = list_next(e);
                list_remove(e);
            }

            if(strcmp(cmdToken[0], "list_reverse") == 0)
            {
                list_reverse(tList);
            }

            if(strcmp(cmdToken[0], "list_sort") == 0)
            {
                list_sort(tList, cmp, (void*)0);
            }

            if(strcmp(cmdToken[0], "list_splice") == 0)
            {
                int ind;
                struct list_elem *before, *first, *last;
                
                before = list_begin(tList);
                ind = atoi(cmdToken[2]);
                for(int i = 0; i < ind;i++)
                    before = list_next(before);

                first = last = list_begin(LIST[cmdToken[3][4] -'0']);
                ind = atoi(cmdToken[4]);
                for(int i = 0; i < ind;i++)
                    first = list_next(first);

                ind = atoi(cmdToken[5]);
                for(int i =0 ; i< ind ;i++)
                    last = list_next(last);

                list_splice(before, first, last);
            }
            
            if(strcmp(cmdToken[0], "list_swap") == 0)
            {
                int lind = atoi(cmdToken[2]);
                int rind = atoi(cmdToken[3]);
                if(lind > rind) {
                    int t = lind;
                    lind = rind;
                    rind = t;
                }
                
                struct list_elem *l, *r;
                l = r = list_begin(tList);

                for(int i =0 ; i<lind;i++)
                    l = list_next(l);
                for(int i =0 ; i<rind;i++)
                    r = list_next(r);
                
                list_swap(l, r);
            }
            
            if(strcmp(cmdToken[0], "list_unique") == 0)
            {
                struct list *dupList = NULL;
                if(tokenSz == 3) {
                    dupList = LIST[cmdToken[2][4] - '0'];
                }
                list_unique(tList, dupList, cmp, (void*)0);
            }
            
            if(strcmp(cmdToken[0], "list_shuffle") == 0)
            {
                list_shuffle(tList);
            }
        }

        //hash functions
        if(strncmp(cmdToken[0], "hash", 4 ) == 0)
        {
            struct hash* tHash = HASH[cmdToken[1][4] -'0'];
            struct hash_elem *h = malloc(sizeof(struct hash_elem));

            if(strcmp(cmdToken[0], "hash_insert") == 0)
            {
                hash_entry(h, struct hash_item, elem)->value = atoi(cmdToken[2]);
                hash_insert(tHash, h);    
            }

            if(strcmp(cmdToken[0], "hash_replace") == 0)
            {
                hash_entry(h, struct hash_item, elem)->value = atoi(cmdToken[2]);
                hash_replace(tHash, h);    
            }

            if(strcmp(cmdToken[0], "hash_find") == 0)
            {
                hash_entry(h, struct hash_item, elem)->value = atoi(cmdToken[2]);
                if(hash_find(tHash, h)) 
                    printf("%d\n", hash_entry(h, struct hash_item, elem)->value);
            }

            if(strcmp(cmdToken[0], "hash_empty") == 0)
            {
                printf("%s\n", hash_empty(tHash) ? "true" : "false");
            }

            if(strcmp(cmdToken[0], "hash_size") == 0)
            {
                printf("%zu\n", hash_size(tHash));
            }

            if(strcmp(cmdToken[0], "hash_clear") == 0)
            {
                hash_clear(tHash, hash_destructor);
            }
            
            if(strcmp(cmdToken[0], "hash_delete") == 0)
            {
                hash_entry(h, struct hash_item, elem)->value = atoi(cmdToken[2]);
                struct hash_elem *del = hash_delete(tHash, h);
                free(del);   
            }

            if(strcmp(cmdToken[0], "hash_apply") == 0)
            {
                if(strcmp(cmdToken[2], "square") == 0)
                {
                    hash_apply(tHash, hash_action_square);
                }
                if(strcmp(cmdToken[2], "triple") == 0)
                {
                    hash_apply(tHash, hash_action_triple);
                }
            }
        }
        
        if(strncmp(cmdToken[0], "bitmap", 6) == 0)
        {
            struct bitmap *bm =BM[cmdToken[1][2] - '0'];
            bool value;
            int exsz;
            size_t start, idx, cnt;

            if(strcmp(cmdToken[0], "bitmap_size") == 0)
            {
                printf("%zu\n", bitmap_size(bm));
            }

            if(strcmp(cmdToken[0], "bitmap_test") == 0)
            {
                sscanf(cmdToken[2], "%zu", &idx);
                printf("%s\n", bitmap_test(bm, idx) ? "true" : "false");
            }
        
            if(strcmp(cmdToken[0], "bitmap_contains") == 0)
            {
                sscanf(cmdToken[2], "%zu", &start);
                sscanf(cmdToken[3], "%zu", &idx);
                value = cmdToken[4][0] == 't' ? true : false;
                printf("%s\n", bitmap_contains(bm, start, idx, value) ? "true" : "false");
            }
            
            if(strcmp(cmdToken[0], "bitmap_count") == 0)
            {
                sscanf(cmdToken[2], "%zu", &start);
                sscanf(cmdToken[3], "%zu", &cnt);
                value = cmdToken[4][0] == 't' ? true : false;
                printf("%zu\n", bitmap_count(bm, start,cnt, value ));
            }

            if(strcmp(cmdToken[0], "bitmap_mark") == 0)
            {
                sscanf(cmdToken[2], "%zu", &idx);
                bitmap_mark(bm, idx);
            }

            if(strcmp(cmdToken[0], "bitmap_dump") == 0)
            {
                bitmap_dump(bm);
            }
            
            if(strcmp(cmdToken[0], "bitmap_all") == 0)
            {
                sscanf(cmdToken[2], "%zu", &start);
                sscanf(cmdToken[3], "%zu", &cnt);
                printf("%s\n", bitmap_all(bm, start, cnt) ? "true" : "false");
            }

            if(strcmp(cmdToken[0], "bitmap_any") == 0)
            {
                sscanf(cmdToken[2], "%zu", &start);
                sscanf(cmdToken[3], "%zu", &cnt);
                printf("%s\n", bitmap_any(bm, start, cnt) ? "true" : "false");
            }
        
            if(strcmp(cmdToken[0], "bitmap_none") == 0)
            {
                sscanf(cmdToken[2], "%zu", &start);
                sscanf(cmdToken[3], "%zu", &cnt);
                printf("%s\n", bitmap_none(bm, start, cnt) ? "true" : "false");
            }

            if(strcmp(cmdToken[0], "bitmap_expand") == 0)
            {
                sscanf(cmdToken[2], "%d", &exsz);
                BM[cmdToken[1][2] - '0'] = bitmap_expand(bm, exsz);
            }

            if(strcmp(cmdToken[0], "bitmap_flip") == 0)
            {
                sscanf(cmdToken[2], "%zu", &idx);
                bitmap_flip(bm, idx);
            }
            
            if(strcmp(cmdToken[0], "bitmap_reset") == 0)
            {
                sscanf(cmdToken[2], "%zu", &idx);
                bitmap_reset(bm, idx);
            }

            if(strcmp(cmdToken[0], "bitmap_scan_and_flip") == 0)
            {
                sscanf(cmdToken[2], "%zu", &start);
                sscanf(cmdToken[3], "%zu", &cnt);
                value = cmdToken[4][0] == 't' ? true : false;
                printf("%zu\n", bitmap_scan_and_flip(bm, start, cnt, value));
            }
             
            if(strcmp(cmdToken[0], "bitmap_scan") == 0)
            {
                sscanf(cmdToken[2], "%zu", &start);
                sscanf(cmdToken[3], "%zu", &cnt);
                value = cmdToken[4][0] == 't' ? true : false;
                printf("%zu\n", bitmap_scan(bm, start, cnt, value));
            }
            
            if(strcmp(cmdToken[0], "bitmap_set_all") == 0)
            {
                value = cmdToken[2][0] == 't' ? true : false;
                bitmap_set_all(bm, value);
            }

            if(strcmp(cmdToken[0], "bitmap_set_multiple") == 0)
            {
                sscanf(cmdToken[2], "%zu", &start);
                sscanf(cmdToken[3], "%zu", &cnt);
                value = cmdToken[4][0] == 't' ? true : false;
                 bitmap_set_multiple(bm, start, cnt, value);
            }
            
            if(strcmp(cmdToken[0], "bitmap_set") == 0)
            {
                sscanf(cmdToken[2], "%zu", &idx);
                value = cmdToken[3][0] == 't' ? true : false;
                bitmap_set(bm, idx, value);
            }
        }
    }

    return 0;
}
