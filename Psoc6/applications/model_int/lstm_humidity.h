#include "nnom.h"

/* Weights, bias and Q format */
#define HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_KERNEL_0 {68, -53, 10, -29, -4, 43, 11, 55, 85, -22, -27, 33, 24, -18, -26, -14, 75, -6, 53, 6, -48, -38, 1, -38, 36, 28, 41, -20, 57, 27, -32, 3}

#define HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_KERNEL_0_DEC_BITS {7}

#define HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_RECURRENT_KERNEL_0 {62, -41, -41, 18, -1, -27, -60, 19, 9, 29, 10, -7, 32, -31, 32, -31, -47, 21, -38, -56, -11, 9, -29, 11, -13, 11, -79, 44, 12, -12, -15, 57, 25, -15, 19, -8, -46, 7, -4, -25, 7, -5, 16, -35, -13, 33, 13, -21, -58, -13, 10, 5, 20, -18, 19, -11, 62, -17, -8, 28, 63, -5, 22, -35, 38, 5, -52, -8, 25, 22, -10, -16, 31, 39, 34, 32, 14, 23, 8, -32, -14, 18, -7, 34, -12, -1, 19, 9, -25, 9, -25, 34, -15, 21, -6, -14, 1, -40, -12, -10, -28, -6, 26, -30, 4, -10, -9, 3, 14, 3, 22, 14, 25, 78, -13, 27, 32, 37, -35, 7, 57, -6, -18, 10, 3, 42, 15, -2, -2, 8, -65, -49, 41, -5, -32, -1, -5, -5, 39, 21, 15, -27, 41, -6, -33, -4, 12, -6, -36, 34, -52, -29, 45, 30, -44, 0, 21, -9, 36, -25, 7, 17, 40, 40, 19, 24, 60, -2, -14, 41, 22, 20, -17, 16, 12, -71, 20, -21, -1, -1, 39, 20, -12, -11, 7, -41, -22, -55, -34, -11, 10, -13, -45, -21, -4, -9, -7, 13, -50, 39, 31, -5, -2, -30, -5, -7, 21, -23, -60, 24, -39, 8, -37, 37, -38, 1, 24, 2, -54, 34, -15, 22, -17, 23, 30, 11, -27, 2, -20, 14, -23, -41, -2, 4, 36, -9, 41, -19, 41, 19, -28, 3, -13, 8, 11, -15, -13, -48, 26, 24, -39, 40, 21, 18, -3, -1}

#define HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_RECURRENT_KERNEL_0_DEC_BITS {7}

#define HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_BIAS_0 {10, -12, 8, -9, 7, -5, -9, 8, 67, 54, 68, 56, 68, 57, 54, 68, -7, 3, -1, -1, -1, 2, 0, -1, 10, -14, 6, -9, 5, -4, -9, 7}

#define HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_BIAS_0_DEC_BITS {6}

#define HUMIDITY_LSTM_BIAS_LSHIFT {8}

#define HUMIDITY_LSTM_OUTPUT_RSHIFT {7}

#define HUMIDITY_TENSOR_DENSE_KERNEL_0 {41, 58, 93, -46, -46, -8, -32, -79}

#define HUMIDITY_TENSOR_DENSE_KERNEL_0_DEC_BITS {7}

#define HUMIDITY_TENSOR_DENSE_BIAS_0 {66}

#define HUMIDITY_TENSOR_DENSE_BIAS_0_DEC_BITS {11}

#define HUMIDITY_DENSE_BIAS_LSHIFT {3}

#define HUMIDITY_DENSE_OUTPUT_RSHIFT {7}

/* output q format for each layer */
#define HUMIDITY_INPUT_1_OUTPUT_DEC 7
#define HUMIDITY_INPUT_1_OUTPUT_OFFSET 0
#define HUMIDITY_LSTM_OUTPUT_DEC 7
#define HUMIDITY_LSTM_OUTPUT_OFFSET 0
#define HUMIDITY_DENSE_OUTPUT_DEC 7
#define HUMIDITY_DENSE_OUTPUT_OFFSET 0

/* bias shift and output shift for none-weighted layer */

/* tensors and configurations for each layer */
static int8_t humidity_nnom_input_data[6] = {0};

