#include "nnom.h"

/* Weights, bias and Q format */
#define ENV_TENSOR_DENSE_KERNEL_0 {-18, 0, -35, 0, 0, 4, 0, 39, 12, 0, 25, 0, 0, 18, 0, 8, 9, 0, 0, -26, 0, 10, 0, 9, -65, 0, -5, 0, 0, 27, 0, -25, -15, 0, 6, 0, 0, 11, -6, 0, 2, -41, -21, -14, 0, 0, 0, 0, -1, 25, -12, 4, 0, 0, 0, 0, -27, 21, 0, 0, -23, 8, -31, 42, -4, -1, -15, -2, -8, -23, 14, 12, -16, -1, 7, 3, 18, -1, 6, -102, 9, 0, -22, 0, -25, -23, -16, -18, -2, 0, 5, 0, 5, -70, 2, 24, -13, 0, 30, 4, -70, 0, -30, 0, 31, -2, -19, -10, 14, 0, 23, 0, -34, -1, -5, -7, 5, 0, 20, 3, 0, -13, 0, -19, 0, -25, 0, 13, 0, 10, 0, -12, 0, -17, 0, 17, 0, 27, 0, 27, 0, -34, 0, 55, -2, 1, 45, -5, 0, -3, 0, 7, 10, 12, -12, 14, 0, 2, -7, 21, 0, 0, 0, 0, 2, 23, -9, -8, 0, 0, 0, 0, -66, -17, -24, -29, 0, 0, 20, -2, 5, 12, -22, -10, 0, 0, 0, 0, -6, 33, 5, 5, 0, 0, 0, 0, 8, -20, 0, 0, 0, -2, 0, -8, 20, -5, -29, -2, 0, 1, 0, -50, -5, -58, -2, 0, 0, 14, -3, -22, 0, 22, 0, 28, -6, -1, 13, 35, 0, 6, 0, -5, 11, -9, -39, 12, 0, 15, 15, 23, 1, -6, 25, 11, -43, 0, 5, 0, 10, -31, 13, 15, 2, 0, -12, 0, -51, -19, 5, 0, 30, 0, -21, 0, 20, 4, -23, -13, -15, 0, -21, 0, -11, 4, 27, -1, -7, 0, 4, -23, 2, 0, 29, 0, -5, 6, 12, 31, -23, 0, -7, 0, 15, -14, 18, 9, 7, 0, 15, 1, -15, 11, 13, -7, -15, 8, 0, 35, 0, 2, 20, -4, 17, -7, -22, -6, 7, 12, 17, -14}

#define ENV_TENSOR_DENSE_KERNEL_0_DEC_BITS {5}

#define ENV_TENSOR_DENSE_BIAS_0 {-30, -15, -2, -39, -9, 2, 56, 0, 87, -5, 0, -1, 2, -64, 13, 78, 44, 0, 2, -1, -2, -9, -15, 9, -25, 11, -6, -48, 0, -9, 6, -80, -7, -4, 64, 35, 19, -4, 0, 0, -2, 33, 6, 63, 0, -56, 56, -60, -30, 31, 74, -27, 41, -2, -78, 32, 14, 0, -84, -28, -28, -21, 32, 1}

#define ENV_TENSOR_DENSE_BIAS_0_DEC_BITS {7}

#define ENV_DENSE_BIAS_LSHIFT {5}

#define ENV_DENSE_OUTPUT_RSHIFT {7}

