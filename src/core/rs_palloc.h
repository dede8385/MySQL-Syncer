
#ifndef _RS_SLAB_H_INCLUDED_
#define _RS_SLAB_H_INCLUDED_

#include <rs_config.h>
#include <rs_core.h>

#define RS_POOL_PAGEALLOC           0
#define RS_POOL_PREALLOC            1
#define RS_POOL_CLASS_IDX           200

typedef struct {
    uint32_t        size; /* slab chunk size */
    uint32_t        num;

    char            **free_chunk;
    char            *chunk;

    uint32_t        free;

    uint32_t        used_slab;
    uint32_t        total_slab;

    uint32_t        total_free; 
    uint32_t        used_free; 
    char            **slab;

    uint32_t        died;
} rs_pool_class_t;

#define rs_pool_class_t_init(cls)                                            \
    (cls)->size = 0;                                                         \
    (cls)->num = 0;                                                          \
    (cls)->free_chunk = NULL;                                                \
    (cls)->chunk = NULL;                                                     \
    (cls)->free = 0;                                                         \
    (cls)->used_slab = 0;                                                    \
    (cls)->total_slab = 0;                                                   \
    (cls)->total_free = 0;                                                   \
    (cls)->used_free = 0;                                                    \
    (cls)->slab = NULL;                                                      \
    (cls)->died = 0


typedef struct {

    rs_pool_class_t *slab_class; /* chunk class */

    int32_t         cur_idx; /* current slab class index */
    int32_t         max_idx; /* max slab class index */

    char            *start; /* while prealloc start is start memory ptr */
    char            *cur; /* while preaaloc cur is current memory ptr */
    
    uint32_t        max_size; /* max memory size */
    uint32_t        used_size; /* used memory max size */
    uint32_t        free_size; /* free memory size*/

    uint32_t        chunk_size;

    int             flag;
} rs_pool_t;

/*
 * DESCRIPTION 
 *   Calc alloc size belong to which class id.
 *
 * PARAMTER 
 *   sl    : struct rs_pool_t
 *   size  : alloc size
 *
 * RETURN VALUE
 *   On success class_id is returned, On error RS_ERR is returned.
 */
int rs_palloc_id(rs_pool_t *p, uint32_t size);

/*
 * DESCRIPTION 
 *   Init memory slab.
 *
 * PARAMTER 
 *   init_size  : init chunk size
 *   mem_size   : memory max size
 *   chunk_size : chunk size
 *   max_idx    : max class index
 *   factor     : chunk class grow factor
 *   flag       : prealloc or page assgin
 *
 * RETURN VALUE
 *   On success rs_pool_t is returned, On error NULL is returned.
 */
rs_pool_t *rs_create_pool(uint32_t init_size, uint32_t mem_size, 
        uint32_t chunk_size, uint32_t max_idx, double factor, int32_t flag);

/*
 * DESCRIPTION 
 *   Alloc memory from slab class
 *
 * PARAMTER 
 *   sl    : struct rs_pool_t
 *   size  : memory size
 *   id    : chunk class id
 *
 * RETURN VALUE
 *   On success rs_pool_t is returned, On error NULL is returned.
 */
void *rs_palloc(rs_pool_t *p, uint32_t size, int id);

/*
 * DESCRIPTION 
 *   Free class chunk
 *
 * PARAMTER 
 *   sl    : struct rs_pool_t
 *   data  : free chunk data ptr
 *   id    : chunk class id
 *
 * RETURN VALUE
 *   On success class_id is returned, On error RS_ERR is returned.
 */
void rs_pfree(rs_pool_t *p, void *data, int id);

/*
 * DESCRIPTION 
 *   Free all slabs
 *
 * PARAMTER 
 *   sl    : struct rs_pool_t 
 *
 * RETURN VALUE
 *   None
 */
void rs_destroy_pool(rs_pool_t *p);

#endif
