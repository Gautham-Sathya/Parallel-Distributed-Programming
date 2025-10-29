#include <immintrin.h>
#include <cstdint>
#include <cmath>
#include <complex>
#include <iostream>
#include <fstream>



void checkM(__m256 a, __m256 b, uint32_t* count_arr,int i, int j, int steps){
__m256 x = a;
__m256 y = b;
__m256i iter = _mm256_setzero_si256(); //iteration count
    for(int k = 0; k<50; k++){//iterations 20 is arbitrary
//mandelbrot iteration
__m256 x2 = _mm256_mul_ps(x, x);
__m256 y2 = _mm256_mul_ps(y, y);
__m256 xy = _mm256_mul_ps(x, y);
__m256 mag2 = _mm256_add_ps(x2, y2); //magnitude squared

//make mask
__m256 mask = _mm256_cmp_ps(mag2, _mm256_set1_ps(4.0), _CMP_LT_OS); //mask for mag2 < 4.0
__m256 zx = _mm256_add_ps(_mm256_sub_ps(x2, y2),a);//zn+1 real = x2 - y2 + x0
__m256 zy = _mm256_add_ps(_mm256_mul_ps(_mm256_mul_ps(_mm256_set1_ps(2.0f), x),y), b); //zn + 1 imag = 2xy + y0

//blend mask
x = _mm256_blendv_ps(x, zx, mask);
y = _mm256_blendv_ps(y, zy, mask);

__m256i mask_i = _mm256_castps_si256(mask); //integer mask for and incmrement
iter = _mm256_add_epi32(iter, _mm256_and_si256(mask_i, _mm256_set1_epi32(1))); //incrementer

}
int indx = i + j*steps;
_mm256_storeu_si256((__m256i*)&count_arr[indx], iter); //store iteration counts at row major order

}

void savePPM(const uint32_t* mcount, int width, int height) {//https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html
    std::ofstream out("mandelbrot.ppm");
    out << "P3\n" << width << " " << height << "\n255\n"; // P3 = text RGB

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            uint32_t count = mcount[j * width + i];
            int color = (count * 5) % 256; // simple coloring; 5 was chosen since 256/50 iterations = ~5
            out << color << " " << color << " " << color << " ";
        }
        out << "\n";
    }

    out.close();
}

int main() {//g++ -O3 -mavx2 hw3.cpp -o hw3.exe && .\hw3.exe

    int w = 3; //-2 to 1
    int h = 3 ;
    int steps = 8*300;
    float fdx = (1.0*w) / steps;  
    float fdy = (1.0*h) / steps;
    __m256 dy = _mm256_set1_ps(fdy); // (0.1, 0.1, 0.1, ...)
    __m256 s = _mm256_set_ps(0, 1, 2, 3, 4, 5, 6, 7);
    __m256 a = _mm256_set1_ps(-2); //start
    __m256 dx = _mm256_mul_ps(_mm256_set1_ps(fdx), s); // (0dx, 1dx, 2dx, ....)

    uint32_t* mcount = new uint32_t[steps*steps];

    for(int i =0; i<steps; i+=8){ //~100 i from -2 to 1
        __m256 x = _mm256_add_ps(a, dx); //made x = a + dx
        __m256 y = _mm256_set1_ps(-1.5); //start 
        for(int j =0; j<steps; j++){ //100i from -1.5 to 1.5
            
            //for each xy, record count 
            checkM(x,y, mcount,i,j,steps);

            y = _mm256_add_ps(y, dy); //a+=8dx
        }
        a = _mm256_add_ps(a, _mm256_set1_ps(8*fdx)); //a+=8dx
    }
    std::cout << mcount << std::endl;
    std::cout << "Done!" << std::endl;
    savePPM(mcount, steps, steps);
        delete[] mcount;
}



