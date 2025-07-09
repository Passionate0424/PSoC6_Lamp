#include "nnom.h"

/* Weights, bias and Q format */
#define TENSOR_DENSE_KERNEL_0 {-15, 4, -10, 2, 1, 13, 2, -14, 9, -5, -9, -5, -1, 5, 1, -11, 12, -5, 1, 1, 11, 16, -29, -16, 6, -12, 9, -65, -2, -10, 1, -7, 5, 0, 0, 8, -5, -2, -10, -4, -4, 5, -3, -18, -4, 3, -7, -18, -3, -18, -1, 6, -9, 3, 15, -16, -3, 6, -3, -3, 1, 12, -4, 6, -6, -2, 3, -1, 1, 4, -2, -1, -4, -1, 7, -4, -4, -5, -15, 1, 0, 3, -38, 12, -23, -19, -40, 7, 2, -1, -3, 13, 4, -15, 3, -11, 12, -9, -7, 15, 0, -10, 43, 1, -2, -3, 21, -4, -3, -6, 2, 8, -12, 0, 2, -1, -3, -8, 10, 0, -1, 6, -2, -2, 1, 6, -23, -16, -4, -2, -4, -19, 6, 10, 8, -3, 0, 16, -5, -5, -13, 6, 15, 23, 6, -3, -10, -25, -12, -37, 1, 5, -1, 16, 9, 7, 8, -10, 2, -4, 0, 7, -2, 25, -10, 9, -27, -4, 0, -3, -3, 10, 3, 7, 5, 1, -1, -1, 12, -16, 8, 1, 25, -11, -5, -1, -23, 31, 5, -4, -2, 12, 2, -1, -8, -3, -7, 5, 8, 6, -3, 8, 8, -48, -1, -1, -2, 2, 6, -13, 10, 6, -2, -3, -2, -4, -23, -9, -1, -4, -11, -13, -59, -12, 4, -1, -27, 4, 10, 4, -1, 7, 0, 0, 10, -3, 3, 13, 4, -5, -3, 4, -1, -23, -7, -2, 33, -29, 5, 15, -10, -5, 7, 4, 0, 6, 9, 2, 9, 2, -1, 2, 35, -20, 5, -1, -51, 1, -15, 1, 3, -2, 10, -2, 0, 1, 6, 0, 0, 1, -11, -1, 49, 24, -2, 1, 3, 43, 9, -2, 1, -12, -2, 1, 0, 8, -15, 12, 1, 0, 3, -22, -24, 15, -2, 6, 3, 13, 2, 14, 2, 4, -4, 7, 1, -11, 0, -10, -1, 12}

#define TENSOR_DENSE_KERNEL_0_DEC_BITS {4}

#define TENSOR_DENSE_BIAS_0 {19, -5, -23, -20, -18, 13, 10, 20, 0, -35, -15, 68, 0, -44, 55, -1, -25, -40, 51, 37, -11, 42, -24, 0, 0, -34, -26, -5, 24, 30, -77, -26, 0, -75, -33, 6, -17, -65, 46, -7, 0, 23, 0, 0, 20, -31, -71, -3, 17, -42, -28, -20, -25, -7, -18, -13, 32, 1, -3, -60, -13, 8, -8, -34}

#define TENSOR_DENSE_BIAS_0_DEC_BITS {7}

#define DENSE_BIAS_LSHIFT {5}

#define DENSE_OUTPUT_RSHIFT {5}

