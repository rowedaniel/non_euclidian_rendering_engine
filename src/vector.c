#include "vector.h"
#include <stdio.h>

void print_vector(double vector[], int n) {
    for(int i=0; i<n; ++i)
        printf("%lf ", vector[i]);
    printf("\n");
}

void print_vectorN(VectorN vector) {
    print_vector(vector.data, N_DIM);
}

void print_vector3(Vector3 vector) {
    print_vector(vector.data, 3);
}

void print_vector2(Vector2 vector) {
    print_vector(vector.data, 2);
}

void print_vector2d(Vector2d vector) {
    for(int i=0; i<2; ++i)
        printf("%d ", vector.data[i]);
    printf("\n");
}
