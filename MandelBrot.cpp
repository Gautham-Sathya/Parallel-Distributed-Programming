#include <immintrin.h>
#include <cstdint>
#include <cmath>
#include <complex>
#include <iostream>
#include <fstream>
int main() {//g++ -O3 -mavx2 mandelbrot_avx2.cpp -o mandelbrot_avx2
    int w = 3; //-2 to 1
    int h = 3 ;
    float steps = 800;
    float fdx = (w/steps);  
    float fdy = (h/steps);
    __m256 dy = _mm256_set1_ps(fdy); // (0.1, 0.1, 0.1, ...)
    __m256 s = _mm256_set_ps(0, 1, 2, 3, 4, 5, 6, 7);
    __m256 a = _mm256_set1_ps(-2); //start
    __m256 dx = _mm256_mul_ps(dy, s); // (0dx, 1dx, 2dx, ....)

    uint32_t* mcount = new uint32_t[w*h];

    for(int i =0; i<steps/8; i++){ //~100 i from -2 to 1
        __m256 x = _mm256_add_ps(a, dx); //made x = a + dx
        __m256 y = _mm256_set1_ps(-1.5); //start 
        for(int j =0; j<steps/8; j++){ //100i from -1.5 to 1.5
            
            //for each xy, record count 
            checkM(x,y, mcount);

            y = _mm256_add_ps(y, dy); //a+=8dx
        }
        a = _mm256_add_ps(a, _mm256_set1_ps(8*fdx)); //a+=8dx
    }
}

void checkM(__m256 x, __m256 y, uint32_t count_arr[]){


}
