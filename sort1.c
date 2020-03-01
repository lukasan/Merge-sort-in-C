#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

struct two_array
{
    int first;
    int second;
};

struct node {
    int first;
    int second;
    struct node *next;
};

int n = 8; // n > 9 freezes OS if not enough RAM (need <8gb)

void merge_sort(struct two_array arr[], int l, int r);
void merge_sort_linked(struct node **static_head);
void merge(struct two_array *arr, int l, int m, int r);
void FrontBackSplit(struct node *source, struct node **frontRef, struct node **backRef);
struct node *SortedMerge(struct node *a, struct node *b);
void push_linked(struct node **head_ref, int first, int second);
void generate_doubleAr(struct two_array *structArr, int count);
void generate_linked(struct node *linked, int count);
void print_doubleAr(struct two_array *arrDubs, int count);
void print_linked_list(struct node *node);

void benchmark_array(int n, bool linear);
void benchmark_linked(int n, bool linear);

// MERGE SORT

//TODO: linked list memory management free()

int main(int argc, char *argv[]){
    bool benchMethod = true;
    if (argc < 4){
        printf("ERROR: not enough arguments.\nUsage: [a | l] [length] [method 0 | 1]\n");
        return 1;
    }
    if (argc > 4) {
        printf("ERROR: too many arguments.\nUsage: [a | l] [length] [method 0 | 1]\n");
        return 1;
    }
    char type = *argv[1];
    if(type != 'a' && type != 'l'){
        printf("ERROR: illegal array argument.\nUsage: [a | l] [length] [method 0 | 1]\n");
        return 1;
    }
    if (*argv[3] == '0' && atoi(argv[2]) > 8){
        char in;
        printf("WARNING: Quadratic method with power of >8 can halt OS. You entered: %d\nAre you sure? [y\\n] ", atoi(argv[2]));
        scanf("%c", &in);
        if (in != 'y') return 0;
    }

    if (argc == 3) n = atoi(argv[2]);
    if (argc == 4){
        n = atoi(argv[2]);
        benchMethod = atoi(argv[3]);
    }

    if (type == 'a'){
        benchmark_array(n, benchMethod);
        return 0;
    }

    if (type == 'l'){
        benchmark_linked(n, benchMethod);
        return 0;
    }

    return 0;
}

void benchmark_array(int n, bool linear){
    if (linear){
        printf("Starting array merge sort benchmark: LINEAR\n\n");
        struct two_array *data = malloc(n * sizeof(struct two_array));

        generate_doubleAr(data, n);

        printf("Sorting...%d\n",n);
        clock_t t;

        // BENCHMARK ARRAY
        t = clock();
        merge_sort(data, 0, n);
        t = clock() - t;
        double taken = ((double)t) / CLOCKS_PER_SEC;
        printf("Done. Took %f seconds.\n\n", taken);

        if (!data) free(data); // work-around array is not even -> segfaults
        return;
    }
    printf("Starting array merge sort benchmark: QUADRATIC\n\n");
    for (int i = 1; i < n; i++){
        int count = pow(n, i);
        struct two_array *data1 = malloc(count * sizeof(struct two_array));
        //int count = sizeof(*data1) / sizeof(struct two_array);

        generate_doubleAr(data1, count);

        //print_doubleAr(data1, count);
        
        //printf("---------- Two-Array ----------\n\n");
        //printf("+ STARTING DATA +\n");

        printf("Sorting...%d\n ",count);
        clock_t t;

        // BENCHMARK ARRAY
        t = clock();
        merge_sort(data1, 0, count);
        t = clock() - t;
        double taken = ((double)t) / CLOCKS_PER_SEC;
        printf("Done. Took %f seconds.\n\n", taken);

        //printf("+ SORTED DATA +\n");
        //print_doubleAr(data1, count);
        if (!data1) free(data1); // work-around array is not even -> segfaults
    }
}

void benchmark_linked(int n, bool linear){
    if (linear){
        struct node *linked = NULL;
        generate_linked(linked, n);

        printf("Sorting...%d\n", n);
        clock_t t;

        t = clock();
        merge_sort_linked(&linked);
        t = clock() - t;
        double taken = ((double)t) / CLOCKS_PER_SEC;
        printf(" Done. Took %f seconds.\n\n", taken);
        free(linked);
        return;
    }
    for (int i = 1; i < n; i++){
        struct node *linked = NULL;
        int count = pow(n, i);
        generate_linked(linked, count);

        printf("Sorting...%d\n", count);
        clock_t t;

        t = clock();
        merge_sort_linked(&linked);
        t = clock() - t;
        double taken = ((double)t) / CLOCKS_PER_SEC;
        printf(" Done. Took %f seconds.\n\n", taken);
        free(linked);
    }
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
        //printf("m = %d", m);
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

    if (!L) free(L);
    if (!R) free(R);
}

void merge_sort_linked(struct node **static_head){
    struct node *head = *static_head;
    struct node *a;
    struct node *b;

    // Nothing to sort
    if ((head == NULL) || (head->next == NULL)){
        return;
    }

    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(head, &a, &b);

    /* Recursively sort the sublists */
    merge_sort_linked(&a);
    merge_sort_linked(&b);

    /* answer = merge the two sorted lists together */
    *static_head = SortedMerge(a, b);
}

/* See https:// www.geeksforgeeks.org/?p=3622 for details of this  
function */
struct node *SortedMerge(struct node *a, struct node *b)
{
    struct node *result = NULL;
  
    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);
  
    // first key
    if (a->first < b->first) {
        result = a;
        result->next = SortedMerge(a->next, b);
    }

    // second key
    else if (a->first == b->first){
        if (a->second < b->second){
            result = a;
            result->next = SortedMerge(a->next, b);
        }
        else{
            result = b;
            result->next = SortedMerge(a, b->next);
        }
    }
    
    else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}

/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves, 
    and return the two lists using the reference parameters. 
    If the length is odd, the extra node should go in the front list. 
    Uses the fast/slow pointer strategy. */
void FrontBackSplit(struct node *source,
                    struct node **frontRef, struct node **backRef)
{ 
    struct node *fast;
    struct node *slow;
    slow = source;
    fast = source->next;
  
    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
  
    /* 'slow' is before the midpoint in the list, so split it in two 
    at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}
  
/* Function to print nodes in a given linked list */
void print_linked_list(struct node *node)
{
    while (node != NULL) {
        printf("%d - %d \n", node->first, node->second);
        node = node->next;
    }
}
  
/* Function to insert a node at the beginging of the linked list */
void push_linked(struct node **head_ref, int first, int second)
{
    /* allocate node */
    struct node *new_node = (struct node*)malloc(sizeof(struct node));
  
    new_node->first = first;
    new_node->second = second;
  
    /* link the old list off the new node */
    new_node->next = (*head_ref);
  
    /* move the head to point to the new node */
    (*head_ref) = new_node;
} 

void generate_doubleAr(struct two_array *structArr, int count){
    printf("Generating...%d\n", count);
    for (int i = 0; i < count; i++){
        structArr[i].first = rand();
        structArr[i].second = rand();
    }
    printf(" Done.\n");
}

void generate_linked(struct node *linked, int count){
    if (linked != NULL){
        printf("\nERROR: generating list failed\n");
        return;
    }
    printf("Generating...%d\n", count);
    for (int i = 0; i < count; i++){
        push_linked(&linked, rand(), rand());
    }
    printf(" Done.\n");
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