#include "nnom.h"

/* Weights, bias and Q format */
#define TENSOR_DENSE_KERNEL_0 {0, 0, 44, 0, 0, 0, 43, 0, 67, 0, -17, -22, -10, 0, 31, 41, 0, -35, 60, 0, 0, 37, -41, 0, -28, 0, 56, -65, 54, 0, 25, 21, 34, -25, 0, 19, -5, 57, 0, -45, -2, 29, -16, 28, -1, -31, 10, 56, -41, 0, 28, -28, 42, 0, -60, 59, 0, 0, -13, 32, 0, 0, 19, 28}

#define TENSOR_DENSE_KERNEL_0_DEC_BITS {7}

#define TENSOR_DENSE_BIAS_0 {0, 0, 35, 0, 69, 0, 0, -3, 0, -7, 69, -5, -4, 0, 44, -15, 38, -17, 0, 8, 1, 7, 20, 26, -6, 0, 2, -11, -5, 0, -15, 13}

#define TENSOR_DENSE_BIAS_0_DEC_BITS {9}

#define DENSE_BIAS_LSHIFT {5}

#define DENSE_OUTPUT_RSHIFT {7}

#define TENSOR_DENSE_1_KERNEL_0 {0, 0, 35, 59, 0, 0, -1, -13, 0, 0, 0, 0, 0, 0, 0, 0, 17, 2, -30, 57, 0, 29, 0, -53, 0, 0, 26, 58, 0, 0, 0, -54, 0, 0, 30, 9, 0, 0, 0, 26, 17, 28, 0, 0, 0, -2, 0, 0, 24, 24, 17, 47, 0, -31, -1, -5, 0, 0, -42, -74, 0, 0, 0, 0, 50, 9, 0, 0, 6, 30, 0, 0, -48, 10, 1, -17, 0, -67, 0, -81, 4, 2, 1, -19, 0, 0, 0, -5, -15, -41, -19, 58, 2, -16, -9, 36, 9, 46, 37, -71, 0, 0, 0, -55, 0, 0, -14, 24, 0, 0, 0, -57, 0, 0, -48, 15, 0, 0, 0, 0, 0, 0, -9, 44, 0, 0, -2, -20, 0, 0, -42, 4, 0, 0, 0, -22, 0, 0, 0, 0, 0, 0, 0, 0, -42, 1, 30, 28, 0, 49, 0, 10, 0, 0, -4, 46, 0, 0, 0, -11, 0, 0, -4, -61, 0, 0, 0, 35, 39, 62, 0, 0, 0, -50, 0, 0, 35, 31, 25, 12, 0, -27, 0, 0, 0, 0, 43, -47, 0, 0, 0, 12, -5, -23, 0, 0, 0, -1, 0, 0, -22, 0, 0, 0, 0, -16, 0, 10, -1, 0, 7, -34, 0, 2, 0, 5, 0, 0, 6, -33, 0, -39, 0, 28, 22, 17, 0, 0, 0, -54, 0, 49, 0, 0, 21, 45, 0, 0, 0, 12, 0, 0, 27, 36, 0, 0, 0, -17, 0, 0, -32, 34, 0, 0, 0, 29, 0, 0, -10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 61, 0, -1, 0, 0, 0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, -64, 0, 0, 0, -1, 0, 5, 0, 0, 0, 0, 0, 0, 0, 2, 0, -19, 0, 0, 0, -1, 0, 0, 0, -17, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, -2, 0, 33, 0, 0, 0, 0, 0, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 45, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, -1, 0, 0, 0, 0, -41, 0, 0, -9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 0, -43, -9, 0, 3, 0, 0, 0, 0, -28, 0, 0, -4, 0, 0, 0, 0, 46, 0, 0, -19, 0, 0, -6, 0, 0, 33, 0, 0, 0, 0, -71, 0, 3, 37, 0, -13, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 41, 0, 0, 24, 0, 0, 0, 0, -59, 0, -36, 15, 0, 0, 0, 0, 0, 0, -5, -1, 0, 4, 0, 0, 43, 0, 37, 0, 0, 5, 0, 0, 2, 0, 1, 38, 0, 0, 0, 0, 0, 0, -71, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 37, 0, 0, 7, 0}

#define TENSOR_DENSE_1_KERNEL_0_DEC_BITS {7}

#define TENSOR_DENSE_1_BIAS_0 {103, 107, -3, 22, -11, -32, -10, 119, -27, 0, -5, 0, 0, -25, -18, 0}

#define TENSOR_DENSE_1_BIAS_0_DEC_BITS {10}

#define DENSE_1_BIAS_LSHIFT {4}

