#include <stdio.h>
#include <stdlib.h>


int tagFree = 0, tagAlloc = 0;


// Structure for free list 
struct FreeNode
{
    struct FreeNode *next, *prev;
    int size, freeTag;
};


// Structure for allocated list 
struct AllocNode
{
    struct AllocNode *next, *prev;
    int size, allocTag, freeId;
};

typedef struct
{
    struct FreeNode *head, *tail;
}Free;

typedef struct
{
    struct AllocNode *head, *tail;
}Alloc;


// Function to create free 
// list with given sizes 
void create_free(int size, Free *freeBody)
{
    struct FreeNode *node = (struct FreeNode*)malloc(sizeof(struct FreeNode));
    node->freeTag = tagFree;
    node->size = size;
    
    if(freeBody->head == NULL)
	{
        freeBody->tail = node;
        freeBody->head = node;
    }
	else
	{
        node->prev = freeBody->tail;
        (freeBody->tail)->next = node;
        freeBody->tail = node;
    }
    
    tagFree++;
}


// Function to allocate memory to 
// blocks as per First fit algorithm 
void create_alloc(int size, Alloc *allocBody, Free *freeBody)
{
	// create node for process of given size 
    
	struct AllocNode* node = (struct AllocNode*)malloc(sizeof(struct AllocNode));
    node->size = size;
    node->allocTag = tagAlloc;
    struct FreeNode* temp = freeBody->head;
    
    // Iterate to find first memory 
	// block with appropriate size 
	
    while(temp != NULL)
	{
        if(temp->size >= size)
		{
            break;
        }
        temp = temp->next;
    }
    
	// Node found to allocate 
    
    if(temp!=NULL)
    {
    	// Adding node to allocated list
    	
    	temp->size = temp->size-size;
  		node->freeId = temp->freeTag;
    	if(allocBody->head == NULL)
		{
        	allocBody->head = node;
        	allocBody->tail = node;
    	}
		else
		{
        	node->prev = allocBody->tail;
        	(allocBody->tail)->next = node;
        	allocBody->tail = node;
    	}
    
    	tagAlloc++;
	}
    else
	{
        printf("Block of size %d cannot be allocated on the heap\n\n", size);
        return;
    }    
}


// Function to delete node from 
// allocated list to free some space 
void deleteAllocSpace(int tag_id, Alloc *allocBody, Free *freeBody)
{
	// Standard delete function 
	// of a doubly linked list node
	
    struct AllocNode *temp = allocBody->head;
    // First, find the node according 
	// to given tag id 
	
    while(temp != NULL)
	{
        if(temp->allocTag == tag_id)
		{
            break;
        }
        temp = temp->next;
    }
    
    if(temp == NULL)
	{
        printf("Tag ID doesn't exist\n\n");
        return;
    }
    
    int freeId = temp->freeId;
    
    if(temp == allocBody->head)
	{
        allocBody->head = temp->next;
        temp->next = NULL;
        if(allocBody->head == NULL)
		{
            allocBody->tail = NULL;
        }
		else
		{
            (allocBody->head)->prev = NULL;
        }
    }
	else if(temp == allocBody->tail)
	{
        allocBody->tail = temp->prev;
        (allocBody->tail)->next = NULL;
    }
	else
	{
        (temp->prev)->next = temp->next;
        (temp->next)->prev = temp->prev;
        temp->next = NULL;
        temp->prev = NULL;
    }
    
    struct FreeNode *freeTemp = freeBody->head;
    while(freeTemp != NULL)
	{
        if(freeTemp->freeTag == freeId)
		{
            freeTemp->size += temp->size;
            break;
        }
        freeTemp = freeTemp->next;
    }
    
    printf("Alloc Block with Tag %d has been deleted\n\n",tag_id);
    
    free(temp);
}


// Function to print free list which 
// prints free blocks of given sizes 
void printFreeList(Free *freeBody){
    struct FreeNode *temp = freeBody->head;
    if(temp == NULL)
	{
        printf("Free Block List is Empty\n\n");
        return;
    }
        
    printf("Printing Free Blocks List: \n");
    printf("Tag\tSize\n");
    while(temp != NULL)
	{
        printf("%d\t%d\n",temp->freeTag, temp->size);
        temp = temp->next;
    }
    printf("\n");
}


// Function to print allocated list which 
// prints allocated blocks and their block ids 
void printAllocList(Alloc *allocBody){
    struct AllocNode *temp = allocBody->head;
    if(temp == NULL)
        return;
    
    printf("Printing Allocated Blocks List: \n");
    printf("Tag\tId\tSize\n");
    while(temp != NULL){
        printf("%d\t%d\t%d\n", temp->allocTag, temp->freeId, temp->size);
        temp = temp->next;
    }
    printf("\n");
}


//Function to initalize the list to NULL
void initialize(Free *freeBody, Alloc *allocBody){
    freeBody->head = NULL;
    freeBody->tail = NULL;
    allocBody->head = NULL;
    allocBody->tail = NULL;
}


int main(){
    Free freeBody;
    Alloc allocBody;
    
    initialize(&freeBody, &allocBody);
    int freeBlockSizes[] = {100, 500, 200,600};
    int allocProcessSizes[] = {212,417, 112, 426,};
    
    int n = sizeof(freeBlockSizes)/sizeof(freeBlockSizes[0]);
    int m = sizeof(allocProcessSizes)/sizeof(allocProcessSizes[0]);
    int i;
    for(i=0;i<n;i++)
	{
        create_free(freeBlockSizes[i], &freeBody);
    }
    
    printFreeList(&freeBody);
    
    for(i=0;i<m;i++)
	{
        create_alloc(allocProcessSizes[i], &allocBody, &freeBody);
    }
    
    printAllocList(&allocBody);
    
    printFreeList(&freeBody);
    
    int tag_id;
    printf("\n Enter the tag id that u want to delete\n");
    scanf("%d",&tag_id);
    deleteAllocSpace(tag_id, &allocBody, &freeBody);
    
    create_alloc(426, &allocBody, &freeBody);
    
    printAllocList(&allocBody);
    
    printFreeList(&freeBody);
}

