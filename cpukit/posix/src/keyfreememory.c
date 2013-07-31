/**
 * @file
 *
 * @brief POSIX Function Keys Free Memory
 * @ingroup POSIXAPI
 */

/*
 * Copyright (c) 2012 Zhongwei Yao.
 * COPYRIGHT (c) 1989-2010.
 * On-Line Applications Research Corporation (OAR).
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/thread.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/rbtree.h>
#include <rtems/posix/key.h>

void _POSIX_Keys_Free_memory(
  POSIX_Keys_Control *the_key
)
{
  POSIX_Keys_Key_value_pair search_node;
  POSIX_Keys_Key_value_pair *p;
  RBTree_Node *iter, *next;

  search_node.key = the_key->Object.id;
  search_node.thread_id = 0;
  iter = _RBTree_Find_unprotected( &_POSIX_Keys_Rbtree, &search_node.Key_value_lookup_node );
  if ( !iter )
    return;
  /**
   * find the smallest thread_id node in the rbtree.
   */
  next = _RBTree_Next_unprotected( iter, RBT_LEFT );
  p = _RBTree_Container_of( next, POSIX_Keys_Key_value_pair, Key_value_lookup_node );
  while ( p->key == the_key->Object.id) {
    iter = next;
    next = _RBTree_Next_unprotected( iter, RBT_LEFT );
    p = _RBTree_Container_of( next, POSIX_Keys_Key_value_pair, Key_value_lookup_node );
  }

  /**
   * delete all nodes belongs to the_key from the rbtree and chain.
   */
  p = _RBTree_Container_of( iter, POSIX_Keys_Key_value_pair, Key_value_lookup_node );
  while ( p->key == the_key->Object.id ) {
    next = _RBTree_Next_unprotected( iter, RBT_RIGHT );
    _RBTree_Extract_unprotected( &_POSIX_Keys_Rbtree, iter );
    _Chain_Extract_unprotected( &p->Key_values_per_thread_node );
    /* append the node to _POSIX_Keys_Keypool */
    _Freechain_Put( (Freechain_Control *)&_POSIX_Keys_Keypool,
                       ( void * ) p->fc_node_ptr);
    iter = next;
    p = _RBTree_Container_of( iter, POSIX_Keys_Key_value_pair, Key_value_lookup_node );
  }
}
