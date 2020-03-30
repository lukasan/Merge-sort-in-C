#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct node {
    int first;
    int second;
    struct node *next;
};

int n; // global list length
struct node *list = NULL;

void merge_sort_linked(struct node **static_head);
void FrontBackSplit(struct node *source, struct node **frontRef, struct node **backRef);
struct node *SortedMerge(struct node *a, struct node *b);
void push_linked(struct node **head_ref, int first, int second);
void free_nodes(struct node *list);
void generate_linked();
void print_linked_list(struct node *node);

int main(int argc, char *argv[]){
    if (argc == 1){
        printf("Not enough arguments.\n");
        return 1;
    }

    n = atoi(argv[1]);
    generate_linked(list, n);
    printf("Sorting...%d\n", n);
    clock_t t;

    t = clock();
    merge_sort_linked(&list);
    t = clock() - t;
    double taken = ((double)t) / CLOCKS_PER_SEC;
    printf(" Done. Took %f seconds.\n\n", taken);

    free_nodes(list);
    return 0;
}

void merge_sort_linked(struct node **static_head){
    struct node *head = *static_head;
    struct node *a;
    struct node *b;

    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL)) { 
        return; 
    } 

    // Nothing to sort
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
        printf("%d - %d\n", node->first, node->second);
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

void generate_linked(){
    if (list != NULL){
        printf("\nERROR: generating list failed\n");
        return;
    }
    printf("Generating %d items... ", n);
    for (int i = 0; i < n; i++){
        push_linked(&list, rand(), rand());
    }
    printf("Done.\n");
}

void free_nodes(struct node *list){
    struct node *temp = list;
    while (list->next){
        temp = list->next;
        free(list);
        list = temp;
    }
    free(temp);
}