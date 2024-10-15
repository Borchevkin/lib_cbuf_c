/**
  * @file      module.c
  *
  * @brief     Circular buffer implementations
  *
  * @date      2024-10-09
  *
  * @par
  * BSD 2-Clause License
  * 
  * Copyright (c) 2024, Danil Borchevkin
  * 
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met:
  * 
  * 1. Redistributions of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 
  * 2. Redistributions in binary form must reproduce the above copyright
  *    notice, this list of conditions and the following disclaimer in
  *    the documentation and/or other materials provided with the distribution.
  * 
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
  * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  */

/** \addtogroup cbuf Circular buffer
*  @{
*/

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "cbuf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/******************************************************************************
 * DEFINES, CONSTS, ENUMS
 ******************************************************************************/

/******************************************************************************
 * PRIVATE TYPES
 ******************************************************************************/



/******************************************************************************
 * PRIVATE DATA
 ******************************************************************************/



/******************************************************************************
 * PUBLIC DATA
 ******************************************************************************/



/******************************************************************************
 * EXTERNAL DATA
 ******************************************************************************/



/******************************************************************************
 * EXTERNAL FUNCTION PROTOTYPES
 ******************************************************************************/



/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 ******************************************************************************/

CBUF_STATIC int32_t cbuf_check(const cbuf_t * const p_cbuf);

/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

CBUF_STATIC int32_t cbuf_check(const cbuf_t * const p_cbuf) {
    if (NULL == p_cbuf) {
        return CBUF_ERR_PARAM;
    }

    if (p_cbuf->head_idx >= p_cbuf->capacity) {
        return CBUF_ERR_VALIDATION;
    }

    if (p_cbuf->len > p_cbuf->capacity) {
        return CBUF_ERR_VALIDATION;
    }

    return CBUF_ERR_OK;
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

int32_t cbuf_init(cbuf_t * p_cbuf) {
    if (NULL == p_cbuf) {
        return CBUF_ERR_PARAM;
    }

    p_cbuf->head_idx = 0;
    p_cbuf->len = 0;
    p_cbuf->capacity = CONFIG_CBUF_MAX_LEN;

    int32_t err = cbuf_check(p_cbuf);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    return CBUF_ERR_OK;    
}

int32_t cbuf_deinit(cbuf_t * p_cbuf) {
    int32_t err = cbuf_check(p_cbuf);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    // NOTE: no actions needed for this version.

    return CBUF_ERR_OK;
}

int32_t cbuf_push(cbuf_t * p_cbuf, const cbuf_val_t * p_val) {
    int32_t err = cbuf_check(p_cbuf);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    if (NULL == p_val) {
        return CBUF_ERR_PARAM;
    }

    err = cbuf_is_full(p_cbuf);
    switch (err) {
        case CBUF_ERR_OK:

            err = CBUF_ERR_OK;
            break;

        case CBUF_ERR_FULL:
            ++p_cbuf->head_idx;
            err = CBUF_ERR_OVERFLOW;
            break;

        default:
            return err;
    }

    size_t push_idx = (p_cbuf->head_idx + p_cbuf->len) % p_cbuf->capacity;
    
    p_cbuf->buf[push_idx] = *p_val;

    return err;
}

int32_t cbuf_peek(const cbuf_t * const p_cbuf, cbuf_val_t * p_val) {
    int32_t err = cbuf_check(p_cbuf);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    if (NULL == p_val) {
        return CBUF_ERR_PARAM;
    }

    err = cbuf_is_empty(p_cbuf);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    size_t tail_idx = (p_cbuf->head_idx + p_cbuf->len - 1) % p_cbuf->capacity;

    *p_val = p_cbuf->buf[tail_idx];

    return CBUF_ERR_OK;
}

int32_t cbuf_pop(cbuf_t * p_cbuf, cbuf_val_t * p_val) {
    int32_t err = cbuf_check(p_cbuf);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    err = cbuf_is_empty(p_cbuf);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    err = cbuf_peek(p_cbuf, p_val);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    --p_cbuf->len;

    return CBUF_ERR_OK;
}

int64_t cbuf_len(const cbuf_t * const p_cbuf) {
    int32_t err = cbuf_check(p_cbuf);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    return (int32_t) p_cbuf->len; 
}

int64_t cbuf_capacity(const cbuf_t * const p_cbuf) {
    int32_t err = cbuf_check(p_cbuf);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    return (int32_t) p_cbuf->capacity;
}

int32_t cbuf_is_empty(const cbuf_t* const p_cbuf) {
    int32_t err = cbuf_check(p_cbuf);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    if (0 == p_cbuf->len) {
        return CBUF_ERR_EMPTY;
    }

    return CBUF_ERR_OK;
}

int32_t cbuf_is_full(const cbuf_t * const p_cbuf) {
    int32_t err = cbuf_check(p_cbuf);
    if (CBUF_ERR_OK != err) {
        return err;
    }

    if (p_cbuf->len == p_cbuf->capacity) {
        return CBUF_ERR_FULL;
    }

    return CBUF_ERR_OK;
}

/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