#define TENSOR_DENSE_1_KERNEL_0 {11, -11, -4, 1, 9, -58, 0, 0, 3, -1, -78, 10, 1, 0, 7, 9, -5, -51, 2, 4, -4, 0, 5, 9, -14, -43, 12, 8, -15, 5, 7, 3, -1, 0, -4, 5, 1, 2, 8, 15, -34, 14, -7, -3, -25, -5, -8, -46, 3, -3, 10, 3, 2, 2, -2, 0, -3, -3, 2, 0, 4, -8, 1, 0, -4, 5, -40, -13, 2, -26, -1, -26, 4, 2, -20, 14, -19, 3, 5, -2, -8, -51, -1, 4, -25, -3, -23, -3, 7, 5, 2, 0, 3, -7, 3, 1, 0, -2, -3, 8, 2, 4, 9, 0, 0, 14, 2, 8, -23, 11, 7, 0, 2, 5, -7, 17, 2, 10, -12, -31, 16, 16, -12, 11, 14, -1, 13, -13, -3, 0, -33, 3, 0, 0, -2, -16, 2, -66, 6, -13, -15, -14, 4, 31, -2, -41, -1, 1, 9, 2, 6, -3, -19, 6, -38, -21, -9, -10, 0, 0, 25, -11, 1, 1, -43, 26, -2, -2, 3, 8, 3, 2, 0, 15, -2, 0, 23, 2, -8, 14, -12, 11, -4, -31, -22, 6, 3, -4, -1, -10, 2, 1, -14, -3, -2, -7, 2, -12, 3, -27, 9, 13, -2, 4, 4, -5, 0, -8, -3, 13, -9, 16, -59, -8, 3, -16, -12, 3, -4, 1, 4, 1, -4, 0, 15, -2, -2, -2, -42, -3, -4, -2, -3, 1, -30, -22, -6, -28, -12, -14, -12, 2, 1, 0, -1, 2, -3, 2, 4, -40, -6, -14, 4, -45, 5, -18, -4, -5, 3, 2, -7, 2, -7, 1, -2, -1, 12, -5, 0, -2, -9, -2, 4, -6, -7, -1, -8, -1, 2, 1, 11, 12, -28, 5, 6, 0, 1, 2, 2, 2, 13, -3, 2, -3, -19, -3, 8, 2, 1, -3, -4, 0, -5, -4, -3, -2, -5, -2, -2, 0, -4, -4, 23, -15, 1, 2, 6, -3, 2, 2, -20, 0, -18, 3, -2, -1, -1, -3, 7, 5, -6, 11, 8, -2, 2, 1, -9, -6, -2, 6, 9, 1, 5, -3, 12, -14, -1, -1, -3, 2, 2, -3, 2, 0, -1, 2, -2, 2, -16, 2, -14, -6, -7, -3, 2, -2, -4, 2, 17, -21, -2, -33, -4, 2, -7, 3, -17, -12, 5, 3, 7, -4, -5, -1, -4, 3, -8, -8, -1, 4, 5, 0, -1, -9, 9, 14, 10, 0, -17, 0, 1, -14, 2, -4, 10, -1, 1, -1, 0, -4, -3, 7, -4, -2, 4, -2, -5, 17, -3, -1, 0, -2, -2, 0, 12, 6, 2, -1, -2, 2, -2, -1, -75, 3, -17, -16, 1, 3, 4, 2, 1, -7, 0, -1, -4, -2, -1, -2, 1, 5, -3, 3, -5, -4, 4, -2, 6, -8, -13, -6, -1, -1, 1, 0, 0, -12, -27, -5, 8, 2, -4, 3, -2, 1, -3, 0, -9, 3, -2, -3, 19, -1, -2, 1, 9, -2, -3, 3, 11, 5, -7, -7, 13, 3, 7, 2, -7, -1, 1, -1, -11, -3, -1, -1, 30, 0, -8, -3, -1, 0, 13, -1, 4, 0, 3, 2, 13, 7, 0, -1, 4, -1, -8, -26, 2, 2, -12, 19, 8, -8, 3, -3, 11, 18, 4, -7, 14, -11, 0, 0, 6, 14, -12, 7, 0, 4, -27, -3, -2, 0, -19, -17, -3, -2, 8, -44, 12, 2, 7, -13, -3, 1, 0, -21, 3, -3, -18, -27, 2, 6, 0, 1, -4, 3, 1, 0, -5, -3, -6, -4, 6, 3, 2, -12, -16, 17, -13, 13, -19, -12, -25, 5, -7, 11, -9, 3, -14, 0, 2, 1, -3, -18, 0, 0, 18, -13, 1, 0, 1, 1, 4, -1, 1, -3, -30, 5, 2, 36, 3, -4, -12, -6, -3, -4, -6, -35, -9, 11, -13, 2, -6, 1, -18, -6, 7, 7, -1, -52, -37, 6, -3, -2, -27, 8, -3, 3, -19, 9, -8, 9, -1, -29, 2, -13, -37, -3, -18, -6, 6, -4, -29, -1, 0, 10, -5, 11, -8, 5, -2, 1, 1, -2, -17, -1, 0, -1, 2, -41, -3, -1, 7, 1, 3, 1, 14, 18, 1, -3, 7, 0, -3, 10, 18, 15, -2, 3, -18, 12, -4, 1, -56, 9, 2, -3, 5, -1, 2, 4, -3, 6, -24, -6, -32, -2, -2, 4, 1, 7, -25, 6, -1, -13, -2, 0, 8, -30, 7, 9, -3, 1, 1, 2, 1, 0, -2, -1, 12, 9, -2, -3, 0, -9, -4, 3, -12, -27, -3, 4, -14, 2, -43, -13, -3, 5, -2, -1, -1, 2, -4, -3, -16, 12, -22, 1, -4, 6, 8, 1, -6, -12, 5, 4, 2, 4, -1, 1, 3, 3, 15, 13, 1, 1, 1, -20, -3, -8, -3, -4, 1, -4, 3, 0, 2, -3, -13, -12, -1, -4, 3, -4, -3, 3, -6, 17, -1, 1, -1, 5, -1, 5, 5, -3, -2, 4, -4, 33, -1, -2, 7, -1, -1, -3, 1, -5, -1, 0, 3, 3, -1, -23, 1, -2, 5, -9, -7, -56, 2, 1, -2, 7, 0, -5, 25, -18, 3, 4, 1, -14, -16, -19, -6, 3, -3, 0, 3, -4, 3, 2, 2, -3, -1, -7, 2, -3, -3, -1, 5, 0, 2, -1, -2, -1, -39, -28, 1, 4, 0, -28, -1, -4, -10, 10, 18, 0, -4, 1, -4, -33, 11, 13, -2, 11, 1, 21, -4, 3, 0, 0, -15, 4, 1, 2, 2, 6, 13, -1, -16, -3, 0, 2, 0, 2, -7, 6, 2, 5, -4, 7, -3, 8, 12, 1, -5, 5, -3, 1, -1, -9, -4, 9, 3, 2, 1, 0, 3, 0, 9, 3, 0, -2, -4, 5, -2, 4, 2, -18, 17, 3, -2, 6, -4, -1, -17, 1, -1, -1, -1, 6, -3, 3, -5, 4, -5, 2, 0, 3, -2, -9, 6, 2, 0, -1, -2, -14, 1, 1, -26, 6, 11, -7, 0, 6, -2, -10, 5, -1, 0, 1, 1, 0, 0, -2, 17, -2, 0, -2, -1, 3, -1, -1, -16, 8, 3, -2, 1, -16, -1, -4, 7, -1, -1, 3, -3, 4, -1, 0, 2, -36, -5, 6, 3, 13, -3, -40, 1, 7, -2, -2, 6, 2, 4, 4, -2, -2, 5, 17, 1, -3, 36, 1, -9, -9, 7, 6, 11, 3, -1, 4, 8, -1, 6, -3, -1, 2, -5, 11, -1, -3, -7, 6, 2, 4, 20, -1, 2, -14, -3, 1, -1, -23, 9, -9, 1, 2, 9, 10, -2, 2, -4, -4, -10, -5, -2, -1, 1, 9, -1, -2, 4, -1, 2, -1, -2, -8, 9, -3, -6, -10, -47, 13, -17, 2, -41, 0, 12, -15, 1, -7, -21, 6, -4, -4, 10, 8, 3, 1, 3, -12, -3, 0, 2, 2, -10, 2, -3, 2, -11, 1, 8, -5, -2, 2, -28, 24, 8, -2, -3, 4, -8, -10, 10, -7, -8, -11, -58, 10, -10, 2, -6, -22, 0, 3, -2, -2, 7, 4, 3, 0, -1, -3, -5, -14, 10, 11, -11, 10, -1, 1, -1, -8, 10, 4, -1, 3, -2, -4, -7, -8, 3, -1, 1, 4, 1, 3, 13, 16, -2, 3, -3, -15, -3, 2, -35, 6, 1, 2, -14, -3, -2, -2, 1, -9, -5, -7, 11, -5, 2, -5, 3, 5, 0, 0, -3, 6, -3, 1, 3, 5, 2, 8, 1, -7, 3, 8, 0, 14, -2, 2, 16, -16, -3, -2, 1, -4, -2, 0, 12, -20, 16, -10, 2, 1, 1, -1, -3, -3, -1, -1, -9, -11, 2, 3, 5, 10, -3, -3, 10, 18, -3, -10, -17, 3, -10, 13, 1, -1, -1, 0, 3, -5, 2, -5, -23, 9, 5, 10, 13, 5, 13, -1, 2, 27, 2, 0, 3, -9, -1, 0, -3, -3, 10, 4, 2, -1, 2, 15, 2, 6, -3, 4, 0, 11, 2, 6, 1, 4, -8, -78, -3, 3, 1, -14, 4, 1, -1, 8, 0, -3, -4, -2, 8, 1, -36, 40, 2, 1, 0, -22, 0, -3, -20, 10, -2, -3, 1, -5, 3, 9, 0, -1, 0, -3, 0, 4, 12, -8, 6, -37, 2, -3, 3, -30, -5, -2, -13, -38, -4, 3, 0, -25, 1, -4, 7, -3, 3, 15, -4, -9, -8, 10, 2, 0, 0, -8, -2, -3, 0, 3, -10, -9, 2, -2, 2, 5, 28, -50, -1, 5, -3, 8, 2, 6, 8, 0, -16, 24, 3, 6, -3, -13, -13, 14, -15, 3, -3, 7, 1, 15, -3, -2, 5, -36, 4, 3, -4, 0, -6, 4, -2, 7, -4, 1, 0, 8, -1, 1, 3, 1, -5, 10, -2, -5, 6, 4, 10, 1, 2, -18, -3, 1, -13, -8, 3, -1, 2, -19, 2, -2, -14, 10, -1, 1, -3, 6, -3, -2, 3, 6, -5, -1, 0, 7, -3, -19, 9, -25, -3, 1, 0, -1, 3, 2, 2, -1, 8, -6, 1, -3, -1, 7, -5, -10, 0, -7, 1, 3, -3, 5, 2, 0, 4, -12, -3, 1, -3, 8, 0, 3, 2, -1, -3, 2, -3, 0, 6, -1, -2, -1, -3, 2, -3, 0, -4, -14, 3, 0, -3, 0, 2, 9, 8, 5, 1, 1, -5, 5, 1, 2, 10, 4, 17, 5, 1, -9, -2, 7, 8, 3, 3, -3, -9, -19, 4, 2, 2, 4, 2, 2, 0, -2, -22, 2, -1, -3, -3, -1, -12, -2, -6, 6, -11, -4, 5, -3, 3, 1, -1, -7, -1, -2, -4, -2, 2, 1, 6, -1, -13, 1, 21, -3, 6, -11, -18, 17, -2, 0, 0, 0, -2, -2, 0, 1, 30, -3, -3, 3, -2, -25, -1, -3, -10, 0, 6, 1, 1, -9, -35, -4, 4, 1, 18, -2, 9, 3, -3, -21, -2, -3, -5, -2, 9, -19, -2, 3, -13, 3, 3, -3, 6, 9, 2, 2, -1, 0, 5, 1, -2, 1, 0, 5, -16, 3, -7, -3, -4, -3, -1, 0, -10, 3, -27, 3, -1, -11, -5, -32, 1, -2, 12, -3, -12, -8, -2, 16, -1, 3, -3, 0, 3, -1, 3, -3, -6, -2, -1, 3, -5, -20, -13, -13, -28, -2, 5, -1, 1, 0, 1, -1, -9, -2, -2, -3, -6, -7, 0, -6, -10, 3, -3, 1, 16, -11, 0, -3, -1, 0, -2, 3, -7, -5, -2, 0, 5, 1, -25, 2, -12, -16, -4, -11, 2, -3, -2, -3, 5, 7, 0, 2, 3, 2, 11, 3, -1, -5, -11, 0, -13, -1, 0, -4, -12, -1, 0, 2, -10, -4, -1, 3, -5, -3, 4, -7, -4, -4, -1, 1, 4, -3, -2, -1, -14, -2, 4, -2, -1, -11, 3, 3, -15, 0, 12, 0, 12, -2, 1, -8, -4, 3, -1, 2, 0, -5, 0, 4, 10, -3, -23, -1, 4, 1, 5, 1, 12, -5, 4, -1, 1, 11, 3, -1, -3, -3, -24, -36, -2, -3, 3, 16, 4, 9, 7, 4, 9, 22, 0, 5, 3, -54, 4, 1, 0, 3, 23, -15, -1, -2, 6, 3, -2, 0, 4, 7, 8, -3, -33, -46, 6, -6, -2, -15, 1, 4, -26, -7, 3, -3, -36, 12, -17, -33, 4, 0, 15, 13, -4, -3, -15, -23, -28, -8, 4, 1, -61, -5, 12, 11, 24, -5, 7, -5, -19, -4, -12, 14, -6, -27, 13, -14, 5, -9, -7, -13, 3, -1, -30, 2, 0, -1, -3, 3, -2, 8, -3, 0, -2, -2, 7, -28, -1, 7, 22, -48, 7, 5, -3, -12, -2, -23, 5, 5, -1, -7, -21, -20, 6, 6, 13, 5, 0, -2, 0, -3, 5, -3, 2, 0, 2, -6, 6, 3, -28, 1, 2, -17, -9, 21, -1, 0, -3, -21, 1, -4, -3, 13, 2, -20, -3, 5, -3, -2, 5, -5, -66, 7, 1, 0, -12, 2, 2, 1, 3, -4, 3, 1, 18, -3, 1, 1, 1, -1, -3, -14, 36, 21, 4, 0, 11, -4, -2, -1, 0, -4, -3, -3, 0, 3, 7, 0, -3, 5, 0, -10, 8, 22, 4, 4, 4, 3, 2, 5, -7, -19, -9, 13, 11, -20, 5, 10, -2, 7, 1, -7, 5, 3, 0, -1, -26, -3, -3, 0, 3, -16, -3, 0, -13, 7, 1, 13, 0, -2, 8, -26, 2, -2, -2, -3, 2, 5, 2, -5, -14, 6, -4, 19, -5, -1, 5, -1, -19, -21, 5, -9, -41, -25, -4, 3, 3, -1, 26, 24, -2, 0, -2, 19, 22, 5, -5, 0, -5, 0, 3, 1, 17, -9, 11, 9, -9, 6, -1, 0, 2, 1, 8, -16, 3, -2, -27, 5, -9, -41, 6, -5, -13, -13, -13, 7, 3, -1, -5, 11, -3, 3, -7, 14, 5, -10, -2, -2, 2, -6, -3, 3, -5, 5, -1, -3, -7, 2, 1, 7, 7, -27, -20, -10, -5, 2, 6, 20, -12, -11, -10, 5, 10, -9, 6, -3, 8, 13, 1, 2, -6, 6, 1, -2, 3, -2, 3, 2, 3, 2, -13, 4, -33, -1, 1, 6, -1, -90, 4, 3, -8, 17, 7, -9, 9, -5, 6, -5, -33, -16, -5, -1, 4, 5, -9, -7, -3, -1, 2, 5, 0, 2, -7, -9, 4, -32, 8, 4, 4, -2, -12, -2, -1, -16, -5, 10, 8, -4, -20, 13, 8, -50, -1, -16, 10, -3, 5, -4, -10, -3, 1, 3, -9, 10, -2, 3, -11, -8, 3, 2, -2, -4, 2, -3, -1, 2, 8, -83, 1, 2, 10, -2, 6, -3, -1, 0, 0, -7, 3, -2, -9, 2, -11, 0, -1, 9, 4, -7, -12, -20, 7, 10, -14, 6, -5, 3, -35, 4, 0, 15, 8, -11, -11, 5, 4, 3, 4, -2, -12, 5, 0, 3, -2, -7, 0, 2, 8, -4, 2, -3, -12, -29, -8, -29, 20, 1, 8, -7, 7, 5, 0, 0, -5, 5, 0, 2, 0, 9, 7, -18, -5, 14, 14, 12, -25, -26, -2, 2, 12, 18, 1, -2, -2, -2, 1, 9, -2, 0, 13, -2, 12, -8, -2, 1, 7, -4, 2, -4, 14, -1, 1, -8, -14, 8, -9, 0, -4, 3, 12, 10, -2, 1, 7, 11, -1, 2, -3, 4, -14, 22, -19, 7, -3, -1, 2, -3, -2, 0, -15, 2, -6, 5, 2, -1, 30, -23, -2, 1, -1, -4, -6, -4, -5, 17, 3, 25, 8, 12, -3, -17, 19, -4, -11, -25, -25, -20, 21, -9, 20, -30, 1, -1, 3, 0, -1, -3, -5, -3, 3, 3, -1, 3, 1, 8, -2, 1, -3, -2, -2, -20, -5, -2, 12, -4, 5, -6, 7, -5, -12, 2, 3, 0, -10, -59, -70, 1, 0, 19, -18, 14, 15, 19, -1, -3, 5, -5, 3, 3, 1, -29, -100, -2, 9, -18, 6, -45, 5, 6, 12, -1, -10, 0, 2, -9, 3, 2, 11, 3, -34, -6, -3, -4, -3, 16, 11, -6, -3, -2, -13, -2, -2, 1, 5, -5, 0, -3, -11, 18, -4, -1, -4, -8, 11, 0, 14, 30, -6, -26, 7, 2, 0, -3, 7, -32, 0, 2, -15, 3, 13, -1, 1, -2, 0, -48, 3, 15, 2, 4, 10, -8, -1, -14, -10, -5, -11, -5, 13, 3, 9, 1, -8, -1, 0, 1, 2, 3, 1, -2, -12, 9, 2, 2, -3, -29, 0, -3, -12, -9, 9, 4, -37, 18, 14, -46, 0, 0, 1, -1, 5, 1, 2, -1, -2, 19, 5, -5, -4, -38, -10, -47, 9, 0, 2, -6, 12, -14, 1, 1, -3, -2, -12, 16, 1, 3, 19, 5, -9, -11, 2, 1, -6, 3, 6, 8, 13, -5, -5, -12, -4, 4, -14, 15, 4, 1, -8, -20, 1, 2, -6, 2, 3, -9, -1, -17, -34, -6, 19, -10, 2, 2, -20, -4, -1, -2, 11, -7, -11, 9, -2, -3, -5, 9, -1, 1, -2, 8, 1, -47, -9, 3, -24, -2, -4, 4, -28, -18, 10, 3, -17, 21, -10, -11, 2, -2, 13, 14, -13, -2, 8, -3, 0, 3, 15, -6, 3, -2, 0, 3, -8, -4, -2, -3, 2, 5, -10, -18, 4, 0, 15, 15, 0, 3, 4, 5, -7, 2, 15, -7, 13, -8, -14, 19, 4, -3, 12, -8, -6, 1, 1, -1, -5, -4, -3, 2, -8, 3, -8, 4, -12, -15, 18, 7, -4, -16, -1, -6, 4, 12, 8, 0, -6, -5, -6, -3, 4, -1, -4, -4, 1, 0, -5, -3, -1, -1, 4, -10, 4, -3, -16, -5, -1, -1, -14, 15, -1, -1, -8, -6, -10, -8, -23, -12, 8, -5, 1, -6, -3, 3, -11, -3, 1, -3, -1, 7, -5, -4, 2, -4, 12, -5, 9, 1, -14, -4, 2, 8, -4, 1, 9, 1, -11, 5, 3, -7, -9, 12, 2, 0, -1, 1, -3, 1, 0, 3, -18, 5, 1, -2, 8, -7, -2, 1, 1, -12, -14, -1, 3, -8, 22, 0, 1, -1, 0, 1, -4, 3, 3, -4, -22, 12, 3, 4, 3, 12, 1, 6, -1, 7, 2, 1, -7, 4, 1, 2, 0, 4, -9, 14, -3, 2, -4, -22, 7, 20, -7, 8, 11, 0, 6, 6, 1, 9, -7, -4, -1, -10, -12, -5, 4, 2, 9, -6, 3, 4, 4, 4, 14, -30, 14, 21, 2, -8, 21, -25, -3, -2, 7, 12, -3, -3, 13, -1, 3, 3, -3, 3, -7, -13, -2, 0, 9, 2, -4, 22, -20, 4, -12, -9, 1, 7, -3, 14, 17, 8, -8, 9, 2, 15, 3, 1, -3, 4, -1, -6, 5, 10, 4, -4, 3, -8, 3, -1, -2, -2, -3, -1, 3, 4, 10, 3, -2, 7, -12, 4, -3, 38, 9, 8, -12, -7, 20, 8, 4, 0, 23, 2, -1, 8, -1, -5, -43, -17, 6, 11, 1, 3, 9, -4, -2, 2, -17, 2, 8, 23, 11, 2, 11, -3, 23, -4, 3, 8, -7, -18, 2, -1, -6, 3, 15, -1, -1, 3, 1, 1, -4, -5, 2, 15, 3, 2, 13, -11, 1, -3, -11, 13, 2, -2, 5, 15, -1, 0, -8, -21, 19, 8, -8, 4, 12, 2, 12, -7, 2, -2, -7, 0, 1, -3, 3, -6, 1, 3, -22, -1, -2, -6, 2, 6, 3, -6, -10, 11, 1, 5, 23, 3, -16, -1, -12, -17, -4, 11, 2, 1, 5, -1, -3, 2, 1, 1, 9, 12, 3, 3, 3, -11, -1, 1, -2, -6, 9, 13, -14, -34, 10, 0, 2, -5, 0, 1, 1, 5, 0, 0, 14, 11, 4, 3, -3, -16, -10, -13}

