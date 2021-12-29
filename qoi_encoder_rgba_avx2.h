#include <assert.h>
#include <immintrin.h>

static void qoi_encode_block_rgba_runs(const unsigned char *pixels, unsigned int *output) {
    __m256i vec_prev = _mm256_loadu_si256((__m256i *) (pixels - 4));
    __m256i vec_curr = _mm256_loadu_si256((__m256i *) pixels);
    __m256i same = _mm256_cmpeq_epi32(vec_prev, vec_curr);
    __m256i runs = _mm256_and_si256(same, _mm256_set1_epi32(0x00000001));

    _mm256_storeu_si256((__m256i *) output, runs);
}

static void qoi_encode_block_rgba_indexes(const unsigned char *pixels, int *output) {
    __m256i coeff_red_green  = _mm256_set_epi16(5, 3, 5, 3, 5, 3, 5, 3, 5, 3, 5, 3, 5, 3, 5, 3);
    __m256i coeff_blue_alpha = _mm256_set_epi16(11, 7, 11, 7, 11, 7, 11, 7, 11, 7, 11, 7, 11, 7, 11, 7);

    __m256i shuffle_red_green = _mm256_set_epi8(
        13, -1, 12, -1, 9, -1, 8, -1, 5, -1, 4, -1, 1, -1, 0, -1,
        13, -1, 12, -1, 9, -1, 8, -1, 5, -1, 4, -1, 1, -1, 0, -1
    );

    __m256i shuffle_blue_alpha = _mm256_set_epi8(
        15, -1, 14, -1, 11, -1, 10, -1, 7, -1, 6, -1, 3, -1, 2, -1,
        15, -1, 14, -1, 11, -1, 10, -1, 7, -1, 6, -1, 3, -1, 2, -1
    );

    __m256i shuffle_sum_red_green = _mm256_set_epi8(
        -1, -1, -1, 7, -1, -1, -1, 5, -1, -1, -1, 3, -1, -1, -1, 1,
        -1, -1, -1, 7, -1, -1, -1, 5, -1, -1, -1, 3, -1, -1, -1, 1
    );

    __m256i shuffle_sum_blue_alpha = _mm256_set_epi8(
        -1, -1, -1, 15, -1, -1, -1, 13, -1, -1, -1, 11, -1, -1, -1, 9,
        -1, -1, -1, 15, -1, -1, -1, 13, -1, -1, -1, 11, -1, -1, -1, 9
    );

    __m256i modulos = _mm256_set_epi8(
        0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 63,
        0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 63
    );

    __m256i vec_rgba = _mm256_loadu_si256((__m256i *) pixels);

    __m256i red_green  = _mm256_shuffle_epi8(vec_rgba, shuffle_red_green);
    __m256i blue_alpha = _mm256_shuffle_epi8(vec_rgba, shuffle_blue_alpha);

    __m256i hashes_red_green  = _mm256_mullo_epi16(red_green,  coeff_red_green);
    __m256i hashes_blue_alpha = _mm256_mullo_epi16(blue_alpha, coeff_blue_alpha);

    __m256i half_sums_rgba  = _mm256_hadd_epi16(hashes_red_green, hashes_blue_alpha);
    __m256i sums_red_green  = _mm256_shuffle_epi8(half_sums_rgba, shuffle_sum_red_green);
    __m256i sums_blue_alpha = _mm256_shuffle_epi8(half_sums_rgba, shuffle_sum_blue_alpha);

    __m256i full_sums_rgba  = _mm256_add_epi8(sums_red_green, sums_blue_alpha);
    __m256i indexes_pos     = _mm256_and_si256(full_sums_rgba, modulos);

    _mm256_storeu_si256((__m256i *) output, indexes_pos);
}

/**
 * This computes the following chunks values given a RGBA input :
 *   - QOI_OP_DIFF
 *   - QOI_OP_LUMA
 *   - QOI_OP_RGB
 *   - QOI_OP_RGBA
 *
 * @param pixels            32 bytes to encode
 * @param block_lengths     Length of each encoded chunk, only the first byte out of four is set
 *
 *                          When the encoded chunk length equals 5
 *                          Then the QOI_OP_RGBA byte is implied and won't appears in the corresponding encoded chunk
 *
 * @param block_values      Eight encoded chunks
 */
