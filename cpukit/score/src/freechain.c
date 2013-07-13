/**
 * @file
 *
 * @ingroup ScoreFreechain
 *
 * @brief Freechain Handler API
 */
/*
 * Copyright (c) 2013 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#include <rtems/score/freechain.h>
#include <rtems/malloc.h>

void _Freechain_Initialize(
    Freechain_Control *freechain,
    size_t node_size,
    size_t bump_count,
    freechain_callout callout,
    Freechain_Extend extend
) {
  _Chain_Initialize_empty( &freechain->Freechain );
  freechain->node_size = node_size;
  freechain->bump_count = bump_count;
  if (!callout)
    freechain->callout = &_Freechain_Do_nothing;
  else
    freechain->callout = callout;

  freechain->extend = extend;
}

/* size_t _Freechain_Bump(Freechain_Control *freechain) */
/* { */
/*   void *nodes; */
/*   int i; */
/*   size_t count = freechain->bump_count; */
/*   size_t size = freechain->node_size; */

/*   if (freechain->use_workspace == true) */
/*     nodes = _Workspace_Allocate(count * size); */
/*   else */
/*     nodes = malloc(count * size); */

/*   if (!nodes) { */
/*     printk("Unable to allocate free list of size: %d\n", count * size); */
/*     return 0; */
/*   } */

/*   freechain->free_count += count; */
/*   for ( i = 0; i < count; i++ ) { */
/*     _Chain_Append_unprotected( &freechain->Freechain, (uintptr_t)nodes+i*size ); */
/*   } */
/*   freechain->callout(freechain, nodes); */
/*   return count; */
/* } */

void *_Freechain_Get_node(Freechain_Control *freechain) {
  if ( _Chain_Is_empty( &freechain->Freechain ) ) {
    /* if ( !_Freechain_Bump(freechain) ) { */
    /*   return NULL; */
    /* } */
    if ( !(*freechain->extend)(freechain) ) {
        return NULL;
    }
  }
  return _Chain_Get_first_unprotected( &freechain->Freechain );
}

void _Freechain_Put_node(Freechain_Control *freechain, void *n) {
  _Chain_Prepend_unprotected( &freechain->Freechain, n );
}

void _Freechain_Do_nothing(Freechain_Control *freechain, void *nodes) {
  return;
}