const nnom_shape_data_t humidity_tensor_input_1_dim[] = {6, 1};
const nnom_qformat_param_t humidity_tensor_input_1_dec[] = {7};
const nnom_qformat_param_t humidity_tensor_input_1_offset[] = {0};
const nnom_tensor_t humidity_tensor_input_1 = {
    .p_data = (void *)humidity_nnom_input_data,
    .dim = (nnom_shape_data_t *)humidity_tensor_input_1_dim,
    .q_dec = (nnom_qformat_param_t *)humidity_tensor_input_1_dec,
    .q_offset = (nnom_qformat_param_t *)humidity_tensor_input_1_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8};

const nnom_io_config_t humidity_input_1_config = {
    .super = {.name = "input_1"},
    .tensor = (nnom_tensor_t *)&humidity_tensor_input_1};
const int8_t humidity_tensor_lstm_lstm_cell_1_kernel_0_data[] = HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_KERNEL_0;

const nnom_shape_data_t humidity_tensor_lstm_lstm_cell_1_kernel_0_dim[] = {1, 32};
const nnom_qformat_param_t humidity_tensor_lstm_lstm_cell_1_kernel_0_dec[] = HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t humidity_tensor_lstm_lstm_cell_1_kernel_0_offset[] = {0};
const nnom_tensor_t humidity_tensor_lstm_lstm_cell_1_kernel_0 = {
    .p_data = (void *)humidity_tensor_lstm_lstm_cell_1_kernel_0_data,
    .dim = (nnom_shape_data_t *)humidity_tensor_lstm_lstm_cell_1_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t *)humidity_tensor_lstm_lstm_cell_1_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t *)humidity_tensor_lstm_lstm_cell_1_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8};
const int8_t humidity_tensor_lstm_lstm_cell_1_recurrent_kernel_0_data[] = HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_RECURRENT_KERNEL_0;

const nnom_shape_data_t humidity_tensor_lstm_lstm_cell_1_recurrent_kernel_0_dim[] = {8, 32};
const nnom_qformat_param_t humidity_tensor_lstm_lstm_cell_1_recurrent_kernel_0_dec[] = HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_RECURRENT_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t humidity_tensor_lstm_lstm_cell_1_recurrent_kernel_0_offset[] = {0};
const nnom_tensor_t humidity_tensor_lstm_lstm_cell_1_recurrent_kernel_0 = {
    .p_data = (void *)humidity_tensor_lstm_lstm_cell_1_recurrent_kernel_0_data,
    .dim = (nnom_shape_data_t *)humidity_tensor_lstm_lstm_cell_1_recurrent_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t *)humidity_tensor_lstm_lstm_cell_1_recurrent_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t *)humidity_tensor_lstm_lstm_cell_1_recurrent_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8};
const int8_t humidity_tensor_lstm_lstm_cell_1_bias_0_data[] = HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_BIAS_0;

const nnom_shape_data_t humidity_tensor_lstm_lstm_cell_1_bias_0_dim[] = {32};
const nnom_qformat_param_t humidity_tensor_lstm_lstm_cell_1_bias_0_dec[] = HUMIDITY_TENSOR_LSTM_LSTM_CELL_1_BIAS_0_DEC_BITS;
const nnom_qformat_param_t humidity_tensor_lstm_lstm_cell_1_bias_0_offset[] = {0};
const nnom_tensor_t humidity_tensor_lstm_lstm_cell_1_bias_0 = {
    .p_data = (void *)humidity_tensor_lstm_lstm_cell_1_bias_0_data,
    .dim = (nnom_shape_data_t *)humidity_tensor_lstm_lstm_cell_1_bias_0_dim,
    .q_dec = (nnom_qformat_param_t *)humidity_tensor_lstm_lstm_cell_1_bias_0_dec,
    .q_offset = (nnom_qformat_param_t *)humidity_tensor_lstm_lstm_cell_1_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8};

const nnom_lstm_cell_config_t humidity_lstm_lstm_cell_config = {
    .super = {.name = "lstm"},
    .weights = (nnom_tensor_t *)&humidity_tensor_lstm_lstm_cell_1_kernel_0,
    .recurrent_weights = (nnom_tensor_t *)&humidity_tensor_lstm_lstm_cell_1_recurrent_kernel_0,
    .bias = (nnom_tensor_t *)&humidity_tensor_lstm_lstm_cell_1_bias_0,
    .q_dec_z = 6,
    .q_dec_h = 7,
    .q_dec_c = 6,
    .units = 8};

