#ifndef SHUFFLER_H
#define SHUFFLER_H
/*洗牌功能的標頭檔*/
#include "MJtile.h"
#include "time.h"
#include <ctime>
#include <cstdlib>

// #ifdef WINDOWS
#ifndef DWORD
#define WINAPI
typedef unsigned long DWORD;
typedef short WCHAR;
typedef void * HANDLE;
#define MAX_PATH    PATH_MAX
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int BOOL;
#endif 
// #endif

//產生亂數
static uint64_t GetTickCount()//為了讓43行可以跑
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64_t)(ts.tv_nsec / 1000000) + ((uint64_t)ts.tv_sec * 1000ull);
}

class Shuffler {
private:
	MJtile rand_mj[144]; //整個牌都洗過
	int seed;
public:
	Shuffler() {}

	~Shuffler() {}

	void init() {
		// seed = time(NULL);
		
		seed = GetTickCount(); 
		srand(seed);

		// 依照照片中的編號排列麻將
		for (int i = 0; i < 144; i++) {
			rand_mj[i] = MJtile(i + 1);
		}

		// 開始洗牌～
		for (int j = 0; j < 144; j++) {
			int r = rand() % 144;
			MJtile temp = rand_mj[j];
			rand_mj[j] = rand_mj[r];
			rand_mj[r] = temp;
		}

	}

	void fill(MJtile mj[]) {
		for (int i = 0; i < 144; i++) {
			mj[i] = rand_mj[i];
		}
	}

	int getSeed(void){
		return seed;
	}

};

#endif