#define TENSOR_DENSE_1_KERNEL_0_DEC_BITS {4}

#define TENSOR_DENSE_1_BIAS_0 {-13, -30, 26, 32, 16, 32, -68, -3, -6, -10, 4, 6, 11, 0, -2, 11, 16, -2, 2, 23, 4, 4, -7, 4, 26, -4, 20, 43, 14, -25, -18, 18, -7, -1, -43, 10, -30, 30, 9, -20, 23, 26, -14, 22, -31, -26, -6, 37}

#define TENSOR_DENSE_1_BIAS_0_DEC_BITS {6}

#define DENSE_1_BIAS_LSHIFT {5}

#define DENSE_1_OUTPUT_RSHIFT {5}

#define TENSOR_DENSE_2_KERNEL_0 {-2, -8, 13, 9, -6, 4, 8, 9, 0, -36, -4, -1, -48, -18, 8, -5, 12, 11, 2, 0, -3, 3, -4, 3, 20, 7, 0, 3, 14, -2, 0, -2, -9, -15, -6, 31, -3, -2, 14, 4, 14, -15, 7, -20, 3, -7, 2, -39, -18, -32, -1, 2, 0, 5, -2, 1, -7, 9, -10, 0, -16, -1, -14, -7, 3, -1, 6, -10, -25, 6, -10, -13, -4, 3, -3, 6, 3, -25, 4, -5, -7, -8, -1, -3, 12, -3, -4, -4, -11, 1, -10, -5, 11, -47, 11, -21, -13, 5, -48, -1, 3, -8, -6, 2, -4, 4, 20, -5, 3, 4, -40, 2, -27, 9, -2, -11, -3, 3, 11, 8, -1, -14, -6, 5, -26, 12, -2, -29, -6, -5, 4, -6, 10, 10, -21, -2, -4, 33, 8, -11, 4, -4, 1, -75, -7, -3, -5, 16, -1, -5, -2, 3, -35, -23, -12, -57, -3, 4, -12, -1, 1, 11, -1, -1, 8, 3, -2, -2, 2, 8, -15, -4, -5, 14, -4, -2, 5, -23, -20, -6, -25, 4, -7, 8, 16, -9, -28, -33, -6, -12, -12, 2, -27, 20, -2, -7, -3, 2, -1, -13, -2, -7, 11, -9, 3, -4, -6, 4, -3, 2, 6, -5, 7, -8, 3, 11, 12, -7, -1, 2, 11, -41, 3, 4, -20, -2, 21, 40, -11, 1, 2, -3, 0, -3, 15, -11, 1, 8, 9, 9, -8, -2, 2, 1, -9, -3, 1, -2, 2, 9, -1, 0, -32, -25, -9, -9, 3, 4, 16, -1, -15, 4, -7, 1, 1, -1, -5, 6, 0, 5, -16, 6, -2, 2, 3, -4, -8, 9, -2, 4, -9, 3, 14, -11, -45, -8, 7, 4, 1, 10, 6, -5, 0, -11, 1, -11, 0, 3, 13, -1, 1, 0, -12, -38, -21, 5, -9, -2, -44, -22, -7, 6, 4, 3, 5, 5, -1, 4, 6, -50, 12, -7, -27, -1, -4, 4, -3, 5, -8, -15, 13, 14, 4, -3, 5, -9, 3, -9, -16, 5, 7, 3, 5, 3, -9, 7, -7, 26, -11, -7, 0, 3, 9, 0, 0, -16, -22, -5, 7, -6, 3, 3, -1, 8, -53, -6, 3, -3, -15, -6, -24, -8, -8, -4, -8, 2, 11, 0, -1, -12, -8, 0, 14, -51, 0, -16, 1, 4, 11, 6, 8, 11, 0, 6, 0, 6, -2, 8, -14, 0, 4, 0, -2, 10, 19, 3, 10, -6, -3, 0, -1, -3, -5, 8, -4, -4, 2, -6, 3, -13, 17, -11, 0, 3, 2, 6, -2, 6, -4, 4, -6, 13, -4, -3, -4, -2, -4, -16, 1, -3, 4, -8, 0, -2, 11, 4, 11, 2, -5, 7, -2, -5, 0, 8, 13, -23, 0, -4, -4, 2, 5, 2, -4, 5, -2, -4, -4, 1, -4, 1, -4, 4, -1, 3, 3, -3, 19, 7, 10, 1, -2, -5, -3, 6, -19, -16, -1, 7, 4, 1, -5, 2, -1, 3, -3, -5, 3, 1, -5, 4, 0, 8, -5, 1, 0, -5, -3, 0, 17, -7, -26, -5, 0, 1, 1, 2, -2, -13, -9, -9, -4, 2, -2, 8, -22, -37, 9, 0, -2, -10, -1, 1, -24, 17, -2, 9, -3, 15, 1, -1, 6, -7, 14, -45, -1, -10, 4, -3, -4, -3, -6, 0, -4, -8, -1, 3, -2, -12, 3, -7, -1, 8, 3, -7, 6, 2, 29, 11, -4, -4, -2, 14, -22, 3, -14, -15, 8, 1, -10, 11, 1, -14, -5, -15, -25, 2, 7, -8, -11, 18, 1, 19, -4, 9, 4, -2, 2, 13, 3, -16, -5, 0, -1, -3, -5, 16, -2, 3, 15, 3, 8, 1, 23, 2, 16, 30, 1, -7, 6, 0, 1, 9, -24, -4, 1, -8, 0, 2, 9, 10, 2, 2, 1, 7, -5, 5, 10, -7, 6, -13, 9, 12, 0, -8, 8, -2, -3, 0, 0, -1, 3, -12, 3, -13, -2, 6, 10, -43, -1, -3, -22, -4, 2, 1, 5, 16, -26, -12, 1, 3, -3, 1, -21, 4, 6, 4, 9, 16, -2, 2, 4, -4, -13, 6, -1, 0, 5, 7, -2, -33, -4, -19, -10, -7, -16, 0, 12, -6, 4, 5, -11, 12, 11, 18, 11, -4, -10, -1, 14, 8, 2, -21, -23, -21, 29, 2, -11, 1, 5, 7, -18, -13, -23, -9, -7, -1, 1, 2, -4, -10, 9, 2, 5, 2, 39, 46, 2, 0, -24, -6, -35, 9, -12, -10, 1, 2, -6, -39, -19, 3, 0, 9, 6, -22, 19, 8, 8, 2, 8, -15, 7, 5, -19, -11, -5, 6, 8, -4, -7, -8, 12, -18, -4, 16, 13, -2, 8, 6, 24, -20, 0, -32, -12, 5, -8, -61, 9, 3, 1, 11, 12, 2, 3, -34, -2, 3, -6, 1, 2, 4, 35, -2, -33, -1, 27, -2, 0, -15, 3, -25, -25, -8, -2, 27, -1, 14, 3, 13, -3, -3, 7, 8, -2, -1, -7, -12, -2, -1, 11, -8, 2, 9, -7, -8, 1, 0, 6, -1, 0, 6, 10, 1, -5, -13, -8, -5, 12, -17, -26, -2, 3, 4, 2, -18, 0, 4, 4, -7, 2, 0, 6, -5, -18, 6, 11, 0, -12, 1, -4, 6, 2, -36, -2, -4, 11, -9, 1, -1, -5, -8, -12, 21, -10, 6, -1, -24, -3, -7, 8, 5, 14, -8, -7, 23, 2, 3, -7, 2, 0, 2, 10, -26, 11, 0, -2, -52, 7, 2, -40, 33, 3, 4, -42, 19, -3, -4, -8, -9, 3, -6, 3, 2, 22, -1, 7, -10, 9, 28, 8, -19, 5, -24, 2, 11, -5, -4, 4, -8, -8, 10, 27, -1, -1, -34, -14, 5, 0, 0, -13, -12, 3, 5, 6, 13, 2, 0, -5, -15, 2, -2, 7, 3, 4, 8, -39, -25, -7, -5, -12, 6, -16, -31, 19, -5, 22, -30, -10, -8, 6, 6, 4, -18, 12, -8, -3, -6, -3, 4, 4, 10, -1, -4, -5, 34, 8, 20, -8, 11, 30, 17, -12, -10, -1, 3, 11, -69, 3, 7, 9, -6, -2, 4, 6, 7, -2, 2, -6, 4, 7, 7, 6, 4, 9, 15, 1, 15, -4, 0, -4, -6, -3, 10, 16, 2, 2, 14, -11, -5, 5, 0, -6, 8, 0, 0, -6, 4, -1, 1, 2, 8, 1, 1, -14, -6, 0, 2, 13, -3, -2, -2, 12, -4, -5, -1, 1, -4, 4, 8, 3, -4, 5, -2, -5, -8, -4, 2, -5, 2, -3, -12, 1, -4, -10, -8, 3, -6, 20, 5, 1, -2, 8, -13, 10, 11, 16, -1, -19, -11, 0, 8, -5, 6, 1, 8, 12, -21, 1, 1, -1, 3, 0, -12, 3, -1, 12, 106, 3, 3, -16, 35, 11, -4, 2, -6, 1, -4, 8, -4, 10, 5, -7, 4, -3, 6, -1, 3, 2, -9, 16, 21, 9, -20, -19, 11, -8, -15, -11, -4, 6, 15, 0, -17, -11, -8, -13, -25, 28, -5, 10, 3, -23, 2, 6, -2, -21, -17, -15, 0, 7, -2, 0, -5, 9, -27, 3, -10, -14, 9, 1, -4, -18, 4, -3, 3, -5, -1, 36, -29, 4, -5, -29, 31, 0, -12, -15, -2, -9, -4, 6, -1, -6, -37, 0, 3, 1, -18, -1, 2, 9, 2, -3, -12, -3, 15, 7, 1, -3, 6, 6, -18, -2, 0, -1, 3, -10, -2, 10, 3, 6, -6, 3, 0, -9, 4, -2, 2, 3, 9, -2, 1, 12, 9, -9, 1, 10, -34, -7, 0, 0, -46, 5, 7, 2, -5, -1, 8, -2, 1, 6, 3, 0, 2, 5, 4, -4, 7, -10, -14, -15, 10, -2, 3, -2, 3, 9, -9, -19, 28, 4, -6, -4, -27, 5, 7, -3, -11, 0, -3, -12, -1, 3, -1, 5, -13, 12, -18, -9, 11, -1, -12, -14, -16, 5, -14, 14, 0, 4, -1, 4, 10, 44, -1, -12, 6, -4, -13, -4, 2, 0, -6, 12, 0, 13, 4, 10, 6, -4, -6, 4, 5, -5, -4, -1, 0, -10, -4, -11, -15, -1, 10, -5, -19, -8, 8, -23, 19, -4, 9, -38, -7, -3, 4, -9, -16, 1, -9, -37, 6, 3, 8, 8, -3, 2, 4, -1, -2, -2, 14, -12, -10, 1, 3, -11, 1, 0, -3, 0, 4, 8, 0, 12, -9, 0, 24, -3, -1, 0, -9, 7, 6, -6, 1, 9, -9, 4, 2, 4, 3, 0, 0, 10, 10, 0, 0, 10, -3, -3, 0, 10, 13, 10, -1, 11, 4, 2, -7, 7, -1, 18, -1, 7, -6, 3, 13, -2, -5, 3, 4, -12, -8, -4, -1, 10, -6, 6, 4, 8, -1, -22, 0, 5, 5, -20, -5, 2, 4, 9, 13, -1, -3, -7, 0, -1, 1, 4, 15, -19, 3, 4, -13, -6, -5, 7, 5, 7, 0, -19, 12, 28, -2, 5, 0, 2, -46, 10, 3, 17, 1, -2, 12, -42, 20, 0, -3, -32, -1, -5, -1, 19, -48, 5, 11, -10, 4, -1, 3, 4, 4, 4, -6, -1, 16, 51, -20, -1, -24, 8, -12, -8, 6, -24, 9, 16, -27, -5, -1, -2, -5, -3, -2, 6, 0, 13, -3, -13, -2, 20, -8, -6, -16, -4, 5, -29, -2, 3, 11}

