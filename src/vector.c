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


void vec_add(double vec1[], double vec2[], int n, double out[]) {
    for(int i=0; i<n; ++i) {
        out[i] = vec1[i] + vec2[i];
    }
}

Vector2 vec2_add(Vector2 vec1, Vector2 vec2) {
    Vector2 out;
    vec_add(vec1.data, vec2.data, 2, out.data);
    return out;
}

Vector3 vec3_add(Vector3 vec1, Vector3 vec2) {
    Vector3 out;
    vec_add(vec1.data, vec2.data, 3, out.data);
    return out;
}

VectorN vecN_add(VectorN vec1, VectorN vec2) {
    VectorN out;
    vec_add(vec1.data, vec2.data, N_DIM, out.data);
    return out;
}

double dot(double vec1[], double vec2[], int n) {
    double total = 0;
    for(int i=0; i<n; ++i) {
        total += vec1[i]*vec2[i];
    }
    return total;
}

double vec2_dot(Vector2 vec1, Vector2 vec2) {
    return dot(vec1.data, vec2.data, 2);
}

double vec3_dot(Vector3 vec1, Vector3 vec2) {
    return dot(vec1.data, vec2.data, 3);
}

double vecN_dot(VectorN vec1, VectorN vec2) {
    return dot(vec1.data, vec2.data, N_DIM);
}

Vector3 vec3_cross(Vector3 vec1, Vector3 vec2) {
    Vector3 vec = {
        {
            vec1.y*vec2.z - vec1.z*vec2.y,
            vec1.z*vec2.x - vec1.x*vec2.z,
            vec1.x*vec2.y - vec1.y*vec2.x
        }
    };
    return vec;
}