const nnom_rnn_config_t humidity_lstm_config = {
    .super = {.name = "lstm"},
    .return_sequence = false,
    .stateful = false,
    .go_backwards = false};
const int8_t humidity_tensor_dense_kernel_0_data[] = HUMIDITY_TENSOR_DENSE_KERNEL_0;

const nnom_shape_data_t humidity_tensor_dense_kernel_0_dim[] = {8, 1};
const nnom_qformat_param_t humidity_tensor_dense_kernel_0_dec[] = HUMIDITY_TENSOR_DENSE_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t humidity_tensor_dense_kernel_0_offset[] = {0};
const nnom_tensor_t humidity_tensor_dense_kernel_0 = {
    .p_data = (void *)humidity_tensor_dense_kernel_0_data,
    .dim = (nnom_shape_data_t *)humidity_tensor_dense_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t *)humidity_tensor_dense_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t *)humidity_tensor_dense_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8};
const int8_t humidity_tensor_dense_bias_0_data[] = HUMIDITY_TENSOR_DENSE_BIAS_0;

const nnom_shape_data_t humidity_tensor_dense_bias_0_dim[] = {1};
const nnom_qformat_param_t humidity_tensor_dense_bias_0_dec[] = HUMIDITY_TENSOR_DENSE_BIAS_0_DEC_BITS;
const nnom_qformat_param_t humidity_tensor_dense_bias_0_offset[] = {0};
const nnom_tensor_t humidity_tensor_dense_bias_0 = {
    .p_data = (void *)humidity_tensor_dense_bias_0_data,
    .dim = (nnom_shape_data_t *)humidity_tensor_dense_bias_0_dim,
    .q_dec = (nnom_qformat_param_t *)humidity_tensor_dense_bias_0_dec,
    .q_offset = (nnom_qformat_param_t *)humidity_tensor_dense_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8};

const nnom_qformat_param_t humidity_dense_output_shift[] = HUMIDITY_DENSE_OUTPUT_RSHIFT;
const nnom_qformat_param_t humidity_dense_bias_shift[] = HUMIDITY_DENSE_BIAS_LSHIFT;
const nnom_dense_config_t humidity_dense_config = {
    .super = {.name = "dense"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t *)&humidity_tensor_dense_kernel_0,
    .bias = (nnom_tensor_t *)&humidity_tensor_dense_bias_0,
    .output_shift = (nnom_qformat_param_t *)&humidity_dense_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&humidity_dense_bias_shift};
static int8_t humidity_nnom_output_data[1] = {0};

const nnom_shape_data_t humidity_tensor_output0_dim[] = {1};
const nnom_qformat_param_t humidity_tensor_output0_dec[] = {HUMIDITY_DENSE_OUTPUT_DEC};
const nnom_qformat_param_t humidity_tensor_output0_offset[] = {0};
const nnom_tensor_t humidity_tensor_output0 = {
    .p_data = (void *)humidity_nnom_output_data,
    .dim = (nnom_shape_data_t *)humidity_tensor_output0_dim,
    .q_dec = (nnom_qformat_param_t *)humidity_tensor_output0_dec,
    .q_offset = (nnom_qformat_param_t *)humidity_tensor_output0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8};

const nnom_io_config_t humidity_output0_config = {
    .super = {.name = "output0"},
    .tensor = (nnom_tensor_t *)&humidity_tensor_output0};
/* model version */
#define HUMIDITY_NNOM_MODEL_VERSION (10000 * 0 + 100 * 4 + 3)

/* nnom model */
static nnom_model_t *humidity_nnom_model_create(void)
{
    static nnom_model_t model;
    nnom_layer_t *layer[4];

    check_model_version(HUMIDITY_NNOM_MODEL_VERSION);
    new_model(&model);

    layer[0] = input_s(&humidity_input_1_config);
    layer[1] = model.hook(rnn_s(lstm_cell_s(&humidity_lstm_lstm_cell_config), &humidity_lstm_config), layer[0]);
    layer[2] = model.hook(dense_s(&humidity_dense_config), layer[1]);
    layer[3] = model.hook(output_s(&humidity_output0_config), layer[2]);
    model_compile(&model, layer[0], layer[3]);
    return &model;
}
