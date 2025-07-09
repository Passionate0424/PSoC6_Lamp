#include "nnom.h"

/* Weights, bias and Q format */
#define TENSOR_DENSE_KERNEL_0 {48, 28, -70, -34, 0, 22, 0, 15, 26, 26, 27, -8, 0, 3, 0, 20, -27, 25, 0, 20, 0, -20, 0, 0, 8, 0, -13, 0, 0, 24, 0, 26, -26, 0, 30, 0, 0, -42, 12, 0, 27, 0, -1, 0, 19, -95, 36, -6, 11, 0, 4, 0, -10, 33, -22, 2, -12, 0, 16, 1, 44, 0, -61, 0, -7, -23, 31, 35, -39, 0, 1, 0, -14, -51, 20, -4, 3, 0, -109, 46, 0, -17, 0, -50, 0, -17, 0, 38, 0, 20, 0, 13, 0, 20, 0, 15, 0, 30, 0, 72, 53, 12, 3, 22, 29, 6, -16, 20, -9, 4, -40, -69, -49, -55, 35, 18, 40, 36, 4, -18, -4, 25, -49, -34, 24, -21, 84, 82, 45, 28, -35, -77, 20, 16, 7, -10, 36, 6, 7, -11, 3, 0, 95, 0, -8, 32, -10, 42, 0, 0, 2, 0, 2, -78, 11, -4, -4, 0, 71, -3}

#define TENSOR_DENSE_KERNEL_0_DEC_BITS {6}

#define TENSOR_DENSE_BIAS_0 {6, -14, 0, -42, 0, 26, -21, -3, -27, -3, 7, 31, 10, 0, 39, -16, 0, -7, -6, -28, -50, 20, -45, 34, 1, 42, -55, -21, -73, 0, -19, -3}

#define TENSOR_DENSE_BIAS_0_DEC_BITS {7}

#define DENSE_BIAS_LSHIFT {6}

#define DENSE_OUTPUT_RSHIFT {7}

#define TENSOR_DENSE_1_KERNEL_0 {9, 4, 0, 0, 8, -5, 0, 0, 8, -1, 2, 11, -6, -4, -12, 10, 0, 0, 8, 4, 0, 0, -13, 13, -11, -6, 0, 0, 10, 3, 0, 0, -6, 2, -14, -15, 3, -3, 3, -2, 0, 0, -5, -15, 0, 0, 13, -9, -16, -16, 3, -2, 5, -28, 38, -31, 0, 0, 5, -7, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 4, 10, -2, 12, -2, 9, -32, 8, -58, -10, 14, 8, 0, 23, -6, 2, -15, -26, -3, 18, 0, -24, -6, -4, -3, -10, -17, -13, 9, -23, 19, -1, -43, -3, -10, -13, 3, 9, 8, -26, -4, -17, 12, 13, 2, -28, -31, 10, 0, 0, -22, -6, 0, 0, -7, -1, -10, 0, 0, 0, 6, 20, 0, 0, -7, 0, 0, 0, 8, 16, 10, 4, 0, 0, -9, 0, 0, 0, 1, -1, 2, 0, 0, 0, 4, 13, 0, 0, -4, 0, -1, 0, -5, 10, -4, -9, 0, 0, -22, 0, 0, 0, -11, 0, -32, 0, -20, 0, 13, -10, 0, 1, 0, 0, -10, 0, 0, 0, -4, -7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 14, 0, 1, 1, -11, 0, -11, 0, 14, 6, 21, -7, 3, 0, -8, 0, 12, -4, -9, -20, -9, 0, 11, 0, 19, 3, 1, 7, 9, 0, -16, 0, -18, -1, 35, -11, 0, 0, 11, 0, 57, -16, 10, -2, 0, 0, -22, 0, 0, 0, 2, -38, 0, 0, 0, 0, -8, 4, 0, 0, 0, 0, 0, 0, -12, -6, -4, -9, 0, 0, 0, 0, 0, 0, 2, -2, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 0, -6, -1, -9, 0, 0, 0, 0, 0, 0, 0, 17, 16, 0, 0, 0, 0, 0, 7, 29, 2, 0, 0, 0, 0, 0, 0, 25, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 19, 8, -5, 0, 0, 0, 0, -23, -14, 32, 7, 0, 0, 0, 0, 3, 22, -8, -6, 0, 0, 0, 0, -9, -14, -2, 2, 0, 0, 0, 0, -5, 6, 11, -6, 0, 0, 0, 0, -16, -25, 9, 9, 0, 0, 0, 0, 0, 0, 5, -2, 6, -7, 0, 0, -61, 11, 0, 0, -2, -3, -11, -2, 13, 7, 2, 16, 0, 0, -9, -9, 0, 0, 6, 7, 14, -17, 0, 0, -8, -13, 0, 0, 0, 5, 0, -2, 0, 6, 1, 7, 0, 0, 17, 7, 0, 0, -26, -38, 4, -6, 27, 21, 6, 6, 0, -19, 0, 0, 0, 15, 0, 0, 10, 16, 0, 0, 0, 0, 0, 0, 0, 0, -3, 4, -34, 18, 6, -7, 3, 5, 0, -17, -6, 16, -11, 11, 8, 9, 0, -32, 1, -7, 18, -18, -17, 15, 9, 12, 19, 0, 13, -21, -51, 19, 3, -24, 7, 7, 7, -16, -9, 15, 1, 91, -33, 6, 0, 8, 10, -1, 0, 0, -9, -2, 0, 0, -1, 8}

