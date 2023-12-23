#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int wt;
	int val;
} Item;

static Item *items;

void shopping(int *purchaseList, int N);

int main(int argc, char const *argv[])
{
	// Read file values in
	FILE *fp;
	int numCases, numItems, numMembers;
	int **familyAccrument; // [people][items]

	if((fp = fopen("shopping.txt", "r")) == NULL) goto exit;
	if(fscanf(fp, "%d", &numCases) == EOF) goto exit;

	for(int i = 0; i < numCases; ++i){

		//Initialize List of Items
		if(fscanf(fp, "%d", &numItems) == EOF) goto exit;
		items = (Item *) malloc (numItems * sizeof(Item));
		for (int j = 0; j < numItems; ++j)
			if(fscanf(fp, "%d %d", &items[j].val, &items[j].wt) == EOF) goto exit;

		// Initialize familyAccrument
		if(fscanf(fp, "%d", &numMembers) == EOF) goto exit;
		familyAccrument = (int **) malloc (numMembers * sizeof(int *));
		for(int j = 0; j < numMembers; ++j){
			familyAccrument[j] = (int *) calloc ((numItems + 3), sizeof(int));
			
			/**
			 * [numItems]:   weight capacity
			 * [numItems+1]: indiv max value
			 * [numItems+2]: number carried
			*/

			// read in individual weight capacity
			if(fscanf(fp, "%d", &familyAccrument[j][numItems]) == EOF) goto exit;
		}

		//Go shopping for every member
		for(int j = 0; j < numMembers; j++)
			shopping(familyAccrument[j], numItems);

		int familyTotal = 0;
		for(int j = 0; j < numMembers; j++)
			familyTotal += familyAccrument[j][numItems+1];

		//Output
		printf("Test Case %d\n", i+1);
		printf("Total Price %d\n", familyTotal);
		for(int j = 0; j < numMembers; j++){
			printf("%d: ", j+1);
			for(int k = familyAccrument[j][numItems+2]-1; k >=0 ; k--)
				printf("%d ", familyAccrument[j][k]);
			putchar('\n');
		}
		putchar('\n');

		for(int j = 0; j < numMembers; j++)
			free(familyAccrument[i]);
		free(familyAccrument);
		free(items);
	}

	exit:;
	if(fp) fclose(fp);
	return 0;
}

// Go Shopping for 1 person
/**
 * IN: List of items purchased w/ weight capacity, max value, and num items carried
 *   : Number of items available for purchase
*/
void shopping(int *purchaseList, int N){
	int MAX_WEIGHT = purchaseList[N];
	int *maxValue = &purchaseList[N+1];
	purchaseList[N+2] = 0;
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

	*maxValue = V[N][MAX_WEIGHT];
	
	// Get Items Aquired
	for(int i = N, w = MAX_WEIGHT; i > 0 && w > 0; i--){
		if(V[i][w] != V[i-1][w]){
			purchaseList[purchaseList[N+2]] = i;
			purchaseList[N+2]++;
			w-=items[i-1].wt;
		}
	}
}