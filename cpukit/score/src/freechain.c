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
    Freechain_Extend extend
) {
  _Chain_Initialize_empty( &freechain->Freechain );
  freechain->extend = extend;
}

void *_Freechain_Get(Freechain_Control *freechain) {
  if ( _Chain_Is_empty( &freechain->Freechain ) ) {
    if ( !(*freechain->extend)(freechain) ) {
        return NULL;
    }
  }
  return _Chain_Get_first_unprotected( &freechain->Freechain );
}

void _Freechain_Put(Freechain_Control *freechain, void *n) {
  _Chain_Prepend_unprotected( &freechain->Freechain, n );
}

