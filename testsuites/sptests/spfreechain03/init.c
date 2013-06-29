/*
 * Copyright (c) 2013 Zhongwei Yao.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define __RTEMS_VIOLATE_KERNEL_VISIBILITY__
#include <tmacros.h>
#include <rtems/score/chain.h>
#include <rtems/score/freechain.h>
#include <rtems/malloc.h>

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);
bool my_freechain_extend( Freechain_Control *freechain );
void my_freechain_init( Freechain_Control *freechain );

typedef struct {
  Objects_Control obj;
  int             x;
} test_node;

bool my_freechain_extend( Freechain_Control *freechain )
{
  size_t size = freechain->bump_count * freechain->node_size;
  int i;
  test_node *nodes = _Workspace_Allocate(size);
  
  if (!nodes) {
    printf( "INIT - Unable to allocate free chain of size: %d\n", size );
    return NULL;
  }

  puts( "INIT - Allocate node from workspace in user defined freechain extend"
        " - OK" ); 

  for ( i = 0; i < freechain->bump_count; i++ ) {
      _Freechain_Put_node(freechain,
                          nodes + i * freechain->node_size);
  }
  return nodes;
}

void my_freechain_init( Freechain_Control *freechain )
{
  size_t bump_count = freechain->bump_count;
  size_t size = bump_count * sizeof(test_node);
  test_node *nodes = _Workspace_Allocate(size);

  _Chain_Initialize(
    &freechain->Freechain,
    nodes,
    bump_count,
    size
    );
}

rtems_task Init(
  rtems_task_argument ignored
  )
{
    test_node *test_node_p;
    Freechain_Control fc;
    int i;
    int init_size = 5;

    puts( "*** START OF RTEMS FREECHAIN API TEST ***" );
    _Freechain_Initialize(&fc,
                          sizeof(test_node),
                          init_size,
                          &my_freechain_extend);
    my_freechain_init(&fc);
    
    puts( "INIT - Get more than intialized nodes from freechain - OK" );
    
    for ( i = 0; i < init_size * 2; i++ ) {
        test_node_p = (test_node *)_Freechain_Get_node(&fc);
        if (!test_node_p) {
            puts( "INIT - Get node from freechain failed - FAILED" );
            rtems_test_exit(0);
        }
    }
    
    puts( "*** END OF RTEMS FREECHAIN API TEST ***" );
    rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER

#define CONFIGURE_MEMORY_OVERHEAD sizeof(test_node)
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

/* global variables */
