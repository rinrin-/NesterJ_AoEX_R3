/*
Author: davex
e-mail: efengeler@gmail.com
*/

#ifndef REWIND_H
#define REWIND_H

#include "main.h"
#include "nes/nes.h"
#include <string.h>
#include <stdlib.h>


void allocate_rewind_states(void);
int read_rewind_state(void);
void save_rewind_state(void);
void free_rewind_states(void);


#endif
