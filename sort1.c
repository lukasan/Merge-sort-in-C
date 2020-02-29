/* 

    n1 - first num to cmp
    n2 - second num, if n1 is equal
    c  - memory address; data sector pointer
    whole sector - c + 2;

    array = {n1 , n2, c, n1, n2, c, ...}

    Two key sort
*/

#include <stdlib.h>
#include <stdio.h>

struct two_array
{
    int first;
    int second;
};

void merge_sort(struct two_array arr[], int l, int r);
void merge(struct two_array arr[], int l, int m, int r);
void print_array(int *arr, int count);
void print_linked(int *arr, int count);
void print_doubleAr(struct two_array *arrDubs, int count);


// MERGE SORT

int main(int argc, char *argv[]){
    
    int data1[] = {1, 4, 3, 6, 2, 9};
    int count = sizeof(data1) / sizeof(int);

    // du skaiciai kaip vienas objektas
    struct two_array data11[4] = {
        {1, 2},
        {1, 1},
        {2, 3},
        {3, 1}
    };

    int count1 = sizeof(data11) / sizeof(struct two_array);
    print_doubleAr(data11, count1);
    
    printf("---------- Two-Array ----------\n\n");
    printf("+ STARTING DATA +\n");

    merge_sort(data11, 0, count);
    printf("+ SORTED DATA +\n");
    print_doubleAr(data11, count1);

    printf("---------- Linked ----------\n\n");
    printf("+ STARTING DATA +\n");
    
    printf("+ SORTED DATA +\n");
    
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
        printf("m = %d", m);
        // Sort first and second halves 
        merge_sort(arr, l, m); 
        merge_sort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
}

void merge_sort_linked(int arr[], int l, int r){
    if (3 < r){
        int m = l+(r-3)/2;
        merge_sort_linked(arr, l, m);
        merge_sort_linked(arr, m+3, r);

        merge_linked(arr, l, m, r);
    }
}

/*  Merges two subarrays of arr[]. 
    First subarray is arr[l..m] 
    Second subarray is arr[m+1..r]
*/
void merge(struct two_array arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    /* create temp arrays */
    struct two_array L[n1], R[n2]; 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
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
} 

/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void merge_linked(int arr[], int l, int m, int r){
    // INDEX ONLY MERGE??!?!?! AAAAAAAAAAAAA
    int size = sizeof(arr)/sizeof(int);
    for (int i = 0; i < size; i+=3){
        printf("%d - %d --> %d", arr[i], arr[i+1], arr[i+2]);
    }
    printf("\n__________________\n");
}

void print_array(int *arr, int count){

    for (int i = 0; i < count; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n\n");
}

// array = {c, n1, n2, c, n1, n2, ...}
void print_linked(int *arr, int count){
    printf("Memory address of array start: %p \n", arr);
    for (int i = 0; i < count; i++) // main loop for current sector
    {
        for (int j = 0; j < count; j+=3) // inloop for arranging output
        {
            if (i+1 >= count && arr[j] == 0) { // edge case where last item is left
                printf("%d (%p) %d (%p) -> %d\n", arr[j+1], &arr[j+1], arr[j+2], &arr[j+2], arr[j]);
                continue;
            }
            if (i+2 == arr[j]){ // main case that arranges "linked" items
                printf("%d (%p) %d (%p) -> %d\n", arr[j+1], &arr[j+1], arr[j+2], &arr[j+2], arr[j]);
            }
        }
    }
    printf("\n\n");
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