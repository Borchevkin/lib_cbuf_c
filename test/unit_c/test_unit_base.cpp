#include <gtest/gtest.h>

#define CONFIG_CBUF_TEST

#include "cbuf.h"
#include "cbuf.c"
#include <stdint.h>

TEST(Unit, CbufValidation) {
    int32_t err = CBUF_ERR_NG;

    cbuf_t cbuf;

    err = cbuf_check(NULL);
    EXPECT_EQ(err, CBUF_ERR_PARAM);

    cbuf.head_idx = CONFIG_CBUF_MAX_LEN;
    cbuf.tail_idx = CONFIG_CBUF_MAX_LEN;

    err = cbuf_check(&cbuf);
    EXPECT_EQ(err, CBUF_ERR_VALIDATION);

    cbuf.head_idx = 0;
    cbuf.tail_idx = 0;

    err = cbuf_check(&cbuf);
    EXPECT_EQ(err, CBUF_ERR_OK);
}

TEST(Unit, CbufInit) {
    int32_t err = CBUF_ERR_NG;

    err = cbuf_init(NULL);
    EXPECT_EQ(err, CBUF_ERR_PARAM);

    cbuf_t cbuf;

    err = cbuf_init(&cbuf);
    EXPECT_EQ(err, CBUF_ERR_OK);
}

TEST(Unit, CbufDeinit) {
    int32_t err = CBUF_ERR_NG;
    cbuf_t cbuf;
    err = cbuf_init(&cbuf);
    EXPECT_EQ(err, CBUF_ERR_OK);

    err = cbuf_deinit(NULL);
    EXPECT_EQ(err, CBUF_ERR_PARAM);

    err = cbuf_deinit(&cbuf);
    EXPECT_EQ(err, CBUF_ERR_OK);
}

TEST(Unit, CbufPush) {
    int32_t err = CBUF_ERR_NG;
    cbuf_t cbuf;
    err = cbuf_init(&cbuf);
    EXPECT_EQ(err, CBUF_ERR_OK);

    EXPECT_EQ(cbuf.head_idx, 0);
    EXPECT_EQ(cbuf.tail_idx, 0);

    cbuf_val_t val = 53;

    err = cbuf_push(NULL, &val);
    EXPECT_EQ(err, CBUF_ERR_PARAM);

    err = cbuf_push(&cbuf, NULL);
    EXPECT_EQ(err, CBUF_ERR_PARAM);

    err = cbuf_push(NULL, NULL);
    EXPECT_EQ(err, CBUF_ERR_PARAM);

    err = cbuf_push(&cbuf, &val);
    EXPECT_EQ(err, CBUF_ERR_OK);
    EXPECT_EQ(cbuf.head_idx, 0);
    EXPECT_EQ(cbuf.tail_idx, 1);
    EXPECT_EQ(cbuf.buf[0], 53);

    val = 77;
    err = cbuf_push(&cbuf, &val);
    EXPECT_EQ(err, CBUF_ERR_OK);
    EXPECT_EQ(cbuf.head_idx, 0);
    EXPECT_EQ(cbuf.tail_idx, 2);
    EXPECT_EQ(cbuf.buf[0], 77);

    val = 88;
    err = cbuf_push(&cbuf, &val);
    EXPECT_EQ(err, CBUF_ERR_OK);
    EXPECT_EQ(cbuf.head_idx, 0);
    EXPECT_EQ(cbuf.tail_idx, 3);
    EXPECT_EQ(cbuf.buf[0], 88);
}

