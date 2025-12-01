#include <cuda_runtime.h>
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <chrono>
/*
Gautham Sathyanarayanan PDP

C++ CPU Simd based  Ryzen 9 AI 270 laptop  : Computation time: 4.2464 seconds for 6400 x 6400
GPU based: 5060 laptop :  GPU computation time: 1.67008 seconds
6400 x 6400

8*1600 x 8*1600
simd: 18s 
gpu: 1.79s


*/



__global__ void m(unsigned char* img, int w, int h, int max) {
   
  int x = blockIdx.x * blockDim.x + threadIdx.x;
int y = blockIdx.y * blockDim.y + threadIdx.y; //index
    double zr = 0;
    double zi = 0;
    int i =0;
    if(x>=w || y>= h)
    return;

    double r = -2.0 + x*(3/(double)w);
    double im = -1.5  +y*(3/(double)h);
    
    while(zr*zr + zi*zi <=4.0&& i < max) {
        double temp = zr*zr - zi*zi + r;
        zi = 2*zr*zi + im;
        zr = temp;
        i++;
    }
    //grayscale 0-255
    img[y*w + x] = (unsigned char(255 * i/max));
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    int w = 8*1600;
    int h = w;
    int max = 100;
    unsigned char* img = new unsigned char[w*h]; //image arr
    
    unsigned char* d_img; 
    cudaMalloc(&d_img, w*h*sizeof(unsigned char));
    
    dim3 block(16,16);
    dim3 grid((w+block.x-1)/block.x, (h+block.y-1)/block.y); //round up
    m<<<grid,block>>>(d_img, w,h,max);
    cudaDeviceSynchronize();

    cudaMemcpy(img, d_img, w*h,cudaMemcpyDeviceToHost);
    stbi_write_png("mandel.png", w, h,1,img,w);
    cudaFree(d_img);
    delete[] img;
    std::cout<<"done\n";


    
auto end = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> elapsed = end - start;

std::cout << "GPU computation time: " << elapsed.count() << " seconds\n";
}

