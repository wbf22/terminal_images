#ifndef LIST
#define LIST

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
    A list of elements of any type with the following operation complexities
    - len(...) -> O(1)
    - l_push(...) -> O(1) amoritized
    - l_push_front(...) -> O(1) amoritized
    - l_pop(...) -> O(1)
    - l_pop_front(...) -> O(1)
    - l_get(...) -> O(1)
    - l_set(...) -> O(1)
    - l_clear() -> O(n)
    - free_list() -> O(n)
    - l_slice() -> O(n)


    This list implementation stores pointers to objects inserted by the user. These
    object can be on the stack or the heap. Object will NOT be cleaned up with
    free_list(), only the list's meta data is freed. 

    When done with your list you must call free_list() to avoid memory leaks.

    To update an index in the list do something like this:
    ```
    *(int*) l_get(list, 0) = 14;
    ```

    or this
    ```
    int data = 12;
    l_set(list, 0, &data, sizeof(int));
    ```


    Take care inserting objects on the stack. If those objects go out of scope or the objects
    are overwritten in a loop or something, the list will behave unexpectedly. 

    For example this
    ```

    List* list = new_list();
    for (int i = 0; i < 10; i++) {
        l_push(list, &i);
    }
    for(int i = 0; i < list->len; ++i) {
        int ele = *(int*) l_get(list, i);
        printf("%d, ", ele);
    }
    free_list(list);
    ```

    outputs:
    ```
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
    ```

    To insert one through 9 you'd have to do this:
    ```

    List* list = new_list();
    for (int i = 0; i < 10; i++) {
        int* d = malloc(sizeof(int));
        *d = i;
        l_push(list, &d);
    }
    for(int i = 0; i < list->len; ++i) {
        int* ele = (int*) l_get(list, i);
        printf("%d, ", *ele);
        free(ele);
    }
    free_list(list);
    ```

*/
typedef struct List {
    void** data; // pointer to array of pointers
    size_t data_size;

    size_t start;
    size_t end;
    size_t len;
} List;



static void list_mem_error_exit_failing() {
    fprintf(stderr, "List couldn't get more memory on the system! Exiting...");
    exit(EXIT_FAILURE);
}


List* new_list_s(size_t size) {
    List *p_list = malloc(sizeof(List));
    if (p_list == NULL) {
        list_mem_error_exit_failing();
    }
    p_list->data = malloc(size * sizeof(void*));
    if (p_list->data == NULL) {
        free(p_list);
        list_mem_error_exit_failing();
    }
    p_list->data_size = size;
    p_list->start = 0;
    p_list->end = 0;
    p_list->len = 0;

    return p_list;
}

List* new_list() {
    return new_list_s(10);
}



/*
    doubles the internal array of the list

    returns 0 if successful
*/
int resize(List* list) {

    /*
        int size = size();
        T[] newData = (T[]) new Object[data.length * 2];
        if (startIndex > endIndex) {
            int startSize = this.data.length - startIndex;
            System.arraycopy(this.data, startIndex, newData, 0, startSize);
            System.arraycopy(this.data, 0, newData, startSize, endIndex);
        }
        else if (startIndex == endIndex) {
            // do nothing empty array
        }
        else {
            System.arraycopy(this.data, startIndex, newData, 0, size);
        }

        data = newData;
        startIndex = 0;
        endIndex = size;
    */

    size_t new_size = list->data_size * 2;
    void** new_data = malloc(new_size * sizeof(void*));
    if (new_data == NULL) {
        list_mem_error_exit_failing();
    }

    if (list->start > list->end) {

        // copy start index to array end
        memcpy(
            new_data, 
            list->data + list->start, 
            (list->data_size - list->start) * sizeof(void*)
        );
        // copy array start to end index
        memcpy(
            new_data + (list->data_size - list->start), 
            list->data, 
            list->end * sizeof(void*)
        );
    }
    else {
        memcpy(
            new_data, 
            list->data + list->start, 
            (list->end - list->start) * sizeof(void*)
        );
    }

    // freeing list but not contents
    free(list->data);
    list->data = new_data;
    list->data_size = new_size;
    list->start = 0;
    list->end = list->len;


    return 0;
}




/*
    Function to s_add to the end of a list.
    May resize interanl size of the list,
    but amoritized is a constant operation.

    A reference to either a stack or heap variable
    can be passed in. This pointer will be stored 
    in the list. If this data is freed or goes out
    of scope, the list will be in a bad state.
*/
void l_push(List* list, void* data) {
    
    if (list->len == list->data_size - 1) {
        resize(list);
    }

    list->data[list->end] = data;
    list->end = (list->end + 1) % list->data_size;
    list->len = list->len + 1;
}

