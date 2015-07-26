/*
Author: davex
e-mail: efengeler@gmail.com
*/

#include "rewind.h"



int num_rwnd_states = 0;
int rwnd_state_size = 0;
int g_memory_allocated = 0;

struct rewind_state{
	int have_data;
	unsigned char *data;
	struct rewind_state *next;
	struct rewind_state *prev;
};


struct rewind_state *ptr_rewind_states, *prev_state, *next_state;




int get_psp_max_free_memory(void){
	unsigned char *mem;
	const int MEM_CHUNK_SIZE = 500*1024; //blocks of 500KBytes
	const int MAX_CHUNKS = 128;
	unsigned char* mem_reserv[MAX_CHUNKS];
	int total_mem = 0;
	int i = 0;

	//initializes
	for( i = 0; i< MAX_CHUNKS; i++){
		mem_reserv[i] = NULL;
	}

	//allocate
	for( i=0; i<MAX_CHUNKS; i++){
		mem = (unsigned char *)malloc(MEM_CHUNK_SIZE);
		if( mem != NULL){
			total_mem += MEM_CHUNK_SIZE;
			mem_reserv[i] = mem;
		}else{
			break;
		}
	}

	//free
	for( i=0; i<MAX_CHUNKS; i++){
		if( mem_reserv[i] != NULL)
			free( mem_reserv[i] );
        else
            break;
	}

	return total_mem;
}



void allocate_rewind_states(void){
	struct rewind_state *created_state, *first_state;
	int i;

	int total_rewind_memory = (int)( (float)(get_psp_max_free_memory()) * 0.85 ); //reserves 85% of free memory

	rwnd_state_size = get_mem_save_snss_size()+10000;  //declared in "nes/nes.h"
	//ShimaFlareX:
	//It was originally 
	//rwnd_state_size = get_mem_save_snss_size()+100;  //declared in "nes/nes.h"
	//I had to change that to something bigger to fix some issues with Mario Bros Classic, for instance.
	//TODO: Investigate this.

	num_rwnd_states = (int) ( (float)total_rewind_memory / (float) rwnd_state_size );
	if( num_rwnd_states <= 0){
		g_memory_allocated = 0;
		return;
	}else{
		g_memory_allocated = 1;
	}

	//reserves first state
	created_state =  (struct rewind_state *)malloc( sizeof(struct rewind_state) );
	created_state->have_data = 0;
	created_state->data = (byte *) malloc( rwnd_state_size);
	first_state = created_state;
	prev_state = first_state;

	//reserves remaining states
	for( i = 1; i< num_rwnd_states; i++){
		created_state  = (struct rewind_state *)malloc( sizeof(struct rewind_state) );
		created_state->have_data = 0;
		created_state->data = (byte *) malloc( rwnd_state_size);
		created_state ->prev = prev_state;
		prev_state->next = created_state;
		prev_state = created_state;
	}


	//make list be circular
	created_state->next = first_state;
	first_state->prev = created_state;
	ptr_rewind_states = first_state;

}


void free_rewind_states(void){
	if( g_memory_allocated == 0)
		return;

	struct rewind_state *now_state;

	now_state = ptr_rewind_states;
	prev_state = now_state->prev;
	prev_state->next = NULL;

	while(1){
		if ( now_state == NULL)
			break;
		next_state = now_state->next;
		free(now_state->data );
		free(now_state);
		now_state = next_state;
	}
	g_memory_allocated = 0;
}



void save_rewind_state(void){

	if( g_memory_allocated == 0){
		return;
	}
	save_state_to_mem(ptr_rewind_states->data, rwnd_state_size);//declared in "nes/nes.h"
	ptr_rewind_states->have_data = 1;
	ptr_rewind_states = ptr_rewind_states->next;

}

int read_rewind_state(void){
	if( g_memory_allocated == 0){
		return -999;
	}

	int ret_val = -999;
	prev_state = ptr_rewind_states->prev;

	if (prev_state->have_data > 0 ){
		load_state_from_mem(prev_state->data); //declared in "nes/nes.h"
		prev_state->have_data = 0;
		ptr_rewind_states = ptr_rewind_states->prev;
		ret_val = 1;
	}
	return ret_val;
}

