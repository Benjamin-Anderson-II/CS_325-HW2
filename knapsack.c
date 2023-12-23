/**
 * Benjamin Anderson II
 * Homework 2 for CS_325 @ Oregon State University Summer 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

typedef struct {
	int wt;
	int val;
} Item;

static int MAX_WEIGHT;
static Item *items;

int dynamic(int N);
int recursive(int k, int w, int n);

int main(int argc, char const *argv[])
{
	int N, maxRec, maxDP;
	struct timeval recStart, recStop, DPStart, DPStop;

	srand(time(NULL));

	for(int N = 10; N <= 50; N+=5){
		MAX_WEIGHT = 100;

		//Create Wieghts and Vals
		items = (Item *)malloc(N * sizeof(Item));
		for(int j = 0; j < N; j++){
			items[j].val = (rand() % 100) + 1;
			items[j].wt  = (rand() % MAX_WEIGHT+1) + 1;
		}

		//Get dynamic runtime
		gettimeofday(&DPStart, NULL);
		//printf("%ld\n", DPStart.tv_usec);
		maxDP = dynamic(N);
		gettimeofday(&DPStop, NULL);

		//Get recursive runtime
		gettimeofday(&recStart, NULL);
		maxRec = recursive(0, MAX_WEIGHT, N);
		gettimeofday(&recStop, NULL);

		//Format Output
		/*printf("N=%d W=%d\tRec time = %f\tDP time = %f\tmax Rec = %d\tmax DP = %d\n", 
			N, MAX_WEIGHT, 
			(float)((recStop.tv_sec - recStart.tv_sec) * 1000000 + recStop.tv_usec - recStart.tv_usec)/(float)1000000, 
			(float)((DPStop.tv_sec - DPStart.tv_sec) * 1000000 + DPStop.tv_usec - DPStart.tv_usec)/(float)1000000, 
			maxRec, maxDP);
		*/
		//Just Numbers
		printf("%f\t%f\n", 
			(float)((recStop.tv_sec - recStart.tv_sec) * 1000000 + recStop.tv_usec - recStart.tv_usec)/(float)1000000, 
			(float)((DPStop.tv_sec - DPStart.tv_sec) * 1000000 + DPStop.tv_usec - DPStart.tv_usec)/(float)1000000);

		// Free items
		free(items);
	}
	return 0;
}

int dynamic(int N){
	int V[N+1][MAX_WEIGHT+1];

	//GET MAX VALUE
	for(int w = 0; w <= MAX_WEIGHT; w++)
		V[0][w] = 0;
	for(int i = 1; i <= N; i++){
		V[i][0] = 0;
		for(int w = 1; w <= MAX_WEIGHT; w++){
			int wi = items[i-1].wt, vi = items[i-1].val;

			if(wi <= w && vi + V[i-1][w-wi] > V[i-1][w])
				V[i][w] = vi + V[i-1][w-wi];
			else
				V[i][w] = V[i-1][w];
		}
	}
	return V[N][MAX_WEIGHT];
}

int recursive(int k, int w, int n){
	//Base Case
	if(k >= n || w <= 0) return 0;

	int wi = items[k].wt, vi = items[k].val;

	if(wi <= w){
		int a = vi + recursive(k+1, w-wi, n), 
		    b = recursive(k+1, w, n);
		return (a>b)?a:b;
	} else
		return recursive(k+1, w, n);
}