#define ENV_TENSOR_DENSE_1_KERNEL_0 {0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 12, 0, 0, 0, -12, 0, 0, 0, 38, 0, 4, 0, 0, 0, 3, 0, 0, 0, 19, 0, 0, 0, 17, 0, 0, 0, 10, 0, 0, 0, 17, 0, 0, 0, 5, 0, -5, 0, 3, 0, -4, 0, 10, 0, 0, 0, 2, 0, -49, 0, -12, 0, 4, 0, -2, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, -7, 0, 0, 0, -33, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 4, 0, 6, 0, -3, 0, 0, 0, -12, 0, 0, 0, 16, 0, -25, 0, 19, 0, 19, 0, 4, 0, 0, 0, -5, 0, 0, 0, -10, 0, 0, 0, -9, 0, 0, 0, -1, 0, -1, 0, 0, 0, -8, 0, 0, 0, -19, 0, 0, 0, -7, 0, 0, 0, 0, 0, -15, 0, 0, 0, -3, 0, 10, 0, -12, 0, 7, 0, 14, 0, 0, 0, 17, 0, 0, 0, 6, 0, 9, 0, 11, 0, -1, 0, 9, 0, -1, 0, 13, 0, -14, 0, 24, 0, 8, 0, 0, 0, 1, 0, 0, 0, -12, 0, 22, 0, -28, 0, -4, 0, 0, 0, 1, 0, 0, 0, 6, 0, -7, 0, 24, 0, -3, 0, 10, 0, 0, 0, -4, 0, 0, 0, 11, 0, 16, 0, 13, 0, -4, 0, 10, 0, 3, 0, -18, 0, -15, 0, 2, 1, -5, 0, 0, -27, -9, 0, 0, 0, 0, -7, 3, 0, 0, -5, 12, 0, 0, -5, -4, 0, 0, -9, 0, -6, -1, 0, 0, 16, 14, 0, 0, -2, 12, 0, 0, 11, -8, 0, 0, -8, -8, 0, 0, -27, -20, 0, 0, -13, -2, -13, 9, -19, 7, 0, 2, -42, -2, -69, -33, 6, 4, -21, -7, 2, 8, -2, -7, -2, -2, 12, 0, 0, 0, -22, 6, 0, 0, 0, -13, 19, 13, -5, 20, -1, 0, 2, 27, 0, 0, 1, 2, 0, 0, 0, -3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, -4, 4, 2, -13, 29, 24, 0, 0, -4, -7, 0, 0, 5, 12, -9, -5, -2, 6, 0, -27, 7, -4, 0, 0, 1, -6, 0, 0, -41, 35, 0, 0, -13, 12, 0, 0, -9, 0, 1, 4, 0, 0, 1, -2, 0, 0, 0, -7, 0, 0, -4, 11, 0, 0, 0, 0, -6, 7, 0, 0, -1, -3, 23, 2, -18, 11, -32, -8, -8, -2, 0, 0, 9, 17, 0, 0, 8, -14, -16, -18, 3, 5, 3, 11, -10, 13, -19, -8, -14, 2, 3, -2, 23, -18, 9, 5, 0, 0, -43, 4, 0, 0, -12, 7, -27, -10, 13, 2, 12, -16, 0, 0, 0, 4, 0, 0, -10, -2, 1, -14, -11, 9, -1, 12, -9, -8, 0, 0, 3, 0, 0, 0, 0, 10, 5, -12, 6, -10, -17, 1, 11, -16, -4, 3, 9, -10, 3, 13, 8, 6, 4, 0, 0, 0, 0, 10, 0, 0, 0, 0, 9, 0, 0, 0, 0, 9, 0, 0, 13, 0, 0, 0, 0, -4, 7, 0, 0, 0, 0, -14, 0, 0, 21, 0, 0, 0, 0, -1, 0, 0, 3, 0, 0, 0, 0, -1, 0, 0, 4, 0, -10, 0, 0, 3, 0, 1, -1, 0, 62, 0, 0, -1, 0, -36, 5, 0, 2, 0, 0, 5, 0, -25, 0, 0, 12, 0, 0, 0, 0, -5, 20, 0, -19, 0, 0, 9, 0, -6, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, -6, 0, 0, -29, 0, 11, 0, 0, -7, 0, 0, 0, 0, 12, 6, 0, -18, 0, 0, -17, 0, 17, 0, 0, -16, 0, 0, 0, 0, 29, 0, 0, 3, 0, 0, 0, 0, 0, -6, 0, 0, 0, 0, -1, 0, 0, -5, 0, 0, 0, 0, 20, 0, 0, 0, 0, -3, 0, 0, 0, 0, -4, -27, 0, 15, 0, 0, 17, 0, 26, 0, 0, 2, 0, 0, 0, 0, -2, -23, 0, -43, 0, 0, -12, 0, 16, 21, 0, 18, 0, 0, -18, 0, 1, -1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 7, 0, 0, -15, 0, 3, 0, 0, 7, 0, 0, 0, 0, 1, -17, 0, 3, 0, 0, -8, 0, 13, 0, 0, -16, 0, 0, 0, 0, 5, -23, 0, 7, 0, 0, 14, 0, -12, -6, 0, 14, 0, 0, -3, 0, -1, -1, 0, 0, 0, 7, -1, 0, 0, 0, 0, -2, 0, 0, 0, -29, 7, 0, 0, 4, 0, 0, 0, 27, -3, -15, 0, 0, 0, 12, -27, 0, 0, 3, 0, 0, 0, 7, 21, 0, 0, 8, 0, 0, 0, 3, -1, 0, 0, 0, 0, 8, 0, 2, -5, 6, -7, -17, 0, -17, 0, -32, -1, 1, 1, 8, 0, -8, 0, 6, 8, -8, -2, 0, 0, -1, 0, 0, 0, 16, -2, 3, 0, -1, 0, -25, 14, -18, -8, 0, 0, -2, 0, 0, 0, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, -8, 0, 5, 0, -2, 6, -7, -23, 0, 0, 8, 0, 0, 0, -1, -4, -16, 0, 6, 0, -18, 19, 0, -5, 0, 0, -1, 0, 0, 0, 1, 12, 0, 0, 9, 0, 0, 0, 25, 2, 8, 0, 0, 0, 7, -5, 0, 0, 1, 0, 0, 0, -33, 20, 0, 0, 0, 0, -4, 0, 0, 0, -14, -4, 0, 0, 7, 0, -7, -14, -9, 11, 0, 0, 5, 0, 0, 0, 29, 8, -14, 0, 16, 0, 12, -29, -8, -37, -15, 0, -7, 0, -2, -4, 3, 6, -19, 0, 0, 0, 6, 6, 0, 0, -8, 0, -12, 0, -21, 10, 5, -13, 0, 0, 2, 0, 0, 0, -20, 9, -4, 0, 18, 0, -6, -28, -11, -3, 0, 0, 7, 0, 0, 0, -4, -10, 3, 0, -12, 0, 5, -12, 7, -3, -2, 0, -23, 0, -1, 0, -18, -1, 3, 0, 0, 0, 0, 10, 0, 0, 0, 0, -10, 0, 0, 0, 0, -4, 0, 0, -8, 0, 0, 0, 0, 0, -5, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, -4, 0, 0, -11, 0, 0, 0, 0, 15, 0, 0, 19, 0, 7, 0, 0, 6, 0, -8, 25, 0, -49, 0, 0, 16, 0, -2, 5, 0, 13, 0, 0, -5, 0, 10, 0, 0, -24, 0, 0, 0, 0, 0, 13, 0, 14, 0, 0, 18, 0, -4, 0, 0, -2, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, -9, 0, 20, 0, 0, 8, 0, 15, 0, 0, 1, 0, 0, 0, 0, -1, -12, 0, -10, 0, 0, 11, 0, 16, 0, 0, 7, 0, 0, 0, 0, -3, 0, 0, 27, 0, 0, 0, 0, 6, 1, 0, 0, 0, 0, 4, 0, 0, -1, 0, 0, 0, 0, 8, 0, 0, 0, 0, -5, 0, 0, 0, 0, 0, 0, 0, -18, 0, 0, 0, 0, -11, 0, 0, -39, 0, 0, 0, 0, 1, 0, 0, -8, 0, 0, 2, 0, 6, -4, 0, -35, 0, 0, 1, 0, 8, -19, 0, 0, 0, 0, -15, 0, 0, -22, 0, 5, 0, 0, -3, 0, 16, 0, 0, -1, 0, 0, 0, 0, 7, 8, 0, -8, 0, 0, 2, 0, 16, 0, 0, 3, 0, 0, 0, 0, -13, -1, 0, -19, 0, 0, 17, 0, 6, -3, 0, 6, 0, 0, -1, 0, -7, 2, 4, 0, 0, -2, 0, 0, 0, 0, 0, -26, 8, 0, 0, -4, 0, 0, 0, -19, 0, 0, 0, -6, 0, -12, 1, 0, 0, -12, 0, 0, 0, -11, 0, 0, 0, 23, 0, 0, 0, 17, 4, 0, 0, -16, 0, 0, 0, -5, -11, -1, -10, -14, 0, -7, 0, 16, 17, 9, 0, -24, 0, -14, 0, -9, -18, -13, -13, 2, 0, -13, 0, 0, 0, -36, -7, 0, 0, -1, 0, 37, 1, 28, -3, -41, 0, -1, 0, 0, 0, -2, 0, 0, 0, -3, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 11, -4, -11, 0, 0, 0, 0, 0, 9, 21, 0, 0, 8, 0, 8, 29, -11, -3, 3, 0, 11, 0, 0, 0, 26, -11, 0, 0, 3, 0, 0, 0, -12, -2, 0, 0, -3, 0, -13, -17, 0, 0, -6, 0, 0, 0, -1, 3, 0, 0, 3, 0, 0, 0, 0, 0, 1, 2, 0, 0, -9, 0, 6, 3, 0, 1, -2, 0, 33, 0, 0, 0, 8, 11, 0, 0, 5, 0, -26, 10, -2, 13, 9, 0, 18, 0, 3, 10, 8, 10, -5, 0, 0, 0, 5, -3, 0, 0, 8, 0, 0, 0, -3, 1, -12, 4, -13, 0, 8, 0, 0, 0, 8, 2, 0, 0, 4, 0, 18, -2, -14, 5, 10, 0, 14, 0, 0, 0, 8, 10, 0, 0, 15, 0, 0, 2, 2, 6, 14, 0, -3, 0, 4, -1, 11, 10, -8, 0, -9, 0, 6, 0, 0, 0, 0, -9, 0, 0, 0, 0, -7, 0, 0, 0, 0, -13, 0, 0, 9, 0, 0, 0, 0, -7, 4, 0, 0, 0, 0, -10, 0, 0, 12, 0, 0, 0, 0, -18, 0, 0, 11, 0, 0, 0, 0, 4, 0, 0, -23, 0, -6, 0, 0, -8, 0, 4, 8, 0, 15, 0, 0, -7, 0, 24, -3, 0, -15, 0, 0, 10, 0, -18, 0, 0, 5, 0, 0, 0, 0, -18, -16, 0, 4, 0, 0, 11, 0, -2, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0, 0, -10, 0, 4, 0, 0, 6, 0, 0, 0, 0, 0, -14, 0, 12, 0, 0, 7, 0, -2, 0, 0, -6, 0, 0, 0, 0, -31, 0, 0, 7, 0, 0, 0, 0, -19, -6, 0, 0, 0, 0, 6, 0, 0, 1, 0, 0, 0, 0, 12, 0, 0, 0, 0, -6, 0, 0, 0, 0, -3, -10, 0, 9, 0, 0, -1, 0, -16, 0, 0, 15, 0, 0, 0, 0, 4, 5, 0, -7, 0, 0, -10, 0, -3, -6, 0, 19, 0, 0, 2, 0, -7, 3, 0, 0, 0, 0, 0, 0, 0, -15, 0, 28, 0, 0, 5, 0, 7, 0, 0, 0, 0, 0, 0, 0, -3, 13, 0, 4, 0, 0, -2, 0, -7, 0, 0, 6, 0, 0, 0, 0, -6, -23, 0, 13, 0, 0, 4, 0, 1, -5, 0, 2, 0, 0, -2, 0, -3, -9, 38, 0, 0, 7, -17, 0, 0, 0, 0, 4, 5, 0, 0, -23, 25, 0, 0, -18, -28, 0, 0, 1, 2, 14, 2, 0, 0, 19, 8, 0, 0, 0, -13, 0, 0, 8, -6, 0, 0, 1, 28, 0, 0, 5, -12, 0, 0, -10, 30, 4, -8, -5, -2, -3, -5, -25, 19, -11, 2, -4, 14, 10, -10, 11, -24, -9, 23, 2, -13, -13, -17, 0, 0, -32, -28, 0, 0, -18, 17, -17, 28, 4, -1, 2, -1, 9, 8, 0, 0, 0, 0, 0, 0, -5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -3, 3, -18, -3, -6, -21, -40, 4, 0, 0, 8, 21, 0, 0, -3, 22, -13, 16, -7, 5, 21, 20, 9, 3, 0, 0, -14, -21, 0, 0, -31, 13, 0, 0, 2, 0, 0, 0, 4, 5, 6, -20, 0, 0, 0, -17, 0, 0, -6, -5, 0, 0, 9, 9, 0, 0, 0, 0, -5, 0, 0, 0, 5, -12, -14, 0, 6, -1, -16, 1, 10, 8, 0, 0, -10, 3, 0, 0, -31, 8, -17, 8, -6, 4, 0, 4, 11, 13, -11, 3, -12, -4, 8, -8, -28, -9, 4, -11, 0, 0, -1, 1, 0, 0, -10, 5, -35, 12, -5, -18, -16, -23, 0, 0, -2, -5, 0, 0, -1, -10, -2, 2, 1, 5, 17, 8, 7, 9, 0, 0, -1, 3, 0, 0, 10, 15, -2, -1, -7, 4, -8, 4, -1, -25, 3, -6, -15, 10, 6, -4, 3, 13}

