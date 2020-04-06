#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct two_array
{
    int first;
    int second;
};

int n; // global array length

void merge_sort(struct two_array arr[], int l, int r);
void merge(struct two_array *arr, int l, int m, int r);
void generate_doubleAr(struct two_array *structArr, int count);
void print_doubleAr(struct two_array *arrDubs, int count);
void benchmark_array(int n);

int main(int argc, char *argv[]){
    if (argc == 1){
        printf("Not enough arguments.\n");
        return 1;
    }
    
    n = atoi(argv[1]);
    struct two_array *array = malloc(n * sizeof(struct two_array));
    generate_doubleAr(array, n);
    
    printf("Sorting %d items...\n", n);
    clock_t t;

    // BENCHMARK ARRAY
    t = clock();
    merge_sort(array, 0, n);
    t = clock() - t;
    double taken = ((double)t) / CLOCKS_PER_SEC;
    printf("Done. Took %f seconds.\n\n", taken);

    if (!array) free(array);

    return 0;
}

/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void merge_sort(struct two_array arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        int m = l+(r-l)/2;
        // Sort first and second halves 
        merge_sort(arr, l, m);
        merge_sort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}

/*  Merges two subarrays of arr[]. 
    First subarray is arr[l..m] 
    Second subarray is arr[m+1..r]
*/
void merge(struct two_array *arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    struct two_array
        *L = malloc(sizeof(struct two_array) * n1),
        *R = malloc(sizeof(struct two_array) * n2);
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) // actual sorting logic
    {
        // first key
        if (L[i].first < R[j].first)
        {
            arr[k] = L[i];
            i++;
        }

        // first key is equal
        else if (L[i].first == R[j].first)
        {
            // second key
            if (L[i].second < R[j].second){
                arr[k] = L[i];
                i++;
            }
            else
            {
                arr[k] = R[j];
                j++;
            }
        }

        // first key
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 

    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    }

    if (!L) free(L);
    if (!R) free(R);
}

void generate_doubleAr(struct two_array *structArr, int count){
    printf("Generating %d items... ", count);
    for (int i = 0; i < count; i++){
        structArr[i].first = rand();
        structArr[i].second = rand();
    }
    printf("Done.\n");
}

void print_doubleAr(struct two_array *structArr, int count)
{
    printf("Struct printing, first node address: %p\n", structArr);
    for (int i = 0; i < count; i++)
    {
        printf("%d - %d\n", structArr[i].first, structArr[i].second);    
    }
    printf("\n");
};
