/**
 * @file      cbuf.h
 *
 * @brief     Circular buffer module header
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

#ifndef __MODULE_H_
#define __MODULE_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 * DEFINES
 ******************************************************************************/

#ifndef CONFIG_CBUF_MAX_LEN
#define CONFIG_CBUF_MAX_LEN ((size_t)32)   /**< cbuf length. May be set in user code */
#endif // CONFIG_CBUF_MAX_LEN

#ifndef CONFIG_CBUF_VAL_TYPE
#define CONFIG_CBUF_VAL_TYPE int32_t /**< cbuf value types. May be set in user code */
#endif // CONFIG_CBUF_VAL_TYPE

#ifdef CONFIG_CBUF_TEST
#define CBUF_STATIC
#else 
#define CBUF_STATIC static
#endif

#define CBUF_ERR_OK ((int32_t)0)      /**< cbuf error - no error */
#define CBUF_ERR_PARAM ((int32_t)-1)   /**< cbuf error - error_param */
#define CBUF_ERR_VALIDATION ((int32_t) -2) /**< cbuf error - validation */
#define CBUF_ERR_OVERFLOW ((int32_t) -3) /**< cbuf error - overflow */
#define CBUF_ERR_EMPTY      ((int32_t) -4) /**< cbuf error - empty */
#define CBUF_ERR_NG         ((int32_t) -127) /**< cbuf error - general error */

/******************************************************************************
 * PUBLIC TYPES
 ******************************************************************************/

typedef CONFIG_CBUF_VAL_TYPE cbuf_val_t;

typedef struct cbuf_s {
    size_t head_idx;        /**< Index of first element in buf */
    size_t tail_idx;        /**< Indef of element in buf for next write */
    CONFIG_CBUF_VAL_TYPE buf[CONFIG_CBUF_MAX_LEN];
} cbuf_t;

/******************************************************************************
 * INLINE FUNCTIONS
 ******************************************************************************/

/******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 ******************************************************************************/

int32_t cbuf_init(cbuf_t * p_cbuf);
int32_t cbuf_deinit(cbuf_t * p_cbuf);
int32_t cbuf_push(cbuf_t * p_cbuf, const cbuf_val_t * p_val);
int32_t cbuf_peek(const cbuf_t * const p_cbuf, cbuf_val_t * p_val);
int32_t cbuf_pop(cbuf_t * p_cbuf, cbuf_val_t * p_val);
int64_t cbuf_len(const cbuf_t * const p_cbuf);
int64_t cbuf_capacity(const cbuf_t * const p_cbuf);
int32_t cbuf_is_full(const cbuf_t * const p_cbuf);

/******************************************************************************
 * END OF HEADER'S CODE
 ******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif  // __MODULE_H_

/** @}*/