#define TENSOR_DENSE_2_KERNEL_0_DEC_BITS {4}

#define TENSOR_DENSE_2_BIAS_0 {-12, -2, -5, -13, 13, 27, 14, -74, -1, 23, 19, -20, -15, -14, 34, 6, 14, -45, -7, -9, 32, 10, -24, -16, 59, -12, 32, -3, -29, -17, 2, 24}

#define TENSOR_DENSE_2_BIAS_0_DEC_BITS {6}

#define DENSE_2_BIAS_LSHIFT {4}

#define DENSE_2_OUTPUT_RSHIFT {7}

#define TENSOR_DENSE_3_KERNEL_0 {28, 17, 7, 15, -27, 21, -9, 8, 22, 13, -44, -3, -6, 23, 27, -28, -7, 5, 27, 19, 6, -20, 4, 28, 6, 0, 71, 13, -1, 6, -11, 44, -3, 1, 12, 9, 4, -5, 4, 6, 10, 1, -9, 11, 5, 1, 12, 10, 14, 8, 9, 17, -9, 15, -9, 21, -15, -14, 3, 3, 14, -15, 11, 0, 7, 20, 9, -8, -14, 8, -2, 6, 20, 13, 17, 2, -5, 17, -32, -1, 0, 8, -6, -5, 5, 1, 9, -5, -1, 17, 41, 27, -4, 8, -22, 39, 12, 3, 9, 6, 6, 7, 3, 1, 0, 21, 6, -13, -44, 25, 7, -13, -5, -6, 15, 43, 8, -5, 2, 36, -23, -1, 5, 5, -4, -6, 8, 0, 13, 19, 8, 22, -7, 15, -3, -17, 44, 11, 42, -7, -4, -6, 3, -1, -10, -3, 31, 2, 4, -16, 3, 4, 1, 1, 21, 13, 6, 1, 0, 20, -6, -3, 2, 10, -1, 5, 5, -8, 7, 9, 9, -2, -4, 9, 4, 6, -2, 9, 6, 17, 20, -25, -7, -9, 19, -14, -7, -5, -40, -4, -1, -3, 20, 12, 19, 0, -3, -24, -59, 5, 28, 9, 40, 15, -15, -17, 8, 4, 8, 7, 7, -1, 1, -22, 6, 2, 7, 12, 20, 18, -20, 19, 22, -12, -9, 0, 10, 15, -9, -36, 1, -2, 36, 14, -9, -13, -4, 21, -5, 13, 9, -3, 51, 19, 9, -8, 27, 5, -4, -5, 8, 0, 39, 19, -12, 7, -21, 6, -16, 6, -8, 22, -57, 24, 16, 21, -1, -30, -22, -62, -33, 15, -12, -3, -11, 14, 8, 13, 6, 19, -1, 7, -19, 44, -4, 5, 12, 17, -2, -4, 5, 6, 1, 0, -46, -3, -5, 7, 14, 8, 7, 6, -18, 0, 17, 14, 12, 9, -11, -4, -8, 0, 5, -17, 6, -1, 3, -1, 9, 11, 14, 11, -65, -7, -3, -8, -5, 5, 5, 17, 16, 14, -1, -24, -31, -31, -2, 2, -11, -6, -4, 5, 9, 10, 10, 11, 33, 29, 2, -10, -32, -39, -5, 8, 3, 8, 7, 2, 3, -4, -14, 10, 11, -13, -70, -62, 9, -4, -15, -9, 20, 27, -14, -7, -39, -62, -43, -5, -24, -2, -2, 2, 7, -2, 41, 11, 12, -28, 2, -6, -4, -53, 5, 2, -46, 3, -3, -22, -2, 18, 8, -14, 11, 1, -1, 7, 3, 19, 19, -34, 7, 22, -1, 10, -3, 13, 0, -3, -1, -50, 3, 5, 0, 2, 1, 8, -21, -2, -2, 1, -2, 0, -8, -12, -7, 7, -1, -2, -4, -5, 7, 4, 15, -2, 0, 24, 2, 13, -3, 8, -2, 5, -1, -9, -1, 1, 12, -6, 25, 5, -4, -9, -5, -33, -14, -6, 6, 2, 3, 9, -4, 3, -5, 12, 20, -4, -8, -14, -3, -25, 24, -20, 5, -76, -1, 8, -3, 1, 24, -18, -16, -14, 1, -21, -5, 10, -2, -19, 7, -29, 1, 2, 2, -34, -31, 9, 6, 9, 4, 10, -4, 7}