#define TENSOR_DENSE_1_KERNEL_0_DEC_BITS {5}

#define TENSOR_DENSE_1_BIAS_0 {27, 3, -36, -33, -4, -5, -47, -10, -11, -1, 0, 41, -38, -22, -65, -35}

#define TENSOR_DENSE_1_BIAS_0_DEC_BITS {8}

#define DENSE_1_BIAS_LSHIFT {3}

#define DENSE_1_OUTPUT_RSHIFT {6}

#define TENSOR_DENSE_2_KERNEL_0 {-9, 49, -20, 73, 12, 0, -31, 0, -3, 39, 17, -54, -41, 0, -42, 0, 3, -35, -28, 38, -72, 0, 37, 0, 0, 0, 33, 27, 0, 0, 10, 0, 0, 0, 32, 11, 0, 0, 29, 0, 0, 0, 12, 2, 0, 0, 23, 0, -18, 72, -30, 96, -30, 0, -61, 0, -17, 50, -1, -111, -71, 0, -2, 0, 54, 0, -4, 0, 38, -20, 36, -34, -30, 0, -23, 0, 26, 35, -39, 31, -21, 0, 61, 0, 19, 5, 9, -28, 0, 0, 4, 0, 0, 0, -31, 45, 0, 0, 20, 0, 0, 0, 26, 56, 0, 0, 10, 0, 0, 0, -4, 32, 2, 0, 41, 0, 32, -31, 43, -56, 61, 0, 6, 0, 44, -35, -22, -15}

#define TENSOR_DENSE_2_KERNEL_0_DEC_BITS {6}

#define TENSOR_DENSE_2_BIAS_0 {43, -53, 77, -11, -39, 0, -9, 64}

#define TENSOR_DENSE_2_BIAS_0_DEC_BITS {8}

#define DENSE_2_BIAS_LSHIFT {3}

#define DENSE_2_OUTPUT_RSHIFT {7}

#define TENSOR_DENSE_3_KERNEL_0 {36, 0, 97, 0, 0, 28, 0, 56, -123, 0, 0, 0, 0, -127, 0, 0, -47, 0, -63, 0, 0, -81, 0, -33, 0, 0, 72, 0, 0, 0, 0, 53}

#define TENSOR_DENSE_3_KERNEL_0_DEC_BITS {6}

#define TENSOR_DENSE_3_BIAS_0 {97, -10, -4, 124}

#define TENSOR_DENSE_3_BIAS_0_DEC_BITS {9}

#define DENSE_3_BIAS_LSHIFT {1}

#define DENSE_3_OUTPUT_RSHIFT {7}

