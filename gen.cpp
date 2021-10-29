#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

#define MULTIPLIER 16807
#define MODULUS    2147483647

long long saved_seed;
int hi = 1000000, lo = 1;

long long random(int a, int b)
{
  register int hi, lo;

  hi = MULTIPLIER * (saved_seed >> 16);
  lo = MULTIPLIER * (saved_seed & 0xffff);
  hi += (lo>>16);
  lo &= 0xffff;
  lo += (hi>>15);
  hi &= 0x7fff;
  lo -= MODULUS;
  if ((saved_seed = (hi<<16) + lo) < 0)
    saved_seed += MODULUS;

  if (b <= a)
    return b;
  return a + saved_seed % (b - a + 1);
} 

int n_rows, n_cols, k;

int main(int argc, char *argv[])
{
	if(argc < 5)
		return 1;
	n_rows = atoi(argv[1]), n_cols = atoi(argv[2]), k = atoi(argv[3]), saved_seed = atoi(argv[4]);
	printf("%d %d %d\n", n_rows, n_cols, k);
	for (int j = 0; j < k; ++j)
		for (int i = 0; i < n_cols; ++i)
			printf("%lld%c", random(lo, hi), i == n_cols-1 ? '\n' : ' ');
	
	vector <int> row;
	for (int c = 0; c < n_rows; ++c) {
		for (int i = 1; i <= n_cols; ++i) {
			if(random(1, 100) < 15)
				row.push_back(i);
		}
		if((int)row.size() == 0)
			row.push_back(random(1, n_cols));
		printf("%d ", (int)row.size());
		for(int i = 0; i < (int)row.size(); i++)
			printf("%d%c", row[i], i == (int)row.size()-1 ? '\n' : ' ');
		row.clear();
	}
	return 0;
}