#define TENSOR_DENSE_3_KERNEL_0_DEC_BITS {4}

#define TENSOR_DENSE_3_BIAS_0 {31, 26, 0, 21, 5, 32, -94, -17, -63, 29, 20, 11, 10, -12, -5, -2}

#define TENSOR_DENSE_3_BIAS_0_DEC_BITS {6}

#define DENSE_3_BIAS_LSHIFT {1}

#define DENSE_3_OUTPUT_RSHIFT {4}

#define TENSOR_DENSE_4_KERNEL_0 {-10, 0, -4, 11, 5, 13, -5, -7, 0, -25, -13, -9, 2, 3, -6, 10, -18, 9, -7, -4, -5, 5, 3, -6, -16, -25, 20, 45, -4, 6, -2, -14, 7, 0, 3, -9, 5, -36, -6, -13, -15, -21, 9, 6, -2, 9, 0, -14, 1, 12, 6, -4, -6, 14, 6, 5, 6, 43, 7, 12, -5, -15, -2, -12, -16, -9, 11, -2, 1, 9, -1, -4, 11, -13, -54, -4, 0, -10, -1, -10, -31, -17, 0, -5, 7, -14, -16, -2, -13, -15, 37, 34, -7, -3, 41, 47, 3, 7, 4, -1, 15, 8, 8, 18, 0, -3, 13, -7, 2, -3, 3, 8, 12, 2, 4, -2, 2, 11, 6, 6, 30, 22, 13, -3, 10, 33, 11, 9, 2, 9, 3, -7, 7, -6, -4, 5, -6, 9, 0, 12, 7, -5, 14, 5, -3, -3, 3, -43, -7, -6, -82, 4, -1, 2, 5, 0, -2, -2, -20, 1, 5, -25, -7, -16, -70, -1, -14, -2, -7, -2, -7, -13, 10, -4, -15, -7, -7, 9, -1, 3, 7, -6, -1, 4, -5, 0, -6, -6, -13, 4, -5, 4}