static void qoi_encode_block_rgba_values(const unsigned char *pixels, unsigned char *block_lengths, unsigned char *block_values) {
    __m256i vec_prev = _mm256_loadu_si256((__m256i *) (pixels - 4));
    __m256i vec_curr = _mm256_loadu_si256((__m256i *) pixels);
    __m256i vec_diff = _mm256_sub_epi8(vec_curr, vec_prev);

    __m256i vec_mask_red      = _mm256_set1_epi32(0x000000ff);
    __m256i vec_mask_green    = _mm256_set1_epi32(0x0000ff00);
    __m256i vec_mask_blue     = _mm256_set1_epi32(0x00ff0000);
    __m256i vec_mask_alpha    = _mm256_set1_epi32(0xff000000);
    __m256i vec_mask_no_alpha = _mm256_set1_epi32(0x00ffffff);

    /************************************************************
     *                                                          *
     *      Compute all the arithmetic operations required      *
     *      to determine if the OP code is QOI_OP_RGBA          *
     *                                                          *
     ************************************************************/

     __m256i vec_same       = _mm256_cmpeq_epi8(vec_diff, _mm256_setzero_si256());
     __m256i vec_same_alpha = _mm256_and_si256(vec_same, vec_mask_alpha);

    __m256i vec_is_op_rgba = _mm256_cmpeq_epi32(vec_same_alpha, _mm256_setzero_si256());
    __m256i vec_is_not_op_rgba = _mm256_cmpeq_epi32(vec_is_op_rgba, _mm256_set1_epi32(0));


    /************************************************************
     *                                                          *
     *      Compute all the arithmetic operations required      *
     *      to determine if the OP code is QOI_OP_DIFF :        *
     *                                                          *
     *              vr > -3 && vr < 2                           *
     *           && vg > -3 && vg < 2                           *
     *           && vb > -3 && vb < 2                           *
     *                                                          *
     ************************************************************/

    __m256i vec_diff_gt_minus_3 = _mm256_cmpgt_epi8(vec_diff, _mm256_set1_epi8(-3));
    __m256i vec_diff_gt_1       = _mm256_cmpgt_epi8(vec_diff, _mm256_set1_epi8(1));
    __m256i vec_diff_lt_2       = _mm256_andnot_si256(vec_diff_gt_1, _mm256_set1_epi8(255));
    __m256i vec_between_minus_3_and_2 = _mm256_and_si256(vec_diff_gt_minus_3, vec_diff_lt_2);
    __m256i vec_op_diff_mask_alpha    = _mm256_and_si256(vec_between_minus_3_and_2, vec_mask_no_alpha);

    __m256i vec_is_op_diff = _mm256_cmpeq_epi32(vec_op_diff_mask_alpha, vec_mask_no_alpha);


    /************************************************************
     *                                                          *
     *      Compute all the arithmetic operations required      *
     *      to determine if the OP code is QOI_OP_LUMA :        *
     *                                                          *
     *              vg_r > -9  && vg_r < 8                      *
     *           && vg   > -33 && vg   < 32                     *
     *           && vg_b > -9  && vg_b <  8                     *
     *                                                          *
     ************************************************************/

    __m256i vec_diff_green = _mm256_and_si256(vec_diff, vec_mask_green);

    // Computes : vg > -33 && vg < 32
    __m256i vec_green_gt_minus_33 = _mm256_cmpgt_epi8(vec_diff_green, _mm256_set1_epi8(-33));
    __m256i vec_green_gt_31       = _mm256_cmpgt_epi8(vec_diff_green, _mm256_set1_epi8(31));
    __m256i vec_green_lt_32       = _mm256_andnot_si256(vec_green_gt_31, vec_mask_green);
    __m256i vec_green_is_between  = _mm256_and_si256(vec_green_gt_minus_33, vec_green_lt_32);

    // Computes : vg_r > -9 && vg_r < 8 && vg_b > -9 && vg_b < 8
    __m256i vec_shuffle_green = _mm256_set_epi8(
        -1, 13, -1, 13, -1, 9, -1, 9, -1, 5, -1, 5, -1, 1, -1, 1,
        -1, 13, -1, 13, -1, 9, -1, 9, -1, 5, -1, 5, -1, 1, -1, 1
    );

    __m256i vec_shuffled_diff_green = _mm256_shuffle_epi8(vec_diff_green, vec_shuffle_green);
    __m256i vec_diff_red_blue = _mm256_and_si256(vec_diff, _mm256_set1_epi32(0x00ff00ff));
    __m256i vec_diff_green_rb = _mm256_sub_epi8(vec_diff_red_blue, vec_shuffled_diff_green);

    __m256i vec_diff_green_rb_gt_minus_9 = _mm256_cmpgt_epi8(vec_diff_green_rb, _mm256_set1_epi8(-9));
    __m256i vec_diff_green_rb_gt_7       = _mm256_cmpgt_epi8(vec_diff_green_rb, _mm256_set1_epi8(7));
    __m256i vec_diff_green_rb_lt_8       = _mm256_andnot_si256(vec_diff_green_rb_gt_7, _mm256_set1_epi32(0x00ff00ff));
    __m256i vec_diff_green_rb_is_between = _mm256_and_si256(vec_diff_green_rb_gt_minus_9, vec_diff_green_rb_lt_8);


    // Computes : vg > -33 && vg < 32 && vg_r > -9 && vg_r < 8 && vg_b > -9 && vg_b < 8
    __m256i vec_diff_luma  = _mm256_or_si256(vec_green_is_between, vec_diff_green_rb_is_between);
    __m256i vec_is_op_luma = _mm256_cmpeq_epi32(vec_diff_luma, vec_mask_no_alpha);


    /************************************************
     *                                              *
     *      Compute the chunk values for :          *
     *                                              *
     *          - QOI_OP_DIFF                       *
     *          - QOI_OP_LUMA                       *
     *          - QOI_OP_RGB                        *
     *                                              *
     ************************************************/

    /**
     * Compute : QOI_OP_DIFF | (vr + 2) << 4 | (vg + 2) << 2 | (vb + 2)
     *
     *                              chan A |   chan B |   chan G |   chan R
     *   bits                   :   31..24 |   23..16 |    15..8 |     7..0
     *   vec_diff_plus_2        : 000000aa | 000000bb | 000000gg | 000000rr
     *   vec_masked_red         : 00000000 | 00000000 | 00000000 | 000000rr
     *   vec_masked_green       : 00000000 | 00000000 | 000000gg | 00000000
     *   vec_masked_blue        : 00000000 | 000000bb | 00000000 | 00000000
     *
     *   vec_code_op_diff       : 00000000 | 00000000 | 00000000 | 01000000
     *   vec_shifted_red        : 00000000 | 00000000 | 00000000 | 00rr0000     vec_masked_red   <<  2
     *   vec_shifted_green      : 00000000 | 00000000 | 00000000 | 0000gg00     vec_masked_green >>  6
     *   vec_shifted_blue       : 00000000 | 00000000 | 00000000 | 000000bb     vec_masked_blue  >> 16
     *
     *   vec_value_op_diff_high : 00000000 | 00000000 | 00000000 | 01rr0000
     *   vec_value_op_diff_low  : 00000000 | 00000000 | 00000000 | 0000ggbb
     *   vec_value_op_diff      : 00000000 | 00000000 | 00000000 | 01rrggbb
     */

    __m256i vec_diff_plus_2 = _mm256_add_epi8(vec_diff, _mm256_set1_epi8(2));

    __m256i vec_masked_red   =  _mm256_and_si256(vec_diff_plus_2, vec_mask_red);
    __m256i vec_masked_green =  _mm256_and_si256(vec_diff_plus_2, vec_mask_green);
    __m256i vec_masked_blue  =  _mm256_and_si256(vec_diff_plus_2, vec_mask_blue);

    __m256i vec_code_op_diff  = _mm256_set_epi8(
        0, 0, 0, QOI_OP_DIFF, 0, 0, 0, QOI_OP_DIFF, 0, 0, 0, QOI_OP_DIFF, 0, 0, 0, QOI_OP_DIFF,
        0, 0, 0, QOI_OP_DIFF, 0, 0, 0, QOI_OP_DIFF, 0, 0, 0, QOI_OP_DIFF, 0, 0, 0, QOI_OP_DIFF
    );

    __m256i vec_shifted_red   = _mm256_slli_epi32(vec_masked_red,   4);
    __m256i vec_shifted_green = _mm256_srli_epi32(vec_masked_green, 6);
    __m256i vec_shifted_blue  = _mm256_srli_epi32(vec_masked_blue,  16);

    __m256i vec_value_op_diff_high = _mm256_or_si256(vec_code_op_diff, vec_shifted_red);
    __m256i vec_value_op_diff_low  = _mm256_or_si256(vec_shifted_green, vec_shifted_blue);

    __m256i vec_value_op_diff = _mm256_or_si256(vec_value_op_diff_high, vec_value_op_diff_low);


    // Computes : QOI_OP_LUMA | (vg   + 32)
    __m256i vec_diff_green_plus_32    = _mm256_add_epi8(vec_diff_green, _mm256_set1_epi32(0x00002000));
    __m256i vec_shifted_green_luma    = _mm256_srli_epi32(vec_diff_green_plus_32, 8);

    __m256i vec_code_op_luma       = _mm256_set_epi8(
        0, 0, 0, QOI_OP_LUMA, 0, 0, 0, QOI_OP_LUMA, 0, 0, 0, QOI_OP_LUMA, 0, 0, 0, QOI_OP_LUMA,
        0, 0, 0, QOI_OP_LUMA, 0, 0, 0, QOI_OP_LUMA, 0, 0, 0, QOI_OP_LUMA, 0, 0, 0, QOI_OP_LUMA
    );

    __m256i vec_value_op_luma_high = _mm256_or_si256(vec_code_op_luma, vec_shifted_green_luma);

    // Computes : (vg_r + 8) << 4 | (vg_b +  8)
    __m256i vec_diff_green_rb_plus_8 = _mm256_add_epi8(vec_diff_green_rb, _mm256_set1_epi32(0x00080008));
    __m256i vec_diff_green_red       = _mm256_and_si256(vec_diff_green_rb_plus_8, vec_mask_red);
    __m256i vec_diff_green_blue      = _mm256_and_si256(vec_diff_green_rb_plus_8, vec_mask_blue);
    __m256i vec_shifted_green_red    = _mm256_slli_epi32(vec_diff_green_red,  12);
    __m256i vec_shifted_green_blue   = _mm256_srli_epi32(vec_diff_green_blue, 8);
    __m256i vec_value_op_luma_low    = _mm256_or_si256(vec_shifted_green_red, vec_shifted_green_blue);

    __m256i vec_value_op_luma = _mm256_or_si256(vec_value_op_luma_high, vec_value_op_luma_low);


    // Computes : { QOI_OP_RGB, red, green, blue }
    __m256i vec_code_op_rgb  = _mm256_set_epi8(
        0, 0, 0, QOI_OP_RGB, 0, 0, 0, QOI_OP_RGB, 0, 0, 0, QOI_OP_RGB, 0, 0, 0, QOI_OP_RGB,
        0, 0, 0, QOI_OP_RGB, 0, 0, 0, QOI_OP_RGB, 0, 0, 0, QOI_OP_RGB, 0, 0, 0, QOI_OP_RGB
    );

    __m256i vec_rgb          = _mm256_and_si256(vec_curr, vec_mask_no_alpha);
    __m256i vec_shifted_rgb  = _mm256_slli_epi32(vec_rgb, 8);
    __m256i vec_value_op_rgb = _mm256_or_si256(vec_code_op_rgb, vec_shifted_rgb);


    /************************************************
     *                                              *
     *  Compute the appropriate chunk lengths :     *
     *                                              *
     *      - 1 for QOI_OP_DIFF                     *
     *      - 2 for QOI_OP_LUMA                     *
     *      - 4 for QOI_OP_RGB                      *
     *      - 5 for QOI_OP_RGBA                     *
     *                                              *
     ************************************************/

    __m256i vec_length_op_diff = _mm256_and_si256(vec_is_op_diff,     _mm256_set1_epi32(1));
    __m256i vec_length_op_luma = _mm256_and_si256(vec_is_op_luma,     _mm256_set1_epi32(2));
    __m256i vec_length_op_rgb  = _mm256_and_si256(vec_is_not_op_rgba, _mm256_set1_epi32(4));
    __m256i vec_length_op_rgba = _mm256_and_si256(vec_is_op_rgba,     _mm256_set1_epi32(5));

    __m256i vec_is_op_diff_luma = _mm256_or_si256(vec_is_op_diff, vec_is_op_luma);

    __m256i vec_length_op_diff_luma     = _mm256_blendv_epi8(vec_length_op_luma, vec_length_op_diff, vec_is_op_diff);
    __m256i vec_length_op_diff_luma_rgb = _mm256_blendv_epi8(vec_length_op_rgb, vec_length_op_diff_luma, vec_is_op_diff_luma);
    __m256i vec_length_op               = _mm256_blendv_epi8(vec_length_op_diff_luma_rgb, vec_length_op_rgba, vec_is_op_rgba);


    /************************************************
     *                                              *
     *  Compute the appropriate chunk values        *
     *                                              *
     ************************************************/

    __m256i vec_value_op_diff_luma     = _mm256_blendv_epi8(vec_value_op_luma, vec_value_op_diff, vec_is_op_diff);
    __m256i vec_value_op_diff_luma_rgb = _mm256_blendv_epi8(vec_value_op_rgb, vec_value_op_diff_luma, vec_is_op_diff_luma);
    __m256i vec_value                  = _mm256_blendv_epi8(vec_value_op_diff_luma_rgb, vec_curr, vec_is_op_rgba);


    _mm256_storeu_si256((__m256i *) block_lengths, vec_length_op);
    _mm256_storeu_si256((__m256i *) block_values,  vec_value);
}

