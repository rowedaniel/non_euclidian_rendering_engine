/**
 * @file vector.h
 * @brief simple vector library
 * @author Daniel Neshyba-Rowe
 */


// TODO: figure out how to handle adaptive dimensionality at compile-time
#include "dim.h"

/**
 * @struct Vector2d
 * @brief 2D integer vector
*/
typedef union Vector2d {
    struct {
        int x, y;
    };
    struct {
        int u, v;
    };
    int data[2];
} Vector2d;

/**
 * @struct Vector3
 * @brief 3D vector
 */
typedef union Vector3 {
  struct {
    double x, y, z;
  };
  struct {
    double r, g, b;
  };
  double data[3];
} Vector3;


/**
 * @struct VectorN
 * @brief vector of same dimension as space
 */
typedef union VectorN {
  double data[N_DIM];
} VectorN;