#define TENSOR_DENSE_4_KERNEL_0_DEC_BITS {4}

#define TENSOR_DENSE_4_BIAS_0 {-81, -117, -10, 85, -9, -89, -87, 14, 0, 70, 48, -5}

#define TENSOR_DENSE_4_BIAS_0_DEC_BITS {7}

#define DENSE_4_BIAS_LSHIFT {0}

#define DENSE_4_OUTPUT_RSHIFT {5}

#define TENSOR_DENSE_5_KERNEL_0 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -21, 0, 74, 0, 0, 0, 0, 0, -15, 0, -30, 0, 0, 0, 0, 0, 0, 0, -54, 0, 0, 0, 0, 0, 0, 0, 45, 0, 0, 0, 0, 0, 49, 0, 0, -8, 68, 0, -9, 83, 0, 31, 27, 0, -16, -6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

#define TENSOR_DENSE_5_KERNEL_0_DEC_BITS {9}

#define TENSOR_DENSE_5_BIAS_0 {0, -23, -11, 80, 10, -10}

#define TENSOR_DENSE_5_BIAS_0_DEC_BITS {7}

#define DENSE_5_BIAS_LSHIFT {4}

#define DENSE_5_OUTPUT_RSHIFT {7}

#define TENSOR_DENSE_6_KERNEL_0 {0, 0, 0, -16, 59, 0, 0, 0, 0, 86, -61, 0}