#define ENV_TENSOR_DENSE_1_KERNEL_0_DEC_BITS {5}

#define ENV_TENSOR_DENSE_1_BIAS_0 {-1, -15, 0, -4, 27, 7, 11, -28, 12, -1, -1, 15, 13, -2, 17, 15, 9, -1, -1, -14, 8, -50, 19, -1, -10, 0, 0, 33, 30, -80, 17, -26}

#define ENV_TENSOR_DENSE_1_BIAS_0_DEC_BITS {6}

#define ENV_DENSE_1_BIAS_LSHIFT {4}

#define ENV_DENSE_1_OUTPUT_RSHIFT {6}

#define ENV_TENSOR_DENSE_2_KERNEL_0 {0, 0, 0, 0, 0, 0, 0, 0, 8, 0, -18, 0, 0, -34, 0, 59, 3, 0, -8, 0, 0, 33, 0, 77, -9, 0, -36, 0, 0, 53, 0, 9, -61, 0, 0, 0, 0, -40, 0, 0, 0, 0, -22, 0, 0, 0, 0, 70, 20, 0, -14, 0, 0, 8, 0, -12, 0, 0, -29, 0, 0, 0, 0, -24, -67, 0, 0, 0, 0, 9, 0, 0, 0, 0, 42, 0, 0, 0, 0, -14, 35, 0, -125, 0, 0, 28, 0, 10, 0, 0, 0, 0, 0, 11, 0, 0, -34, 0, 0, 0, 0, 13, 0, 0, 0, 0, -2, 0, 0, 0, 0, -36, 35, 0, 11, 0, 0, -8, 0, 2, -52, 0, 0, 0, 0, -113, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 0, -74, 2, -10, -16, 25, 0, -15, 0, -26, -13, 5, -12, 33, 0, 22, 0, -48, 7, 9, 0, -32, 0, 90, 0, 0, 57, -19, 0, 0, 0, 0, 0, 0, 0, 0, 13, 32, 0, 9, 0, -55, 13, 27, -61, -19, 0, 0, 0, -13, 0, 0, 46, 26, 0, 4, 0, 0, -29, -13, 0, 0, 0, 0, 0, 36, 0, 0, -11, -12, 0, -2, 0, -11, 56, 24, 2, 10, 0, -36, 0, 0, -2, -26, 0, 0, 0, 60, 0, 0, 10, -5, 0, 0, 0, 0, 0, 42, 0, 0, 19, 0, 0, 83, 0, 34, 30, 15, 9, -29, 0, -20, 0, 4, -9, -19, -106, -5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 54, 10, 11, -5, 0, 0, 0, 0, 91, -13, -17, 76, 0, 0, 0, 0, 6, -21, -56, -31, 0, 0, 0, 0, 43, -6, 0, 0, 0, 0, 0, 0, 0, 0, -7, 44, 0, 0, 0, 0, -17, -18, 6, 2, 0, 0, 0, 0, 0, 0, -18, 76, 0, 0, 0, 0, 57, -38, 0, 0, 0, 0, 0, 0, 0, 0, -13, 10, 0, 0, 0, 0, -21, 63, 11, 3, 0, 0, 0, 0, -71, -67, 0, 0, 0, 0, 0, 0, -25, -5, 0, 0, 0, 0, 0, 0, 0, 0, 3, 19, 0, 0, 0, 0, -32, -6, 56, -27, 0, 0, 0, 0, 1, -6, -63, -17, 0, 0, 0, 0, 0, 0, 0, 0, -32, 37, -5, 36, 0, -46, 0, 49, 38, -19, 24, 0, 0, 17, 0, 27, 13, 22, -20, 49, 0, 23, 0, -13, 9, -16, 0, 0, 0, -40, 0, 0, 0, 0, 18, 8, 0, 0, 0, 23, 28, 38, -18, 12, 0, 52, 0, 27, 0, 0, -15, 5, 0, 0, 0, 24, 45, -21, 0, 0, 0, 2, 0, 0, 0, 0, 5, 25, 0, 0, 0, -22, -7, 6, -6, 33, 0, 5, 0, 28, 0, 37, 0, 0, 0, -34, 0, 0, -21, 52, 0, 0, 0, 14, 0, 0, 0, 0, -35, 9, 0, 0, 0, -3, -16, 22, 35, -6, 0, 11, 0, -10, -89, 67, -4, 46, 0, -51, 0, -2}