/*
    Function to s_add to the end of a list.
    May resize interanl size of the list,
    but amoritized is a constant operation.

    A reference to either a stack or heap variable
    can be passed in. This pointer will be stored 
    in the list. If this data is freed or goes out
    of scope, the list will be in a bad state.
*/
void l_push_front(List* list, void* data) {

    if (list->len == list->data_size - 1) {
        resize(list);
    }

    list->start = (list->start - 1 + list->data_size) % list->data_size;
    list->data[list->start] = data;
    list->len = list->len + 1;
}

/*
    Removes and retrieves the last element of the list.

    The data is returned in the form void* and should probably
    be cast to the correct type:
    ```
    List* list = new_list();
    int i = 10;
    l_push(list, &i);
    int ele = *(int*) l_pop(list);
    ```
*/
void* l_pop(List* list) {

    if (list->len == 0) {
        return NULL;
    }


    list->end = (list->end - 1 + list->data_size) % list->data_size;
    list->len = list->len - 1;
    void* data = list->data[list->end];
    list->data[list->end] = NULL;

    return data;
}

/*
    Removes and retrieves the first element of the list.

    The data is returned in the form void* and should probably
    be cast to the correct type:
    ```
    List* list = new_list();
    int i = 10;
    l_push(list, &i);
    int ele = *(int*) l_pop(list);
    ```
*/
void* l_pop_front(List* list) {

    if (list->len == 0) {
        return NULL;
    }

    void* data = list->data[list->start];
    list->data[list->start] = NULL;
    list->start = (list->start + 1) % list->data_size;
    list->len = list->len - 1;

    return data;
}



static int convert_index(List* list, int index) {


    while (index < 0) {
        index += list->len;
    }

    int in_bounds = index < list->len;
    if (!in_bounds) {
        fprintf(stderr, "Index ");
        fprintf(stderr, "%d", index);
        fprintf(stderr, " out of bounds for len ");
        fprintf(stderr, "%d", list->len);
        exit(EXIT_FAILURE);
    }
    
    return (list->start + index) % list->data_size;
}


/*
    Get's a pointer to a list element (so changes to pointer object
    will change element on the list)
*/
void* l_get(List* list, int index) {
    int real_index = convert_index(list, index);
    return list->data[real_index];
}

/*
    Copies the value in data to the index in the list. No new memory
    is allocated, and the original pointer inserted at the index is 
    updated. 
*/
void l_set(List* list, int index, void* data, size_t data_size) {
    int real_index = convert_index(list, index);
    memcpy(list->data[real_index], data, data_size);
}



/*
    Clears the list, and frees all object copies on the heap
    created with pushes and sets
*/
void l_clear(List* list) {
    // Free each pointer inside data
    for (size_t i = 0; i < list->data_size; i++) {
        list->data[i] = NULL;
    }
    list->len = 0;
    list->start = 0;
    list->end = 0;
}

/*
    frees the list and stored objects
*/
void free_list(List* list) {
    // Free data
    free(list->data);
    // Free the struct itself
    free(list);
}


/*
    Makes a new list from the specified range (inclusive exclusive).
    The resulting list will share objects with this list. (All objects
    in the list are just pointers to your objects)
*/
List* l_slice(List* list, int start, int end) {
    int real_start = convert_index(list, start);
    int real_end = convert_index(list, end);
    int new_len = 0;
    if (real_start > real_end) {
        new_len = list->data_size - real_start + real_end;
    }
    else {
        new_len = real_end - real_start;
    }

    List* new_list = new_list_s(new_len * 2);

    if (real_start > real_end) {

        // copy start index to array end
        memcpy(
            new_list->data, 
            list->data + real_start, 
            (list->data_size - real_start) * sizeof(void*)
        );
        // copy array start to end index
        memcpy(
            new_list->data + (list->data_size - real_start), 
            list->data, 
            real_end * sizeof(void*)
        );
    }
    else {
        memcpy(
            new_list->data, 
            list->data + real_start, 
            (real_end - real_start) * sizeof(void*)
        );
    }


    new_list->start = 0;
    new_list->end = new_len;
    new_list->len = new_len;

    return new_list;
}


void l_sort(List* list, int (* _Nonnull __compar)(const void *, const void *)) {
    resize(list); // get list data all in a row
    qsort(list->data, list->len, sizeof(void*),  __compar);
}



#endif