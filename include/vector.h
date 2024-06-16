/**
 * @file vector.h
 * @brief simple vector library
 * @author Daniel Neshyba-Rowe
 */

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
*/
void print_vectorN(VectorN vector);

/**
 * @brief Print a 3-dimensional vector
*/
void print_vector3(Vector3 vector);

/**
 * @brief Print a 2-dimensional vector
*/
void print_vector2(Vector2 vector);

/**
 * @brief Print a 2-dimensional integer vector
*/
void print_vector2d(Vector2d vector);


/**
 * @brief Add two vectors
 * @param vec1 the first vector to add
 * @param vec2 the second vector to add
 * @param out pointer to write result to
*/
void vec_add(double vec1[], double vec2[], int n, double out[]);
Vector2 vec2_add(Vector2 vec1, Vector2 vec2);
Vector3 vec3_add(Vector3 vec1, Vector3 vec2);
VectorN vecN_add(VectorN vec1, VectorN vec2);
/**
 * @brief Compute the dot product of two double-precision vectors
 * @param vec1 The first vector
 * @param vec2 The second vector
 * @param n The dimensionality of the vectors
*/
double dot(double vec1[], double vec2[], int n);
double vec2_dot(Vector2 vec1, Vector2 vec2);
double vec3_dot(Vector3 vec1, Vector3 vec3);
double vecN_dot(VectorN vec1, VectorN vecN);
