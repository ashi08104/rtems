/**
 * @file
 *
 * @ingroup ScoreFreelist
 *
 * @brief Freelist Handler API
 */
/*
 * Copyright (c) 2013 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#include <rtems/score/freelist.h>

void _Freelist_Initialize(
    Freelist_Control *fc,
    size_t node_size,
    size_t bump_count,
    freelist_callout callout
) {
  _Chain_Initialize_empty( &fc->Freelist );
  fc->node_size = node_size;
  fc->bump_count = bump_count;
  fc->free_count = 0;
  if (!callout)
    fc->callout = _Freelist_Do_nothing;
  else
    fc->callout = callout;
  _Freelist_Bump(fc);
}

size_t _Freelist_Bump(Freelist_Control *fc)
{
  void *nodes;
  int i;
  size_t count = fc->bump_count;
  size_t size = fc->node_size;

  /* better to use workspace or malloc? */
  nodes = _Workspace_Allocate(count * size);
  if (!nodes) {
    printk("Unable to allocate free list of size: %d\n", count * size);
    return 0;
  }

  fc->free_count += count;
  for ( i = 0; i < count; i++ ) {
    _Chain_Append_unprotected( &fc->Freelist, (uintptr_t)nodes+i*size );
  }
  fc->callout(fc, nodes);
  return count;
}

void *_Freelist_Get_node(Freelist_Control *fc) {
  if ( fc->free_count == 0 ) {
    if ( !_Freelist_Bump(fc) ) {
      return NULL;
    }
  }
  fc->free_count--;
  return _Chain_Get_first_unprotected( &fc->Freelist );
}

void _Freelist_Put_node(Freelist_Control *fc, void *n) {
  _Chain_Prepend_unprotected( &fc->Freelist, n );
  fc->free_count++;
}

void _Freelist_Do_nothing(Freelist_Control *fc, void *nodes) {
  return;
}