#define TENSOR_DENSE_6_KERNEL_0_DEC_BITS {9}

#define TENSOR_DENSE_6_BIAS_0 {4, 84}

#define TENSOR_DENSE_6_BIAS_0_DEC_BITS {8}

#define DENSE_6_BIAS_LSHIFT {5}

#define DENSE_6_OUTPUT_RSHIFT {7}

#define TENSOR_DENSE_7_KERNEL_0 {-75, 110}

#define TENSOR_DENSE_7_KERNEL_0_DEC_BITS {6}

#define TENSOR_DENSE_7_BIAS_0 {98}

#define TENSOR_DENSE_7_BIAS_0_DEC_BITS {8}

#define DENSE_7_BIAS_LSHIFT {4}

#define DENSE_7_OUTPUT_RSHIFT {5}


/* output q format for each layer */
#define INPUT_1_OUTPUT_DEC 8
#define INPUT_1_OUTPUT_OFFSET 0
#define DENSE_OUTPUT_DEC 7
#define DENSE_OUTPUT_OFFSET 0
#define RE_LU_OUTPUT_DEC 7
#define RE_LU_OUTPUT_OFFSET 0
#define DENSE_1_OUTPUT_DEC 6
#define DENSE_1_OUTPUT_OFFSET 0
#define RE_LU_1_OUTPUT_DEC 6
#define RE_LU_1_OUTPUT_OFFSET 0
#define DENSE_2_OUTPUT_DEC 3
#define DENSE_2_OUTPUT_OFFSET 0
#define RE_LU_2_OUTPUT_DEC 3
#define RE_LU_2_OUTPUT_OFFSET 0
#define DENSE_3_OUTPUT_DEC 3
#define DENSE_3_OUTPUT_OFFSET 0
#define RE_LU_3_OUTPUT_DEC 3
#define RE_LU_3_OUTPUT_OFFSET 0
#define DENSE_4_OUTPUT_DEC 2
#define DENSE_4_OUTPUT_OFFSET 0
#define RE_LU_4_OUTPUT_DEC 2
#define RE_LU_4_OUTPUT_OFFSET 0
#define DENSE_5_OUTPUT_DEC 4
#define DENSE_5_OUTPUT_OFFSET 0
#define RE_LU_5_OUTPUT_DEC 4
#define RE_LU_5_OUTPUT_OFFSET 0
#define DENSE_6_OUTPUT_DEC 6
#define DENSE_6_OUTPUT_OFFSET 0
#define RE_LU_6_OUTPUT_DEC 6
#define RE_LU_6_OUTPUT_OFFSET 0
#define DENSE_7_OUTPUT_DEC 7
#define DENSE_7_OUTPUT_OFFSET 0