#define DENSE_1_OUTPUT_RSHIFT {8}

#define TENSOR_DENSE_2_KERNEL_0 {0, 38, 0, 0, 37, -22, 0, -1, -1, 1, 0, -24, 78, 0, 56, 74, 0, -14, 0, 1, -94, 0, 0, 0, 0, -60, 0, -2, 54, -2, 39, 31, 0, -23, 0, 0, 38, -5, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -9, 0, 0, -4, -15, 0, -13, 0, 0, 56, 49, 0, -1, 24, 0, 0, 0, 66, 0, 0, 0, 4, -1, -69, 0, -41, 0, 13, -1, 31, 0, 0, 0, 0, -1, 0, 0, -29, 0, 65, -1, 0, 0, -54, 0, -66, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -33, 0, 0, 0, 0, 0, -46, 0, 0, 0, 59, 0, 1, 0}

#define TENSOR_DENSE_2_KERNEL_0_DEC_BITS {7}

#define TENSOR_DENSE_2_BIAS_0 {0, 82, 64, -11, 69, 0, 20, 0}

#define TENSOR_DENSE_2_BIAS_0_DEC_BITS {10}

#define DENSE_2_BIAS_LSHIFT {3}

#define DENSE_2_OUTPUT_RSHIFT {7}

#define TENSOR_DENSE_3_KERNEL_0 {0, 0, 62, 88, 0, 0, 90, -1, 9, 8, 26, -26, 30, -2, 91, -1, 70, -10, 80, -61, -74, 0, -47, 0, 0, 0, -2, 0, 0, -1, 0, 0, 0, 73, 12, -42, 82, 1, 12, -3, 0, 91, 5, -18, 33, 0, 13, 0}

#define TENSOR_DENSE_3_KERNEL_0_DEC_BITS {7}

#define TENSOR_DENSE_3_BIAS_0 {-64, 118, 6, -24, 97, 4}

#define TENSOR_DENSE_3_BIAS_0_DEC_BITS {11}

#define DENSE_3_BIAS_LSHIFT {2}

#define DENSE_3_OUTPUT_RSHIFT {7}

#define TENSOR_DENSE_4_KERNEL_0 {-18, -44, 35, 79, -25, -46, 23, 77, 78, 32, 0, 0, 87, 4, 0, 0, -51, -71, 95, -30, -102, 38, -2, 7}

#define TENSOR_DENSE_4_KERNEL_0_DEC_BITS {7}

#define TENSOR_DENSE_4_BIAS_0 {-17, -40, 93, 13}

#define TENSOR_DENSE_4_BIAS_0_DEC_BITS {10}

#define DENSE_4_BIAS_LSHIFT {3}

#define DENSE_4_OUTPUT_RSHIFT {6}

#define TENSOR_DENSE_5_KERNEL_0 {77, 48, -87, 65, -92, -33, -8, -89}

#define TENSOR_DENSE_5_KERNEL_0_DEC_BITS {7}

#define TENSOR_DENSE_5_BIAS_0 {-90, 90}

#define TENSOR_DENSE_5_BIAS_0_DEC_BITS {10}

#define DENSE_5_BIAS_LSHIFT {4}

#define DENSE_5_OUTPUT_RSHIFT {7}


/* output q format for each layer */
#define DENSE_INPUT_OUTPUT_DEC 7
#define DENSE_INPUT_OUTPUT_OFFSET 0
#define DENSE_OUTPUT_DEC 7
#define DENSE_OUTPUT_OFFSET 0
#define RE_LU_OUTPUT_DEC 7
#define RE_LU_OUTPUT_OFFSET 0
#define DENSE_1_OUTPUT_DEC 6
#define DENSE_1_OUTPUT_OFFSET 0
#define RE_LU_1_OUTPUT_DEC 6
#define RE_LU_1_OUTPUT_OFFSET 0
#define DENSE_2_OUTPUT_DEC 6
#define DENSE_2_OUTPUT_OFFSET 0
#define RE_LU_2_OUTPUT_DEC 6
#define RE_LU_2_OUTPUT_OFFSET 0
#define DENSE_3_OUTPUT_DEC 6
#define DENSE_3_OUTPUT_OFFSET 0
#define RE_LU_3_OUTPUT_DEC 6
#define RE_LU_3_OUTPUT_OFFSET 0
#define DENSE_4_OUTPUT_DEC 7
#define DENSE_4_OUTPUT_OFFSET 0
#define RE_LU_4_OUTPUT_DEC 7
#define RE_LU_4_OUTPUT_OFFSET 0
#define DENSE_5_OUTPUT_DEC 7
#define DENSE_5_OUTPUT_OFFSET 0
#define ACTIVATION_OUTPUT_DEC 7
#define ACTIVATION_OUTPUT_OFFSET 0