static qoi_encoder_t qoi_encode_rgba_avx2(const unsigned char *pixels, unsigned char *bytes, qoi_encoder_t encoder) {
    {
        qoi_rgba_t px = *(qoi_rgba_t *)(pixels + encoder.px_pos);

        if (px.v == encoder.px_prev.v) {
            encoder.run++;
        }
        else {
            int index_pos = QOI_COLOR_HASH(px) % 64;

            if (encoder.index[index_pos].v == px.v) {
                bytes[encoder.p++] = QOI_OP_INDEX | index_pos;
            }
            else {
                encoder.index[index_pos] = px;

                if (px.rgba.a == encoder.px_prev.rgba.a) {
                    signed char vr = px.rgba.r - encoder.px_prev.rgba.r;
                    signed char vg = px.rgba.g - encoder.px_prev.rgba.g;
                    signed char vb = px.rgba.b - encoder.px_prev.rgba.b;

                    signed char vg_r = vr - vg;
                    signed char vg_b = vb - vg;

                    if (QOI_IS_OP_DIFF(vr, vg, vb)) {
                        QOI_WRITE_OP_DIFF(bytes, encoder, vr, vg, vb);
                    }
                    else if (QOI_IS_OP_LUMA(vg, vg_r, vg_b)) {
                        QOI_WRITE_OP_LUMA(bytes, encoder, vg, vg_r, vg_b);
                    }
                    else {
                        QOI_WRITE_OP_RGB(bytes, encoder, px);
                    }
                }
                else {
                    QOI_WRITE_OP_RGBA(bytes, encoder, px);
                }
            }
        }

        encoder.px_prev = px;
        encoder.px_pos += 4;
    }

    // When block_lengths[block_offfset] == 1 or when block_lengths[block_offfset] == 2
    // we are actually writing too many bytes
    //
    // This explain the need for a little padding at the beginning of the loop
    const int padding = 16;

    while (encoder.px_pos + 32 + padding < encoder.px_end) {
        int indexes_pos[8];
        unsigned int block_runs[8];
        unsigned char block_lengths[32];
        unsigned char block_values[32];

        qoi_encode_block_rgba_runs(pixels + encoder.px_pos, block_runs);
        qoi_encode_block_rgba_indexes(pixels + encoder.px_pos, indexes_pos);
        qoi_encode_block_rgba_values(pixels + encoder.px_pos, block_lengths, block_values);

        for (int i = 0; i < 8; i++) {
            qoi_rgba_t px = *(qoi_rgba_t *)(pixels + encoder.px_pos);

            encoder.run += block_runs[i];

            if (encoder.run >= 62) {
                bytes[encoder.p++] = QOI_OP_RUN | 61;
                encoder.run -= 62;
            }
            else if (block_runs[i] == 0) {
                bytes[encoder.p] = QOI_OP_RUN | (encoder.run - 1);
                encoder.p += encoder.run > 0;
                encoder.run = 0;

                int index_pos = indexes_pos[i];

                bytes[encoder.p] = QOI_OP_INDEX | index_pos;
                encoder.p += encoder.index[index_pos].v == px.v;

                int block_offset = i * 4;

                // The QOI_OP_RGBA byte never appears in the block_values
                // so we need to assume that is might be a QOI_OP_RGBA
                bytes[encoder.p + 0] = QOI_OP_RGBA;
                encoder.p += block_lengths[block_offset] == 5;

                bytes[encoder.p + 0] = block_values[block_offset + 0];
                bytes[encoder.p + 1] = block_values[block_offset + 1];
                bytes[encoder.p + 2] = block_values[block_offset + 2];
                bytes[encoder.p + 3] = block_values[block_offset + 3];

                encoder.p += block_lengths[block_offset] * (encoder.index[index_pos].v != px.v);
                encoder.p -= block_lengths[block_offset] == 5;

                encoder.index[index_pos] = px;
            }

            encoder.px_pos += 4;
        }
    }

    encoder.px_prev = *(qoi_rgba_t *)(pixels + encoder.px_pos - 4);

    return encoder;
}