/* bias shift and output shift for none-weighted layer */

/* tensors and configurations for each layer */
static int8_t nnom_input_data[1] = {0};

const nnom_shape_data_t tensor_input_1_dim[] = {5};
const nnom_qformat_param_t tensor_input_1_dec[] = {8};
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

const nnom_shape_data_t tensor_dense_kernel_0_dim[] = {5, 64};
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

const nnom_shape_data_t tensor_dense_bias_0_dim[] = {64};
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

const nnom_shape_data_t tensor_dense_1_kernel_0_dim[] = {64, 48};
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

const nnom_shape_data_t tensor_dense_1_bias_0_dim[] = {48};
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

const nnom_shape_data_t tensor_dense_2_kernel_0_dim[] = {48, 32};
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

const nnom_shape_data_t tensor_dense_2_bias_0_dim[] = {32};
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

const nnom_shape_data_t tensor_dense_3_kernel_0_dim[] = {32, 16};
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

const nnom_shape_data_t tensor_dense_3_bias_0_dim[] = {16};
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

const nnom_shape_data_t tensor_dense_4_kernel_0_dim[] = {16, 12};
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

const nnom_shape_data_t tensor_dense_4_bias_0_dim[] = {12};
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

const nnom_shape_data_t tensor_dense_5_kernel_0_dim[] = {12, 6};
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

const nnom_shape_data_t tensor_dense_5_bias_0_dim[] = {6};
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
const int8_t tensor_dense_6_kernel_0_data[] = TENSOR_DENSE_6_KERNEL_0;

const nnom_shape_data_t tensor_dense_6_kernel_0_dim[] = {6, 2};
const nnom_qformat_param_t tensor_dense_6_kernel_0_dec[] = TENSOR_DENSE_6_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_6_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_6_kernel_0 = {
    .p_data = (void*)tensor_dense_6_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_6_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_6_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_6_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_6_bias_0_data[] = TENSOR_DENSE_6_BIAS_0;

const nnom_shape_data_t tensor_dense_6_bias_0_dim[] = {2};
const nnom_qformat_param_t tensor_dense_6_bias_0_dec[] = TENSOR_DENSE_6_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_6_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_6_bias_0 = {
    .p_data = (void*)tensor_dense_6_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_6_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_6_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_6_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_6_output_shift[] = DENSE_6_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_6_bias_shift[] = DENSE_6_BIAS_LSHIFT;
const nnom_dense_config_t dense_6_config = {
    .super = {.name = "dense_6"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_6_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_6_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_6_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_6_bias_shift
};
const int8_t tensor_dense_7_kernel_0_data[] = TENSOR_DENSE_7_KERNEL_0;

const nnom_shape_data_t tensor_dense_7_kernel_0_dim[] = {2, 1};
const nnom_qformat_param_t tensor_dense_7_kernel_0_dec[] = TENSOR_DENSE_7_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_7_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_7_kernel_0 = {
    .p_data = (void*)tensor_dense_7_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_7_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_7_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_7_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_7_bias_0_data[] = TENSOR_DENSE_7_BIAS_0;

const nnom_shape_data_t tensor_dense_7_bias_0_dim[] = {1};
const nnom_qformat_param_t tensor_dense_7_bias_0_dec[] = TENSOR_DENSE_7_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_7_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_7_bias_0 = {
    .p_data = (void*)tensor_dense_7_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_7_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_7_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_7_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_7_output_shift[] = DENSE_7_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_7_bias_shift[] = DENSE_7_BIAS_LSHIFT;
const nnom_dense_config_t dense_7_config = {
    .super = {.name = "dense_7"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_7_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_7_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_7_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_7_bias_shift
};
static int8_t nnom_output_data[1] = {0};

const nnom_shape_data_t tensor_output0_dim[] = {1};
const nnom_qformat_param_t tensor_output0_dec[] = {DENSE_7_OUTPUT_DEC};
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
	nnom_layer_t* layer[17];

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
	layer[10] = model.active(act_relu(), layer[9]);
	layer[11] = model.hook(dense_s(&dense_5_config), layer[10]);
	layer[12] = model.active(act_relu(), layer[11]);
	layer[13] = model.hook(dense_s(&dense_6_config), layer[12]);
	layer[14] = model.active(act_relu(), layer[13]);
	layer[15] = model.hook(dense_s(&dense_7_config), layer[14]);
	layer[16] = model.hook(output_s(&output0_config), layer[15]);
	model_compile(&model, layer[0], layer[16]);
	return &model;
}
