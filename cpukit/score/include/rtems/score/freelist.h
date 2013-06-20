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

#ifndef _FREELIST_H
#define _FREELIST_H

#include <rtems/score/chain.h>
#include <rtems/rtems/types.h>
#include <rtems/score/wkspace.h>

/**
 * @defgroup ScoreFreelist Freelist Handler API
 *
 * @ingroup Score
 *
 * The Freelist Handler is used to manage a chain of nodes, of which size can
 * automatically increase when there is no free node left. This handler provides one data structure:
 * Freelist_Control.
 */
/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @typedef Freelist_Control
 */
typedef struct Freelist_Control_struct Freelist_Control;

/**
 * @typedef freelist_callout
 */
typedef void (*freelist_callout)(
  Freelist_Control *fc,
  void *nodes
);

/**
 * @typedef Freelist_Control_struct
 *
 * This is used to manage each element.
 */
struct Freelist_Control_struct {
  Chain_Control     Freelist;
  size_t            free_count;
  size_t            bump_count;
  size_t            node_size;
  freelist_callout  callout;
};

/**
 * @brief Initialize a freelist
 *
 * This routine initializes @a the Freelist_Control structure to manage a
 * chain of nodes, each node's size is @a node_size and the size of  chain is
 * initialized to @a bump_count and it also will increase with size of
 * @a bump_count. @a callout is called on all the nodes after allocated from
 * workspace.
 *
 * @param[in] fc is the freelist too initialize.
 * @param[in] node_size is size of the elemment in this freelist.
 * @param[in] bump_count is the size of chain increased when no free node left.
 * @param[in] callout is the function called on all nodes in freelist_bump,
 * if it's null, a default function is set.
 */
void _Freelist_Initialize(
  Freelist_Control *fc,
  size_t node_size,
  size_t bump_count,
  freelist_callout callout
);

/**
 * @brief bump the freelist's size
 *
 * @param[in] fc specifies the freelist to bump
 */
size_t _Freelist_Bump(
  Freelist_Control* fc
);

/**
 * @brief get node from freelist
 *
 * @param[in] fc specifies the freelist to get
 *
 * @retval the point to node is return if success, return null if failed.
 */
void *_Freelist_Get_node(
  Freelist_Control *fc
);

/**
 * @brief put node back to the freelist
 *
 * @param[in] fc specifies the freelist to put
 * @param[in] n is the node to put back
 */
void Freelist_Put_node(
  Freelist_Control *fc,
  void *n
);

/**
 * @brief this is the default called callout function for freelist
 *
 * @param[in] fc is the freelist which this function is in
 * @param[in] nodes are all nodes bumped in freelist_bump
 */
void _Freelist_Do_nothing(
  Freelist_Control *fc,
  void *nodes
);

#ifdef __cplusplus
}
#endif
/**@}*/

#endif
/* end of include file */