#define ENV_TENSOR_DENSE_2_KERNEL_0_DEC_BITS {6}

#define ENV_TENSOR_DENSE_2_BIAS_0 {48, -2, -3, -6, -1, 73, 22, 45, 0, -2, 10, 50, 39, -23, -2, 44}

#define ENV_TENSOR_DENSE_2_BIAS_0_DEC_BITS {7}

#define ENV_DENSE_2_BIAS_LSHIFT {3}

#define ENV_DENSE_2_OUTPUT_RSHIFT {7}

#define ENV_TENSOR_DENSE_3_KERNEL_0 {0, -124, 0, 0, 27, 15, 0, 0, 0, 0, 0, -22, 0, 0, 53, 87, 0, 0, 0, -11, 0, 0, -25, 57, 0, -11, 0, 26, 32, -72, 29, 37, 0, 0, 0, -94, 0, 0, -35, 24, 0, 0, 0, 52, 0, 0, -42, 63, 0, -13, 0, 0, 25, 47, 0, 0, 0, 23, 0, 39, 38, -10, 40, 44, 0, 81, 0, 0, 0, 5, 0, 0, 0, 0, 0, 35, 0, 0, 0, 18, 0, 0, 0, -73, 0, 0, 0, 1, 0, -79, 0, 22, 0, -38, 0, -10, 0, 0, 0, 13, 0, 0, 0, 4, 0, 0, 0, -30, 0, 0, 0, 0, 0, 19, 0, 0, 0, -63, 0, 0, 0, 48, 0, 49, 0, 64, 0, -21}

