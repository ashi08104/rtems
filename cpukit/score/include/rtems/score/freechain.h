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

#ifndef _FREECHAIN_H
#define _FREECHAIN_H

#include <rtems/score/chain.h>
#include <rtems/rtems/types.h>
#include <rtems/score/wkspace.h>

/**
 * @defgroup ScoreFreechain Freechain Handler API
 *
 * @ingroup Score
 *
 * The Freechain Handler is used to manage a chain of nodes, of which size can
 * automatically increase when there is no free node left. This handler provides one data structure:
 * Freechain_Control.
 */
/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @typedef Freechain_Control
 */
typedef struct Freechain_Control Freechain_Control;

/**
 * @brief Extends the freechain.
 *
 * @param[in] freechain The freechain control.
 */
typedef void *( *Freechain_Extend )( Freechain_Control *freechain );

/**
 * @typedef freechain_callout
 *
 * @param[in] freechain The freechain control.
 * @param[in] nodes The new added nodes.
 */
typedef void (*freechain_callout)(
  Freechain_Control *freechain,
  void *nodes
);

/**
 * @typedef Freechain_Control
 *
 * This is used to manage freechain's nodes.
 */
struct Freechain_Control {
  Chain_Control     Freechain;
  size_t            bump_count;
  size_t            node_size;
  freechain_callout callout;
  Freechain_Extend  extend;
};

/**
 * @brief Initialize a freechain
 *
 * This routine initializes @a the Freechain_Control structure to manage a
 * chain of nodes, each node's size is @a node_size and the size of  chain is
 * initialized to @a bump_count and it also will increase with size of
 * @a bump_count. @a callout is called on all the nodes after allocated from
 * workspace.
 *
 * @param[in] freechain is the freechain too initialize.
 * @param[in] node_size is size of the elemment in this freechain.
 * @param[in] bump_count is the size of chain increased when no free node left.
 * @param[in] callout is the function called on all nodes in freechain_bump,
 * if it's null, a default function is set.
 * @param[in] extend is the user defined extention handle, it is called when no
 * free node left.
 */
void _Freechain_Initialize(
  Freechain_Control *freechain,
  size_t node_size,
  size_t bump_count,
  freechain_callout callout,
  Freechain_Extend extend
);

/**
 * @brief get node from freechain
 *
 * @param[in] freechain specifies the freechain to get
 *
 * @retval the point to node is return if success, return null if failed.
 */
void *_Freechain_Get_node(
  Freechain_Control *freechain
);

/**
 * @brief put node back to the freechain
 *
 * @param[in] freechain specifies the freechain to put
 * @param[in] n is the node to put back
 */
void _Freechain_Put_node(
  Freechain_Control *freechain,
  void *n
);

/**
 * @brief this is the default called callout function for freechain
 *
 * @param[in] freechain is the freechain which this function is in
 * @param[in] nodes are all nodes bumped in freechain_bump
 */
void _Freechain_Do_nothing(
  Freechain_Control *freechain,
  void *nodes
);

#ifdef __cplusplus
}
#endif
/**@}*/

#endif
/* end of include file */
