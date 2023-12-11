#include <math.h>
#include <stdio.h>
#include "consts.h"


const double schwarz_rad = 1;
void minkowski_metric(double point[N_DIM], double metric[N_DIM][N_DIM]) {
    for(int i=0; i<N_DIM; ++i) {
        for(int j=0; j<N_DIM; ++j) {
            metric[i][j] = 0;
        }
    }

    metric[0][0] = -1;
    metric[1][1] = 1;
    metric[2][2] = point[1]*point[1];
    metric[3][3] = point[1]*point[1]*sin(point[2])*sin(point[2]);
}
void minkowski_chris(double point[N_DIM], double chris_sym[N_DIM][N_DIM][N_DIM]) {
    for(int i=0; i<N_DIM; ++i) {
        for(int j=0; j<N_DIM; ++j) {
            for(int k=0; k<N_DIM; ++k) {
                chris_sym[i][j][k] = 0;
            }
        }
    }
    chris_sym[1][2][2] = -point[1];
    chris_sym[1][3][3] = -point[1] * sin(point[2]) * sin(point[2]);

    chris_sym[2][1][2] = 1/point[1];
    chris_sym[2][2][1] = 1/point[1];
    chris_sym[2][3][3] = -sin(point[2]) * cos(point[2]);
        
    chris_sym[3][1][3] = 1/point[1];
    chris_sym[3][3][1] = 1/point[1];
    chris_sym[3][2][3] = cos(point[2]) / sin(point[2]);
    chris_sym[3][3][2] = cos(point[2]) / sin(point[2]);

}
void schwarz_metric(double point[N_DIM], double metric[N_DIM][N_DIM]) {
    for(int i=0; i<N_DIM; ++i) {
        for(int j=0; j<N_DIM; ++j) {
            metric[i][j] = 0;
        }
    }

    metric[0][0] = -(1-schwarz_rad/point[1]);
    metric[1][1] = 1/(1-schwarz_rad/point[1]);
    metric[2][2] = point[1]*point[1];
    metric[3][3] = point[1]*point[1]*sin(point[2])*sin(point[2]);
}

void schwarz_chris(double point[N_DIM], double chris_sym[N_DIM][N_DIM][N_DIM]) {
    for(int i=0; i<N_DIM; ++i) {
        for(int j=0; j<N_DIM; ++j) {
            for(int k=0; k<N_DIM; ++k) {
                chris_sym[i][j][k] = 0;
            }
        }
    }

    const double max_val = 1e-6;

    if(point[1] != 0.0 && point[1] != schwarz_rad) {
        chris_sym[0][0][1] = schwarz_rad / (2*point[1]*(point[1] - schwarz_rad));
    } else {
        chris_sym[0][0][1] = max_val;
    }

    if(point[1] != 0.0) {
        chris_sym[1][0][0] = schwarz_rad / (2*point[1]*point[1]*point[1]);
    } else {
        chris_sym[1][0][0] = max_val;
    }
    chris_sym[1][1][1] = -chris_sym[0][0][1];
    chris_sym[1][2][2] = schwarz_rad - point[1];
    chris_sym[1][3][3] = (schwarz_rad - point[1]) * sin(point[2]) * sin(point[2]);

    if(point[1] != 0.0) {
        chris_sym[2][1][2] = 1 / point[1];
    } else {
        chris_sym[2][1][2] = max_val;
    }
    chris_sym[2][3][3] = -cos(point[2]) * sin(point[2]);

    chris_sym[3][1][3] = 1 / point[1];
    if(point[2] != 0.0) {
        chris_sym[3][2][3] = 1 / tan(point[2]);
    } else {
        chris_sym[3][2][3] = max_val;
    }

    for(int i=0; i<N_DIM; ++i) {
        for(int j=1; j<N_DIM; ++j) {
            for(int k=0; k<j; ++k) {
                chris_sym[i][j][k] = chris_sym[i][k][j];
            }
        }
    }
}