#define ENV_TENSOR_DENSE_3_KERNEL_0_DEC_BITS {6}

#define ENV_TENSOR_DENSE_3_BIAS_0 {-4, 71, 33, 36, 0, 0, -4, -87}

#define ENV_TENSOR_DENSE_3_BIAS_0_DEC_BITS {7}

#define ENV_DENSE_3_BIAS_LSHIFT {2}

#define ENV_DENSE_3_OUTPUT_RSHIFT {6}

#define ENV_TENSOR_DENSE_4_KERNEL_0 {0, 0, 0, 54, 0, 0, 34, 41, 0, -25, 0, 53, -10, 104, -36, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, -89, 0, -11, 91, -61, 64, -66, 0, 65, -14, 81, 0, -21, 0, -55, 0, -54, -31, 74, 0, 53, 0, -23}

#define ENV_TENSOR_DENSE_4_KERNEL_0_DEC_BITS {6}

#define ENV_TENSOR_DENSE_4_BIAS_0 {-1, -49, 86, -14, 0, 17}

#define ENV_TENSOR_DENSE_4_BIAS_0_DEC_BITS {5}

#define ENV_DENSE_4_BIAS_LSHIFT {4}

#define ENV_DENSE_4_OUTPUT_RSHIFT {7}

#define ENV_TENSOR_DENSE_5_KERNEL_0 {-20, -14, 51, 18, -2, 22, -19, -70, 76, -45, -5, -85, -108, 96, 39, 42, -95, -3, 38, 25, -34, 61, -12, -9}

