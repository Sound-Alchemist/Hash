#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hashMap.h"

struct hashLink {
   KeyType key; /*the key is what you use to look up a hashLink*/
   ValueType value; /*the value stored with the hashLink, a pointer to int in the case of concordance*/
   struct hashLink * next; /*notice how these are like linked list nodes*/
};
typedef struct hashLink hashLink;

struct hashMap {
    hashLink ** table; /*array of pointers to hashLinks*/
    int tableSize; /*number of buckets in the table*/
    int count; /*number of hashLinks in the table*/
};
typedef struct hashMap hashMap;

/*the first hashing function you can use*/
int stringHash1(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += str[i];
	return r;
}

/*the second hashing function you can use*/
int stringHash2(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += (i+1) * str[i]; /*the difference between stringHash1 and stringHash2 is on this line*/
	return r;
}

/* initialize the supplied hashMap struct*/
void _initMap (struct hashMap * ht, int tableSize)
{
	int index;
	if(ht == NULL)
		return;
	ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
	ht->tableSize = tableSize;
	ht->count = 0;
	for(index = 0; index < tableSize; index++)
		ht->table[index] = NULL;
}

/* allocate memory and initialize a hash map*/
hashMap *createMap(int tableSize) {
	assert(tableSize > 0);
	hashMap *ht;
	ht = malloc(sizeof(hashMap));
	assert(ht != 0);
	_initMap(ht, tableSize);
	return ht;
}

void _freeMap (struct hashMap * ht)
{  
	struct hashLink* next; //placeholder for the next node
    struct hashLink* cur; //placeholder for the current node
        for(int i=0; i < ht->tableSize; i++){
                cur = ht->table[i]; // assign the current node the pointer
                while(cur !=0){
                        next = cur->next; // move to next node
                        free(cur); //free current one
                        cur = next; // set cur pointer
                }
                ht->table[i] = 0; //clear value
        }
        free(ht->table); //free table
        ht->tableSize = 0; //reset size
        ht->count = 0;	 //reset node counter
}

void deleteMap(hashMap *ht) {
	assert(ht!= 0);
	/* Free all memory used by the buckets */
	_freeMap(ht);
	/* free the hashMap struct */
	free(ht);
}


void _setTableSize(struct hashMap * ht, int newTableSize)
{
	int curr_cap = ht->tableSize;
	hashLink* curr_map = ht->table;
	ht->table = malloc(newTableSize*sizeof(hashMap));
	ht->tableSize=newTableSize;
	//not quite done yet.... needs to move other nodes over
	//but how??
}

void insertMap (struct hashMap * ht, KeyType k, ValueType v)
{  
	    int i=0; //placeholder for the index in the hash table
        struct hashLink* newNode = malloc(sizeof(struct hashLink));
      
		//assignments
        newNode->next = 0;
        newNode->key = k;
        newNode->value = v;
        
		//here, we check to see it it contains the key already given KeyType k
        if(containsKey(ht, k)==1){
                removeKey(ht, k);
        }

		//create the hash        
        i = stringHash1(k) % ht->tableSize;
        
		//resize if necessary, given hash value from previous assignment of i
        if(i < 0){
                _setTableSize(ht,ht->tableSize*2);
        }

        //find the empty key set, insert first node as newNode
		if(ht->table[i]==0){
                ht->table[i] = newNode;
        } else {
			//move along the chain, insert at the end
                struct hashLink* cur = ht->table[i];
				//we need a new cur node to hold the spot in the map to link on
                while(cur->next!=NULL){
                        cur = cur->next;
                }
				//assign the last one newNode in the chain
                cur->next = newNode;
        }
		//increase size
        ht->count++;
}

ValueType atMap (struct hashMap * ht, KeyType k)
{ 
    
	int i = stringHash1(k) % ht->tableSize; //placeholder for the map finder
    
	//resize if necessary for the node to search the map    
	if(i < 0){
               _setTableSize(ht,ht->tableSize*2);
        }

        struct hashLink* cur = ht->table[i]; //a new current node for search
        
        while(strcmp(cur->key, k) != 0){ //compares the two strings, returns 0 if equal
                cur = cur->next;//if not equal, advance along the chain
        }
    return &cur->value; //return type for void* as defined with cur->value, gives the value member
}

int containsKey (struct hashMap * ht, KeyType k)
{  
	int i = stringHash1(k) % ht->tableSize; //placeholder for the map finder
	
	//resize if necessary for the node to search the map
        if(i < 0){
                _setTableSize(ht,ht->tableSize*2);
        }
        
        struct hashLink* cur = ht->table[i]; //a new current node for search
        
        while(cur){ //no specific condition, just any "yes" value will work -there's a lot of them!-
                if(strcmp(cur->key, k) == 0){ //if we find it, return yes (1)
                        return 1;
                }
                cur = cur->next; // advance along the chain
        }
    return 0; // if not found at all
}

void removeKey (struct hashMap * ht, KeyType k)
{  
	    struct hashLink* cur;
        struct hashLink* pre = NULL;
        
        int idx = stringHash1(k) % ht->tableSize;
        if(idx < 0){
                idx += ht->tableSize;
        }
        cur = ht->table[idx];
        while(strcmp(cur->key, k) != 0){
                pre = cur;
                cur = cur->next;
        }
        if(pre){
                pre->next = cur->next;
        } else {
                ht->table[idx] = 0;
        }
        free(cur);
}

int size (struct hashMap *ht)
{  
  return ht->count; 
}


int capacity(struct hashMap *ht)
{  
return ht->tableSize;
}


int emptyBuckets(struct hashMap *ht)
{  
        int i = 0; //number of buckets with no values
        for(int x = 0; x < ht->tableSize; x++){ //iterate through table
                if(ht->table[x]==0){ //if value is 0, add one to number of empty buckets i
                        i++;
                }
        }
    return i; //return number of empty buckets
}

float tableLoad(struct hashMap *ht)
{  
  return ht->count / ht->tableSize;
}

/* print the hashMap */
void printMap (struct hashMap * ht)
{
	int i;
	struct hashLink *temp;	
	for(i = 0;i < capacity(ht); i++){
		temp = ht->table[i];
		if(temp != 0) {		
			printf("\nBucket Index %d -> ", i);		
		}
		while(temp != 0){			
			printf("Key:%s|", temp->key);
			printValue(temp->value);
			printf(" -> ");
			temp=temp->next;			
		}		
	}
}