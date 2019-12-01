/*
 * BSP Stuff, ideally from a header
 * + OS stuff if they exist
 */
#define RAM_START 0x1000
#define RAM_SIZE 0x1000
#define NULL 0x0000

#if defined( DEBUG )
	#include <stdlib.h>
	#include <stdio.h>
	char* dbg_heap = NULL;
	#define HEAP_START dbg_heap
	#define PRINT( STR ) printf("%s\n", STR)
#else
	#define HEAP_START RAM_START
	#define PRINT( STR )
#endif

/* 
 * -- Super Simple Heap Manager --
 * Uses inplace memory management with metadata contained
 * in the heap itself. 
 * Uses Lazy allocation 
 * Splits blocks during allocation if size is great
 */

struct __attribute__((__packed__)) block{
	int size;
	struct block* next;
};
typedef struct block block_t;
static int overhead = sizeof(block_t);

// Null will mean that there's been no blocks allocated
static block_t* free_list = NULL;

// Honestly, should be called right after c runtime is up
void init_heap(){
#if defined( DEBUG )
	dbg_heap = malloc(RAM_SIZE);
#endif
	// Insert the initial block into the freelist
	free_list = (void*)( HEAP_START );
	free_list->size = RAM_SIZE - overhead;
	free_list->next = NULL;
}

void my_free( void* ptr ){
	if( ptr == NULL ) return;

	block_t* block = (block_t*)( (char*)ptr - sizeof(block_t) );
	block->next = NULL;

	block_t* tail;
	for( tail = free_list; tail != NULL && tail->next != NULL; tail = tail->next );

	// We happen to have no free_list
	if( free_list == NULL )
		free_list = block;
	else // insert into tail of the free list
		tail->next = block;
}

void* my_malloc( int size ){
	if( free_list == NULL ){
		PRINT("HEAP NOT INITIALZED");
		return NULL;
	}

	// Remove block from free list
	// Note: char* casting is needed, to not jump
	// sizeof(block_t) when performing offset
	block_t* ptr;
	block_t* parent;
	for( 
		ptr = free_list, parent = NULL;
		ptr->next != NULL;
		parent = ptr, ptr = ptr->next
	){
		if( ( ptr->size == size ) || ( ptr->size > overhead + size ) )
			break;
	}

	// Check which condition occured
	if( ptr->size == size ){
		parent->next = ptr->next;
		return (char*)ptr + overhead;
	}
	else if( ptr->size > ( size + overhead )){
		// Set up the new block
		block_t* block = (block_t*)( (char*)ptr + overhead + size );
		block->size = ptr->size - ( overhead + size );
		block->next = ptr->next;

		if( parent == NULL ) // no parent, dealing with the only block
			free_list = block;
		else // parent exists, and is valid
			parent->next = block;
		
		// Finish up by modifying current ptr to match
		// the slice operation
		ptr->size = size;
		return (char*)ptr + overhead;
	}
	else if( ptr->next == NULL ){
		// We went through the entire list
		// Trigger a defrag TODO
	}
	PRINT("NO BLOCK FOUND");
	return NULL;
}

int main(int argc, char** argv){
#if defined( DEBUG )	
	// Should be called after crt is set up
	init_heap();
#endif
	// A quick Test of the malloc and free :)
	const int array_size = 1000;
	char* char_arry = my_malloc(array_size);
	for( int i = 0; i < array_size; i++ )
		char_arry[i] = 'F';
	char_arry[array_size-1] = '\0';
	my_free( char_arry );

	return 0;
}