#define ENV_TENSOR_DENSE_5_KERNEL_0_DEC_BITS {5}

#define ENV_TENSOR_DENSE_5_BIAS_0 {44, 37, 7, -87}

#define ENV_TENSOR_DENSE_5_BIAS_0_DEC_BITS {5}

#define ENV_DENSE_5_BIAS_LSHIFT {2}

#define ENV_DENSE_5_OUTPUT_RSHIFT {7}


/* output q format for each layer */
#define ENV_INPUT_1_OUTPUT_DEC 7
#define ENV_INPUT_1_OUTPUT_OFFSET 0
#define ENV_DENSE_OUTPUT_DEC 5
#define ENV_DENSE_OUTPUT_OFFSET 0
#define ENV_RE_LU_OUTPUT_DEC 5
#define ENV_RE_LU_OUTPUT_OFFSET 0
#define ENV_DENSE_1_OUTPUT_DEC 4
#define ENV_DENSE_1_OUTPUT_OFFSET 0
#define ENV_RE_LU_1_OUTPUT_DEC 4
#define ENV_RE_LU_1_OUTPUT_OFFSET 0
#define ENV_DENSE_2_OUTPUT_DEC 3
#define ENV_DENSE_2_OUTPUT_OFFSET 0
#define ENV_RE_LU_2_OUTPUT_DEC 3
#define ENV_RE_LU_2_OUTPUT_OFFSET 0
#define ENV_DENSE_3_OUTPUT_DEC 3
#define ENV_DENSE_3_OUTPUT_OFFSET 0
#define ENV_RE_LU_3_OUTPUT_DEC 3
#define ENV_RE_LU_3_OUTPUT_OFFSET 0
#define ENV_DENSE_4_OUTPUT_DEC 2
#define ENV_DENSE_4_OUTPUT_OFFSET 0
#define ENV_RE_LU_4_OUTPUT_DEC 2
#define ENV_RE_LU_4_OUTPUT_OFFSET 0
#define ENV_DENSE_5_OUTPUT_DEC 0
#define ENV_DENSE_5_OUTPUT_OFFSET 0
#define ENV_ACTIVATION_OUTPUT_DEC 7
#define ENV_ACTIVATION_OUTPUT_OFFSET 0

/* bias shift and output shift for none-weighted layer */

/* tensors and configurations for each layer */
static int8_t env_nnom_input_data[5] = {0};

