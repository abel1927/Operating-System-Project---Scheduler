#include "list.h"

#define ptoi (int*)&

void print_list(list* l)
{
    if(l->size==0) return;
    for (node* cur = l->head; cur!=NULL; cur=cur->next)
    {
        printf("%d ", *(int*)cur->value);
    }
    printf("\n");
}

void TestSize(list* l, int expected_size, int numberTest)
{
     if(l->size == expected_size)
    {
        printf("Test%d ok\n", numberTest);
    }
    else
    {
        printf("Test%d failed s:%d", numberTest, l->size);
        exit(1);
    }
}

void TestIndex(list* l, int index, int expected_value, int numberTest)
{
    if(*(int*)(getAt(l,index)->value) == expected_value)
    {
       printf("Test%d ok\n", numberTest);
    }
    else
    {
       printf("Test%d failed", numberTest);
       exit(1);
    }
}

int main()
{

    int a = 30, b = 20, c  = 40, d = 18, e = 35, f = 14;
    list* l = init_list(ptoi a);  //  [30] s = 1
    TestSize(l, 1, 0);
    TestIndex(l, 0, 30, 1);

    push_back(l, ptoi b);   //  [30, 20] s = 2
    print_list(l);

    push_front(l, ptoi c);  //  [40, 30 , 20]  s = 3
    TestSize(l,3, 2);
    TestIndex(l, 2, 20, 3);
    print_list(l); 

    delete(l, 1); //  [40, 20]  s=2
    TestSize(l, 2, 4);
    print_list(l);

    push_front(l, ptoi d);  // [18, 40 , 20] s  = 3
    push_back(l,ptoi a);    // [18, 40 , 20, 30] s = 4
    TestSize(l, 4, 5);
    print_list(l);

    insert(l, ptoi e, 2);   //  [18, 40, 35 20 30] s = 5
    TestSize(l, 5, 6);
    TestIndex(l, 2, 35, 7);
    print_list(l);

    int r = *((int*)pop_front(l)->value);  // [40, 35, 20, 30] s = 4 
    printf("%d\n", r);
    print_list(l);

    int q = *((int*)pop_back(l)->value);  // [40, 35, 20]  s = 3
    printf("%d\n", q);
    TestSize(l, 3, 8);
    TestIndex(l, 2, 20, 9);
    print_list(l);

}