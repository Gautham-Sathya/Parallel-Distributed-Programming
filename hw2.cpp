#include <iostream>
#include <cstdint>
#include <cmath>
#include <thread>
using namespace std;
void threadCount(bool primes[],uint64_t s, uint64_t f, uint64_t* count){

    uint64_t c = 0;
    for(int i = s|1; i<f; i+=2){
        if(primes[i])
        c++;
    }
    *count = c;

}
//O(log log n)
uint64_t eratosthenes(bool primes[], uint64_t n) {
	uint64_t count = 1; //special case, 2 is prime
	for (uint64_t i = 3; i <= n; i += 2) {
		primes[i] = true; // only write odd ones
	}
	const uint64_t lim = sqrt(n);
	for (uint64_t i = 3; i <= lim; i += 2) {
		if (primes[i]) {
			count++;
			for (uint64_t j = i * i; j <= n; j += 2*i) {
				primes[j] = false;
			}
		}
	}
	// if (lim% 2 != 0) {
	// 	lim += 2;
	// } else {
	// 	lim += 1;
	// }
	// (lim + 1) | 1 means round up to next odd number
	uint64_t start = (lim+1)|1;
    uint64_t increment = (n-start)/5;
	//; 9 not counted 
    //9 11 13 15 16; 
    //16|1 -> 23 
    uint64_t count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0;
    thread t1(threadCount,primes,start,start+increment,&count1);
    thread t2(threadCount,primes,start+increment,start+2*increment,&count2);
    thread t3(threadCount,primes,start+2*increment,start+3*increment,&count3);
    thread t4(threadCount,primes,start+3*increment,start+4*increment,&count4);
    thread t5(threadCount,primes,start+4*increment,n,&count5);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    count +=(count1+count2+count3+count4+count5);
    return count;
}


int main(int argc, char* argv[]) {
	uint64_t n = argc > 1 ? atol(argv[1]) : 1000000;
// all modern OS will crash if you allocate more than 4MB on stack
    //int x;
	//int y[1024];
	//int* z[1024*1024]; // BOOM
	bool* primes = new bool[n+1]; 
     cout<<"Starting "  << n<<"\n";
    auto start = chrono::high_resolution_clock::now();
	uint64_t count = eratosthenes(primes, n);
    cout << count << '\n';
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end-start;
     cout<<"\n5 Thread Runtime: " << duration.count() << "\n";
	delete [] primes;
}