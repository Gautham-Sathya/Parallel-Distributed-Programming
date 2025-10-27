#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
using namespace std;


double addF(int n){
double sum = 0;
for (int i = 1; i <= n; i++)
    sum+= 1.0/i;

    return sum;
}

double addB(int n){
  double sum = 0;
for (int i = n; i >0; i--)
    sum+= 1.0/i;  

    return sum;
}

void mThreadAdd(int s, int e, double* psum)
{
    double sum = 0;
    for(int i = s; i<e; i++)
    {
        sum += 1.0/i;
    } 
    *psum = sum;
}

int main(){
    int max = 100000000;
    cout<< fixed;
    cout.precision(15);
        //SINGLE CORE
        cout<<"F: " <<addF(max)<< "\n";
        auto start1 = chrono::high_resolution_clock::now();
    
        cout<<"B: " <<addB(max)<< "\n";
        auto end1 = chrono::high_resolution_clock::now();

        chrono::duration<double> duration1 = end1-start1;
       cout<< "Runtime: "<<duration1.count()<<" s\n\n";

        auto start = chrono::high_resolution_clock::now();
        double sum1 = 0, sum2 =0;
        thread t1(mThreadAdd,1,max/2,&sum1);
        thread t2(mThreadAdd,max/2,max+1,&sum2);
        t1.join();
        t2.join();
        auto end = chrono::high_resolution_clock::now();
        
        chrono::duration<double> duration = end-start;
        cout<<"2 Thread: " <<sum1+sum2<< "\n";
        cout<< "2 Thread Runtime: "<<duration.count()<<" s\n\n";


        auto start2 = chrono::high_resolution_clock::now();
        double sum3 =0, sum4 =0;
        thread tt1(mThreadAdd,1,max/4,&sum1);
        thread tt2(mThreadAdd,max/4,max/2,&sum2);
        thread tt3(mThreadAdd,max/2,(3*max)/4,&sum3);
        thread tt4(mThreadAdd,(3*max)/4,max+1,&sum4);
        tt1.join();
        tt2.join();
        tt3.join();
        tt4.join();        
        auto end2 = chrono::high_resolution_clock::now();
        
        chrono::duration<double> duration2 = end2-start2;
        cout<<"4 Thread: " <<sum1+sum2+sum3+sum4<< "\n";
        cout<< "4 Thread Runtime: "<<duration2.count()<<" s\n\n";

        auto start3 = chrono::high_resolution_clock::now();
        double sum5 =0, sum6 =0, sum7=0, sum8 = 0;
        thread ttt1(mThreadAdd,1,max/8,&sum1);
        thread ttt2(mThreadAdd,max/8,max/4,&sum2);
        thread ttt3(mThreadAdd,max/4,(3*max)/8,&sum3);
        thread ttt4(mThreadAdd,(3*max)/8,max/2,&sum4);
        thread ttt5(mThreadAdd,max/2,(5*max)/8,&sum5);
        thread ttt6(mThreadAdd,(5*max)/8,(3*max)/4,&sum6);
        thread ttt7(mThreadAdd,(3*max/4),(7*max)/8,&sum7);
        thread ttt8(mThreadAdd,(7*max)/8,max+1,&sum8);
        ttt1.join();
        ttt2.join();
        ttt3.join();
        ttt4.join();        
        ttt5.join();
        ttt6.join();
        ttt7.join();
        ttt8.join();        
        auto end3 = chrono::high_resolution_clock::now();
        
        chrono::duration<double> duration3 = end3-start3;
        cout<<"8 Thread: " <<sum1+sum2+sum3+sum4+sum5+sum6+sum7+sum8<< "\n";
        cout<< "8 Thread Runtime: "<<duration3.count()<<" s\n\n";


    return 0;
}
//benchmarks:
/* 
D:\CPP>g++ hw1.cpp -o hw1.exe && hw1.exe
F: 18.997896413852555
B: 18.997896413853447
Runtime: 0.254883200000000 s

2 Thread: 18.997896413853010
2 Thread Runtime: 0.153753500000000 s

4 Thread: 18.997896413849880
4 Thread Runtime: 0.069142600000000 s

8 Thread: 18.997896413851894
8 Thread Runtime: 0.042639900000000 s




*/

