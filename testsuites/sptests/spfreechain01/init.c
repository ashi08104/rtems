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
void *my_freechain_extend_with_nothing( Freechain_Control *freechain );
void my_freechain_init( Freechain_Control *freechain );

typedef struct {
  Objects_Control obj;
  int             x;
} test_node;

void *my_freechain_extend_with_nothing( Freechain_Control *freechain )
{
    return NULL;
}

void my_freechain_init( Freechain_Control *freechain )
{
  size_t bump_count = freechain->bump_count;
  size_t size = bump_count * sizeof(test_node);
  test_node *objs = malloc(size);

  _Chain_Initialize(
    &freechain->Freechain,
    objs,
    bump_count,
    size
    );
}

rtems_task Init(
  rtems_task_argument ignored
  )
{
    puts( "*** START OF RTEMS FREECHAIN API TEST ***" );

    test_node *test_node_p;
    Freechain_Control fc;

    _Freechain_Initialize(&fc,
                          sizeof(test_node),
                          5,
                          NULL,
                          0);
    my_freechain_init(&fc);
    
    puts( "INIT - Get node from freechain - OK" );
    test_node_p = (test_node *)_Freechain_Get_node(&fc);
    test_node_p->x = 1;
    
    puts( "INIT - Put node to freechain - OK" );
    _Freechain_Put_node(&fc, (void *)test_node_p);

    puts( "INIT - Verify freechain node put and get - OK" );
    test_node_p = (test_node *)_Freechain_Get_node(&fc);
    if(test_node_p->x != 1) {
        puts( "INIT - ERROR ON FREECHAIN GET AND PUT" );
        rtems_test_exit(0);
    }
    
    puts( "*** END OF RTEMS FREECHAIN API TEST ***" );
    rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

/* global variables */
