/**
 * @file vector.h
 * @brief simple vector library
 * @author Daniel Neshyba-Rowe
 */

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
 * @struct Vector2
 * @brief 2D vector
 */
typedef union Vector2 {
  struct {
    double x, y;
  };
  struct {
    double u, v;
  };
  double data[2];
} Vector2;


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


/**
 * @brief Print a double-precision vector
 * @param vector The data contained in the vector
 * @param n The size (dimension) of the vector
*/
void print_vector(double vector[], int n);

/**
 * @brief Print a vector of same dimension as the space
 * @param vector The N-dimensional vector
*/
void print_vectorN(VectorN vector);

/**
 * @brief Print a 3-dimensional vector
 * @param vector The 3-dimensional vector
*/
void print_vector3(Vector3 vector);

/**
 * @brief Print a 2-dimensional vector
 * @param vector The 2-dimensional vector
*/
void print_vector2(Vector2 vector);

/**
 * @brief Print a 2-dimensional integer vector
 * @param vector The vector
*/
void print_vector2d(Vector2d vector);