#define TENSOR_DENSE_4_KERNEL_0 {-95, -28, 24, -60, 100, -20, 24, 118}

#define TENSOR_DENSE_4_KERNEL_0_DEC_BITS {5}

#define TENSOR_DENSE_4_BIAS_0 {115, -115}

#define TENSOR_DENSE_4_BIAS_0_DEC_BITS {5}

#define DENSE_4_BIAS_LSHIFT {3}

#define DENSE_4_OUTPUT_RSHIFT {5}


/* output q format for each layer */
#define INPUT_1_OUTPUT_DEC 7
#define INPUT_1_OUTPUT_OFFSET 0
#define DENSE_OUTPUT_DEC 6
#define DENSE_OUTPUT_OFFSET 0
#define RE_LU_OUTPUT_DEC 6
#define RE_LU_OUTPUT_OFFSET 0
#define DENSE_1_OUTPUT_DEC 5
#define DENSE_1_OUTPUT_OFFSET 0
#define RE_LU_1_OUTPUT_DEC 5
#define RE_LU_1_OUTPUT_OFFSET 0
#define DENSE_2_OUTPUT_DEC 4
#define DENSE_2_OUTPUT_OFFSET 0
#define RE_LU_2_OUTPUT_DEC 4
#define RE_LU_2_OUTPUT_OFFSET 0
#define DENSE_3_OUTPUT_DEC 3
#define DENSE_3_OUTPUT_OFFSET 0
#define RE_LU_3_OUTPUT_DEC 3
#define RE_LU_3_OUTPUT_OFFSET 0
#define DENSE_4_OUTPUT_DEC 3
#define DENSE_4_OUTPUT_OFFSET 0
#define ACTIVATION_OUTPUT_DEC 7
#define ACTIVATION_OUTPUT_OFFSET 0

/* bias shift and output shift for none-weighted layer */

/* tensors and configurations for each layer */
static int8_t nnom_input_data[5] = {0};

const nnom_shape_data_t tensor_input_1_dim[] = {5};
const nnom_qformat_param_t tensor_input_1_dec[] = {7};
const nnom_qformat_param_t tensor_input_1_offset[] = {0};
const nnom_tensor_t tensor_input_1 = {
    .p_data = (void*)nnom_input_data,
    .dim = (nnom_shape_data_t*)tensor_input_1_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_input_1_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_input_1_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_io_config_t input_1_config = {
    .super = {.name = "input_1"},
    .tensor = (nnom_tensor_t*)&tensor_input_1
};
const int8_t tensor_dense_kernel_0_data[] = TENSOR_DENSE_KERNEL_0;

const nnom_shape_data_t tensor_dense_kernel_0_dim[] = {5, 32};
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

const nnom_shape_data_t tensor_dense_3_kernel_0_dim[] = {8, 4};
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

const nnom_shape_data_t tensor_dense_3_bias_0_dim[] = {4};
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

const nnom_shape_data_t tensor_dense_4_kernel_0_dim[] = {4, 2};
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

const nnom_shape_data_t tensor_dense_4_bias_0_dim[] = {2};
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
	nnom_layer_t* layer[12];

	check_model_version(NNOM_MODEL_VERSION);
	new_model(&model);

	layer[0] = input_s(&input_1_config);
	layer[1] = model.hook(dense_s(&dense_config), layer[0]);
	layer[2] = model.active(act_relu(), layer[1]);
	layer[3] = model.hook(dense_s(&dense_1_config), layer[2]);
	layer[4] = model.active(act_relu(), layer[3]);
	layer[5] = model.hook(dense_s(&dense_2_config), layer[4]);
	layer[6] = model.active(act_relu(), layer[5]);
	layer[7] = model.hook(dense_s(&dense_3_config), layer[6]);
	layer[8] = model.active(act_relu(), layer[7]);
	layer[9] = model.hook(dense_s(&dense_4_config), layer[8]);
	layer[10] = model.hook(Softmax(), layer[9]);
	layer[11] = model.hook(output_s(&output0_config), layer[10]);
	model_compile(&model, layer[0], layer[11]);
	return &model;
}
