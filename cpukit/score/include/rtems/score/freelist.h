#ifndef _FREELIST_H
#define _FREELIST_H

#include <rtems/score/chain.h>
#include <rtems/rtems/types.h>
#include <rtems/score/wkspace.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Freelist_Control_struct Freelist_Control;

typedef void (*freelist_callout)(
  Freelist_Control *fc,
  void *nodes
);

struct Freelist_Control_struct {
  Chain_Control     freelist;
  size_t            free_count;
  size_t            bump_count;
  size_t            node_size;
  freelist_callout  callout;
};

void freelist_initialize(Freelist_Control *fc, size_t node_size, size_t bump_count, freelist_callout callout);
size_t freelist_bump(Freelist_Control* fc);
void *freelist_get_node(Freelist_Control *fc);
void freelist_put_node(Freelist_Control *fc, void *n);

void freelist_do_nothing( Freelist_Control *fc, void *nodes );

#ifdef __cplusplus
}
#endif


#endif