const nnom_shape_data_t env_tensor_input_1_dim[] = {5};
const nnom_qformat_param_t env_tensor_input_1_dec[] = {7};
const nnom_qformat_param_t env_tensor_input_1_offset[] = {0};
const nnom_tensor_t env_tensor_input_1 = {
    .p_data = (void*)env_nnom_input_data,
    .dim = (nnom_shape_data_t*)env_tensor_input_1_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_input_1_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_input_1_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_io_config_t env_input_1_config = {
    .super = {.name = "input_1"},
    .tensor = (nnom_tensor_t*)&env_tensor_input_1
};
const int8_t env_tensor_dense_kernel_0_data[] = ENV_TENSOR_DENSE_KERNEL_0;

const nnom_shape_data_t env_tensor_dense_kernel_0_dim[] = {5, 64};
const nnom_qformat_param_t env_tensor_dense_kernel_0_dec[] = ENV_TENSOR_DENSE_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_kernel_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_kernel_0 = {
    .p_data = (void*)env_tensor_dense_kernel_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t env_tensor_dense_bias_0_data[] = ENV_TENSOR_DENSE_BIAS_0;

const nnom_shape_data_t env_tensor_dense_bias_0_dim[] = {64};
const nnom_qformat_param_t env_tensor_dense_bias_0_dec[] = ENV_TENSOR_DENSE_BIAS_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_bias_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_bias_0 = {
    .p_data = (void*)env_tensor_dense_bias_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t env_dense_output_shift[] = ENV_DENSE_OUTPUT_RSHIFT;
const nnom_qformat_param_t env_dense_bias_shift[] = ENV_DENSE_BIAS_LSHIFT;
const nnom_dense_config_t env_dense_config = {
    .super = {.name = "dense"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&env_tensor_dense_kernel_0,
    .bias = (nnom_tensor_t*)&env_tensor_dense_bias_0,
    .output_shift = (nnom_qformat_param_t *)&env_dense_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&env_dense_bias_shift
};
const int8_t env_tensor_dense_1_kernel_0_data[] = ENV_TENSOR_DENSE_1_KERNEL_0;

const nnom_shape_data_t env_tensor_dense_1_kernel_0_dim[] = {64, 32};
const nnom_qformat_param_t env_tensor_dense_1_kernel_0_dec[] = ENV_TENSOR_DENSE_1_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_1_kernel_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_1_kernel_0 = {
    .p_data = (void*)env_tensor_dense_1_kernel_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_1_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_1_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_1_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t env_tensor_dense_1_bias_0_data[] = ENV_TENSOR_DENSE_1_BIAS_0;

const nnom_shape_data_t env_tensor_dense_1_bias_0_dim[] = {32};
const nnom_qformat_param_t env_tensor_dense_1_bias_0_dec[] = ENV_TENSOR_DENSE_1_BIAS_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_1_bias_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_1_bias_0 = {
    .p_data = (void*)env_tensor_dense_1_bias_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_1_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_1_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_1_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t env_dense_1_output_shift[] = ENV_DENSE_1_OUTPUT_RSHIFT;
const nnom_qformat_param_t env_dense_1_bias_shift[] = ENV_DENSE_1_BIAS_LSHIFT;
const nnom_dense_config_t env_dense_1_config = {
    .super = {.name = "dense_1"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&env_tensor_dense_1_kernel_0,
    .bias = (nnom_tensor_t*)&env_tensor_dense_1_bias_0,
    .output_shift = (nnom_qformat_param_t *)&env_dense_1_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&env_dense_1_bias_shift
};
const int8_t env_tensor_dense_2_kernel_0_data[] = ENV_TENSOR_DENSE_2_KERNEL_0;

const nnom_shape_data_t env_tensor_dense_2_kernel_0_dim[] = {32, 16};
const nnom_qformat_param_t env_tensor_dense_2_kernel_0_dec[] = ENV_TENSOR_DENSE_2_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_2_kernel_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_2_kernel_0 = {
    .p_data = (void*)env_tensor_dense_2_kernel_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_2_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_2_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_2_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t env_tensor_dense_2_bias_0_data[] = ENV_TENSOR_DENSE_2_BIAS_0;

const nnom_shape_data_t env_tensor_dense_2_bias_0_dim[] = {16};
const nnom_qformat_param_t env_tensor_dense_2_bias_0_dec[] = ENV_TENSOR_DENSE_2_BIAS_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_2_bias_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_2_bias_0 = {
    .p_data = (void*)env_tensor_dense_2_bias_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_2_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_2_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_2_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t env_dense_2_output_shift[] = ENV_DENSE_2_OUTPUT_RSHIFT;
const nnom_qformat_param_t env_dense_2_bias_shift[] = ENV_DENSE_2_BIAS_LSHIFT;
const nnom_dense_config_t env_dense_2_config = {
    .super = {.name = "dense_2"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&env_tensor_dense_2_kernel_0,
    .bias = (nnom_tensor_t*)&env_tensor_dense_2_bias_0,
    .output_shift = (nnom_qformat_param_t *)&env_dense_2_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&env_dense_2_bias_shift
};
const int8_t env_tensor_dense_3_kernel_0_data[] = ENV_TENSOR_DENSE_3_KERNEL_0;

const nnom_shape_data_t env_tensor_dense_3_kernel_0_dim[] = {16, 8};
const nnom_qformat_param_t env_tensor_dense_3_kernel_0_dec[] = ENV_TENSOR_DENSE_3_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_3_kernel_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_3_kernel_0 = {
    .p_data = (void*)env_tensor_dense_3_kernel_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_3_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_3_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_3_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t env_tensor_dense_3_bias_0_data[] = ENV_TENSOR_DENSE_3_BIAS_0;

const nnom_shape_data_t env_tensor_dense_3_bias_0_dim[] = {8};
const nnom_qformat_param_t env_tensor_dense_3_bias_0_dec[] = ENV_TENSOR_DENSE_3_BIAS_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_3_bias_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_3_bias_0 = {
    .p_data = (void*)env_tensor_dense_3_bias_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_3_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_3_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_3_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t env_dense_3_output_shift[] = ENV_DENSE_3_OUTPUT_RSHIFT;
const nnom_qformat_param_t env_dense_3_bias_shift[] = ENV_DENSE_3_BIAS_LSHIFT;
const nnom_dense_config_t env_dense_3_config = {
    .super = {.name = "dense_3"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&env_tensor_dense_3_kernel_0,
    .bias = (nnom_tensor_t*)&env_tensor_dense_3_bias_0,
    .output_shift = (nnom_qformat_param_t *)&env_dense_3_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&env_dense_3_bias_shift
};
const int8_t env_tensor_dense_4_kernel_0_data[] = ENV_TENSOR_DENSE_4_KERNEL_0;

const nnom_shape_data_t env_tensor_dense_4_kernel_0_dim[] = {8, 6};
const nnom_qformat_param_t env_tensor_dense_4_kernel_0_dec[] = ENV_TENSOR_DENSE_4_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_4_kernel_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_4_kernel_0 = {
    .p_data = (void*)env_tensor_dense_4_kernel_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_4_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_4_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_4_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t env_tensor_dense_4_bias_0_data[] = ENV_TENSOR_DENSE_4_BIAS_0;

const nnom_shape_data_t env_tensor_dense_4_bias_0_dim[] = {6};
const nnom_qformat_param_t env_tensor_dense_4_bias_0_dec[] = ENV_TENSOR_DENSE_4_BIAS_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_4_bias_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_4_bias_0 = {
    .p_data = (void*)env_tensor_dense_4_bias_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_4_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_4_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_4_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t env_dense_4_output_shift[] = ENV_DENSE_4_OUTPUT_RSHIFT;
const nnom_qformat_param_t env_dense_4_bias_shift[] = ENV_DENSE_4_BIAS_LSHIFT;
const nnom_dense_config_t env_dense_4_config = {
    .super = {.name = "dense_4"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&env_tensor_dense_4_kernel_0,
    .bias = (nnom_tensor_t*)&env_tensor_dense_4_bias_0,
    .output_shift = (nnom_qformat_param_t *)&env_dense_4_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&env_dense_4_bias_shift
};
const int8_t env_tensor_dense_5_kernel_0_data[] = ENV_TENSOR_DENSE_5_KERNEL_0;

const nnom_shape_data_t env_tensor_dense_5_kernel_0_dim[] = {6, 4};
const nnom_qformat_param_t env_tensor_dense_5_kernel_0_dec[] = ENV_TENSOR_DENSE_5_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_5_kernel_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_5_kernel_0 = {
    .p_data = (void*)env_tensor_dense_5_kernel_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_5_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_5_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_5_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t env_tensor_dense_5_bias_0_data[] = ENV_TENSOR_DENSE_5_BIAS_0;

const nnom_shape_data_t env_tensor_dense_5_bias_0_dim[] = {4};
const nnom_qformat_param_t env_tensor_dense_5_bias_0_dec[] = ENV_TENSOR_DENSE_5_BIAS_0_DEC_BITS;
const nnom_qformat_param_t env_tensor_dense_5_bias_0_offset[] = {0};
const nnom_tensor_t env_tensor_dense_5_bias_0 = {
    .p_data = (void*)env_tensor_dense_5_bias_0_data,
    .dim = (nnom_shape_data_t*)env_tensor_dense_5_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_dense_5_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_dense_5_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t env_dense_5_output_shift[] = ENV_DENSE_5_OUTPUT_RSHIFT;
const nnom_qformat_param_t env_dense_5_bias_shift[] = ENV_DENSE_5_BIAS_LSHIFT;
const nnom_dense_config_t env_dense_5_config = {
    .super = {.name = "dense_5"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&env_tensor_dense_5_kernel_0,
    .bias = (nnom_tensor_t*)&env_tensor_dense_5_bias_0,
    .output_shift = (nnom_qformat_param_t *)&env_dense_5_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&env_dense_5_bias_shift
};
static int8_t env_nnom_output_data[4] = {0};

const nnom_shape_data_t env_tensor_output0_dim[] = {4};
const nnom_qformat_param_t env_tensor_output0_dec[] = {ENV_ACTIVATION_OUTPUT_DEC};
const nnom_qformat_param_t env_tensor_output0_offset[] = {0};
const nnom_tensor_t env_tensor_output0 = {
    .p_data = (void*)env_nnom_output_data,
    .dim = (nnom_shape_data_t*)env_tensor_output0_dim,
    .q_dec = (nnom_qformat_param_t*)env_tensor_output0_dec,
    .q_offset = (nnom_qformat_param_t*)env_tensor_output0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_io_config_t env_output0_config = {
    .super = {.name = "output0"},
    .tensor = (nnom_tensor_t*)&env_tensor_output0
};
/* model version */
#define NNOM_MODEL_VERSION (10000*0 + 100*4 + 3)

/* nnom model */
static nnom_model_t* env_nnom_model_create(void)
{
	static nnom_model_t model;
	nnom_layer_t* layer[14];

	check_model_version(NNOM_MODEL_VERSION);
	new_model(&model);

	layer[0] = input_s(&env_input_1_config);
	layer[1] = model.hook(dense_s(&env_dense_config), layer[0]);
	layer[2] = model.active(act_relu(), layer[1]);
	layer[3] = model.hook(dense_s(&env_dense_1_config), layer[2]);
	layer[4] = model.active(act_relu(), layer[3]);
	layer[5] = model.hook(dense_s(&env_dense_2_config), layer[4]);
	layer[6] = model.active(act_relu(), layer[5]);
	layer[7] = model.hook(dense_s(&env_dense_3_config), layer[6]);
	layer[8] = model.active(act_relu(), layer[7]);
	layer[9] = model.hook(dense_s(&env_dense_4_config), layer[8]);
	layer[10] = model.active(act_relu(), layer[9]);
	layer[11] = model.hook(dense_s(&env_dense_5_config), layer[10]);
	layer[12] = model.hook(Softmax(), layer[11]);
	layer[13] = model.hook(output_s(&env_output0_config), layer[12]);
	model_compile(&model, layer[0], layer[13]);
	return &model;
}