/* bias shift and output shift for none-weighted layer */

/* tensors and configurations for each layer */
static int8_t nnom_input_data[2] = {0};

const nnom_shape_data_t tensor_dense_input_dim[] = {2};
const nnom_qformat_param_t tensor_dense_input_dec[] = {7};
const nnom_qformat_param_t tensor_dense_input_offset[] = {0};
const nnom_tensor_t tensor_dense_input = {
    .p_data = (void*)nnom_input_data,
    .dim = (nnom_shape_data_t*)tensor_dense_input_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_input_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_input_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_io_config_t dense_input_config = {
    .super = {.name = "dense_input"},
    .tensor = (nnom_tensor_t*)&tensor_dense_input
};
const int8_t tensor_dense_kernel_0_data[] = TENSOR_DENSE_KERNEL_0;

const nnom_shape_data_t tensor_dense_kernel_0_dim[] = {2, 32};
const nnom_qformat_param_t tensor_dense_kernel_0_dec[] = TENSOR_DENSE_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_kernel_0 = {
    .p_data = (void*)tensor_dense_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_bias_0_data[] = TENSOR_DENSE_BIAS_0;

const nnom_shape_data_t tensor_dense_bias_0_dim[] = {32};
const nnom_qformat_param_t tensor_dense_bias_0_dec[] = TENSOR_DENSE_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_bias_0 = {
    .p_data = (void*)tensor_dense_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_output_shift[] = DENSE_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_bias_shift[] = DENSE_BIAS_LSHIFT;
const nnom_dense_config_t dense_config = {
    .super = {.name = "dense"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_bias_shift
};
const int8_t tensor_dense_1_kernel_0_data[] = TENSOR_DENSE_1_KERNEL_0;

const nnom_shape_data_t tensor_dense_1_kernel_0_dim[] = {32, 16};
const nnom_qformat_param_t tensor_dense_1_kernel_0_dec[] = TENSOR_DENSE_1_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_1_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_1_kernel_0 = {
    .p_data = (void*)tensor_dense_1_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_1_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_1_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_1_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_1_bias_0_data[] = TENSOR_DENSE_1_BIAS_0;

const nnom_shape_data_t tensor_dense_1_bias_0_dim[] = {16};
const nnom_qformat_param_t tensor_dense_1_bias_0_dec[] = TENSOR_DENSE_1_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_1_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_1_bias_0 = {
    .p_data = (void*)tensor_dense_1_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_1_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_1_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_1_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_1_output_shift[] = DENSE_1_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_1_bias_shift[] = DENSE_1_BIAS_LSHIFT;
const nnom_dense_config_t dense_1_config = {
    .super = {.name = "dense_1"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_1_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_1_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_1_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_1_bias_shift
};
const int8_t tensor_dense_2_kernel_0_data[] = TENSOR_DENSE_2_KERNEL_0;

const nnom_shape_data_t tensor_dense_2_kernel_0_dim[] = {16, 8};
const nnom_qformat_param_t tensor_dense_2_kernel_0_dec[] = TENSOR_DENSE_2_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_2_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_2_kernel_0 = {
    .p_data = (void*)tensor_dense_2_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_2_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_2_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_2_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_2_bias_0_data[] = TENSOR_DENSE_2_BIAS_0;

const nnom_shape_data_t tensor_dense_2_bias_0_dim[] = {8};
const nnom_qformat_param_t tensor_dense_2_bias_0_dec[] = TENSOR_DENSE_2_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_2_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_2_bias_0 = {
    .p_data = (void*)tensor_dense_2_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_2_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_2_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_2_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_2_output_shift[] = DENSE_2_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_2_bias_shift[] = DENSE_2_BIAS_LSHIFT;
const nnom_dense_config_t dense_2_config = {
    .super = {.name = "dense_2"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_2_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_2_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_2_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_2_bias_shift
};
const int8_t tensor_dense_3_kernel_0_data[] = TENSOR_DENSE_3_KERNEL_0;

const nnom_shape_data_t tensor_dense_3_kernel_0_dim[] = {8, 6};
const nnom_qformat_param_t tensor_dense_3_kernel_0_dec[] = TENSOR_DENSE_3_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_3_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_3_kernel_0 = {
    .p_data = (void*)tensor_dense_3_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_3_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_3_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_3_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_3_bias_0_data[] = TENSOR_DENSE_3_BIAS_0;

const nnom_shape_data_t tensor_dense_3_bias_0_dim[] = {6};
const nnom_qformat_param_t tensor_dense_3_bias_0_dec[] = TENSOR_DENSE_3_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_3_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_3_bias_0 = {
    .p_data = (void*)tensor_dense_3_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_3_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_3_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_3_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_3_output_shift[] = DENSE_3_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_3_bias_shift[] = DENSE_3_BIAS_LSHIFT;
const nnom_dense_config_t dense_3_config = {
    .super = {.name = "dense_3"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_3_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_3_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_3_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_3_bias_shift
};
const int8_t tensor_dense_4_kernel_0_data[] = TENSOR_DENSE_4_KERNEL_0;

const nnom_shape_data_t tensor_dense_4_kernel_0_dim[] = {6, 4};
const nnom_qformat_param_t tensor_dense_4_kernel_0_dec[] = TENSOR_DENSE_4_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_4_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_4_kernel_0 = {
    .p_data = (void*)tensor_dense_4_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_4_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_4_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_4_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_4_bias_0_data[] = TENSOR_DENSE_4_BIAS_0;

const nnom_shape_data_t tensor_dense_4_bias_0_dim[] = {4};
const nnom_qformat_param_t tensor_dense_4_bias_0_dec[] = TENSOR_DENSE_4_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_4_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_4_bias_0 = {
    .p_data = (void*)tensor_dense_4_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_4_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_4_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_4_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_4_output_shift[] = DENSE_4_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_4_bias_shift[] = DENSE_4_BIAS_LSHIFT;
const nnom_dense_config_t dense_4_config = {
    .super = {.name = "dense_4"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_4_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_4_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_4_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_4_bias_shift
};
const int8_t tensor_dense_5_kernel_0_data[] = TENSOR_DENSE_5_KERNEL_0;

const nnom_shape_data_t tensor_dense_5_kernel_0_dim[] = {4, 2};
const nnom_qformat_param_t tensor_dense_5_kernel_0_dec[] = TENSOR_DENSE_5_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_5_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_5_kernel_0 = {
    .p_data = (void*)tensor_dense_5_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_5_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_5_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_5_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_5_bias_0_data[] = TENSOR_DENSE_5_BIAS_0;

const nnom_shape_data_t tensor_dense_5_bias_0_dim[] = {2};
const nnom_qformat_param_t tensor_dense_5_bias_0_dec[] = TENSOR_DENSE_5_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_5_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_5_bias_0 = {
    .p_data = (void*)tensor_dense_5_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_5_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_5_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_5_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_5_output_shift[] = DENSE_5_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_5_bias_shift[] = DENSE_5_BIAS_LSHIFT;
const nnom_dense_config_t dense_5_config = {
    .super = {.name = "dense_5"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_5_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_5_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_5_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_5_bias_shift
};
static int8_t nnom_output_data[2] = {0};

const nnom_shape_data_t tensor_output0_dim[] = {2};
const nnom_qformat_param_t tensor_output0_dec[] = {ACTIVATION_OUTPUT_DEC};
const nnom_qformat_param_t tensor_output0_offset[] = {0};
const nnom_tensor_t tensor_output0 = {
    .p_data = (void*)nnom_output_data,
    .dim = (nnom_shape_data_t*)tensor_output0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_output0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_output0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_io_config_t output0_config = {
    .super = {.name = "output0"},
    .tensor = (nnom_tensor_t*)&tensor_output0
};
/* model version */
#define NNOM_MODEL_VERSION (10000*0 + 100*4 + 3)

/* nnom model */
static nnom_model_t* nnom_model_create(void)
{
	static nnom_model_t model;
	nnom_layer_t* layer[14];

	check_model_version(NNOM_MODEL_VERSION);
	new_model(&model);

	layer[0] = input_s(&dense_input_config);
	layer[1] = model.hook(dense_s(&dense_config), layer[0]);
	layer[2] = model.active(act_relu(), layer[1]);
	layer[3] = model.hook(dense_s(&dense_1_config), layer[2]);
	layer[4] = model.active(act_relu(), layer[3]);
	layer[5] = model.hook(dense_s(&dense_2_config), layer[4]);
	layer[6] = model.active(act_relu(), layer[5]);
	layer[7] = model.hook(dense_s(&dense_3_config), layer[6]);
	layer[8] = model.active(act_relu(), layer[7]);
	layer[9] = model.hook(dense_s(&dense_4_config), layer[8]);
	layer[10] = model.active(act_relu(), layer[9]);
	layer[11] = model.hook(dense_s(&dense_5_config), layer[10]);
	layer[12] = model.hook(Softmax(), layer[11]);
	layer[13] = model.hook(output_s(&output0_config), layer[12]);
	model_compile(&model, layer[0], layer[13]);
	return &model;
}
