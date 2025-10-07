#ifndef MAP
#define MAP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Element {
    char* key;
    size_t key_size;
    void* data;
} Element;

/*
    An unordered hash map implementation.

    Used like so:
    ```
    
    typedef struct MyStruct {
        int x;
        int y;
    } MyStruct;

    // insert into map
    Map* map = new_map();
    MyStruct object = {1, 3};
    unique(map, "my_key", &object);

    // update an element (2 ways)
    MyStruct* back_out = (MyStruct*) at(map, "my_key");
    back_out->y = 14;
    back_out = (MyStruct*) at(map, "my_key");
    printf("{%d,%d}\n",back_out->x, back_out->y);

    MyStruct replacement = {10, 33};
    insert(map, "my_key", &replacement, sizeof(replacement));
    back_out = (MyStruct*) at(map, "my_key");
    printf("{%d,%d}\n",back_out->x, back_out->y);

    // remove element
    erase(map, "my_key");

    // get map length
    printf("%d\n", map->len);

    // iterate over map elements
    for (int i = 0; i < 2; i++) {
        MyStruct* new_object = malloc(sizeof(MyStruct));
        new_object->x = 1 + i;
        new_object->y = 3 + i;

        char key[5];
        make_key(&i, sizeof(int), key, 5);
        unique(map, key, new_object);
    }

    Element** items = map_elements(map);
    for (int i = 0; i < map->len; ++i) {
        Element* item = items[i];
        char* key = item->key;
        MyStruct obj = *(MyStruct*) item->data;

        printf("%s{%d,%d}\n", key, obj.x, obj.y);
    }

    // clean up
    for (int i = 0; i < map->len; ++i) {
        Element* item = items[i];
        MyStruct* obj = (MyStruct*) item->data;
        free(obj);
    }
    free(items); // map_elements allocates memory for the 'items' array
    free_map(map);


    ```

    The map does not free any items placed in it, so if you create
    objects on the heap and put them in the map, make sure to clean 
    them up yourself after calling 'free_map' or after erasing the items
    from the map.

    You can mix heap and stack items in the map as well as mixing types
    if you desire. 

    The map returns NULL if a value is not in the map instead of exiting the program
    or something.



    # METHODS

    Methods with their time complexity
    - insert() int_insert() any_insert() unique() int_unique() any_unique() -> O(1) amoritized
    - at() int_at() any_at() -> O(1) amoritized
    - erase() int_erase() any_erase() -> O(1) amoritized
    - free_map() -> O(n)
    - clear_map() -> O(n)
    - map_elements() -> O(n)
    - contains() int_contains() any_contains() -> O(1) amoritized



    # DESIGN
    
    The map uses a hash table (an array) with keys converted to indices
    with a hash function. We use djb2's hash function for this. 

    On hash collions we use probing with a double hash function to insert the element.
    When the hash table is 70% full we do a resize, using the next prime table size
    in a predefined primes list.



*/
typedef struct Map {
    Element** data; // pointer to array of pointers
    size_t data_size;
    size_t len;
} Map;

const char* DELETED_KEY = "<DELETED>";

const size_t PRIMES[] = {
    127,
    257,
    509,
    1021,
    2053,
    4099,
    9311,
    18253,
    37633,
    65713,
    115249,
    193939,
    505447,
    1062599,
    2017963,
    4393139,
    6972593,
    13466917,
    30402457,
    57885161,
    99990001,
    370248451,
    492366587,
    715827883,
    6643838879,
    8589935681,
    32212254719,
    51539607551, // 412 GB of 8 byte elements
    
    // just for fun probably
    80630964769,
    119327070011,
    228204732751,
    1171432692373,
    1398341745571,
    9809862296159,
    15285151248481,
    304250263527209,
    1746860020068409,
    10657331232548839,
    790738119649411319,
    2305843009213693951 // 18,446 PB of 8 byte elements, more than largest super computers have in ram
};

static void map_mem_error_exit_failing() {
    fprintf(stderr, "Map couldn't get more memory on the system! Exiting...");
    exit(EXIT_FAILURE);
}


static Map* new_map_s(size_t size) {

    Map *map = malloc(sizeof(Map));
    if (map == NULL) {
        map_mem_error_exit_failing();
    }
    map->data = malloc(size * sizeof(void*));
    if (map->data == NULL) {
        free(map);
        map_mem_error_exit_failing();
    }
    map->data_size = size;
    for (int i = 0; i < size; ++i) {
        map->data[i] = NULL;
    }
    map->len = 0;

    return map;
}

/*
    Creates an empty map
*/
Map* new_map() {
    return new_map_s(PRIMES[0]);
}

/*
    Function to hash an object's data into a key. Not necessary for use with
    this Map implementation, but can be handy. 

    Use like so:
    ```
    typedef struct MyStruct {
        int x;
        int y;
        float f;
    } MyStruct;

    MyStruct my_struct;
    my_struct.x = 1;
    my_struct.y = 2;
    my_struct.f = 19;

    char key[10]; 
    make_key(&my_struct, sizeof(my_struct), key, 10);
    printf("%s", key);
    ```
*/
void make_key(void* object, size_t object_size, char* output, size_t output_len) {
    unsigned char* bytes = (unsigned char*)object;
    unsigned long hash = 5381; // Initialize with a prime number (DJB2's initial value)

    // Process each byte
    const char charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    for (size_t i = 0; i < output_len; i++) {
        char byte = bytes[i % object_size];
        hash = (hash << 5) + hash + byte + 1;
        int index = hash % 36;
        output[i] = charset[index];
    }

    // null terminate
    output[output_len-1] = '\0';
}

static size_t hash(void* key, size_t key_size) {
    unsigned char* bytes = (unsigned char*)key;
    size_t hash = 5381;

    for (size_t i = 0; i < key_size; i++)
        hash = ((hash << 5) + hash) + bytes[i];

    return (size_t)hash;
}

static size_t hash3(int failures) {
    size_t hash = 5381;

    for (size_t i = 0; i < failures; i++)
        hash = (hash * 131) + failures;

    // not odd or zero
    if (hash % 2 == 0) {
        hash += 1;
    }

    return (size_t)hash;
}

static size_t probe(Map* map, void* key, size_t key_size, int* hash_collisions) {
    size_t index = hash(key, key_size) % map->data_size;
    size_t first_index = index;

    int open_or_match = map->data[index] == NULL;
    if (map->data[index] != NULL && map->data[index]->key_size == key_size) {
        open_or_match = open_or_match || memcmp(map->data[index]->key, key, key_size) == 0;
    }
    while (!open_or_match) {
        ++(*hash_collisions);
        index = (first_index + hash3(*hash_collisions)) % map->data_size;

        open_or_match = map->data[index] == NULL;
        if (map->data[index] != NULL && map->data[index]->key_size == key_size) {
            open_or_match = open_or_match || memcmp(map->data[index]->key, key, key_size) == 0;
        }
    }

    return index;
}


static void free_map_data(Map* map) {
    for (size_t i = 0; i < map->data_size; ++i) {
        if (map->data[i] != NULL) {
            free( map->data[i]->key);
            free( map->data[i]);
        }
    }
    free(map->data);
}

/*
    Used for freeing the maps meta data. You should clean up map objects yourself,
    as the map cannot discover and clean up pointers to objects within objects. This
    function only cleans up the maps structural data.

    So free your objects, then call this to free the map, and everything should be
    cleaned up.
*/
void free_map(Map* map) {
    free_map_data(map);
    free(map);
}


static void insert_no_resize(Map* map, void* key, size_t key_size, void* data, size_t data_size) {
    int hash_collisions = 0;
    size_t index = probe(map, key, key_size, &hash_collisions);


    // if new element
    if (map->data[index] == NULL) {

        // copy the key
        char* key_copy = malloc(key_size);
        if (key_copy == NULL) {
            fprintf(stderr, "Map couldn't get more memory on the system! Exiting...");
            exit(EXIT_FAILURE);
        }
        memcpy(key_copy, key, key_size);

        // make a new element
        Element *element = malloc(sizeof(Element));
        if (element == NULL) {
            map_mem_error_exit_failing();
        }
        element->key = key_copy;
        element->key_size = key_size;
        element->data = data;
        map->data[index] = element;

        ++map->len;
    }
    else {
        if (data_size != -1) {
            memcpy(map->data[index]->data, data, data_size);
        }
        else {
            fprintf(stderr, "Element already exists. Aborting to avoid leaving an unfreed pointer. (use 'insert()' to overwrite elements or simply retrieve and modify elements) Exiting...");
            exit(EXIT_FAILURE);
        }

    }


}

static void resize_map(Map* map) {
    size_t NUM_PRIMES = sizeof(PRIMES) / sizeof(PRIMES[0]);

    // get next table size
    size_t new_table_size = PRIMES[0];
    for (int i = 0; i < NUM_PRIMES; ++i) {
        if (PRIMES[i] > map->data_size) {
            new_table_size = PRIMES[i];
            break;
        }
    }

    // make new table and insert each element
    size_t DELETED_KEY_SIZE = strlen(DELETED_KEY) + 1;
    Map* new_map = new_map_s(new_table_size);
    for (size_t i = 0; i < map->data_size; ++i) {

        // if an element insert into new map
        if (map->data[i] != NULL) {

            // check if deleted
            int deleted = 0;
            if (map->data[i]->key_size == DELETED_KEY_SIZE) {
                if (strcmp(map->data[i]->key, DELETED_KEY) == 0) {
                    deleted = 1;
                }
            }

            // an element insert into map
            if (!deleted) {
                Element* element = map->data[i];
                insert_no_resize(new_map, element->key, element->key_size, element->data, -1);
            }
        }
    }

    free_map_data(map);
    map->data = new_map->data;
    map->data_size = new_map->data_size;
    map->len = new_map->len;

    new_map->data = NULL; // so it won't free the data array copied above
    free(new_map);
}


/*
    Function to insert an object in the map, with any other object used as the key.

    This can be used like so:
    ```
    Map* map = new_map();
    MyStruct key = {1, "hi"};
    MyStruct2 object = {1, 3};

    any_unique(map, &key, sizeof(key), &object);
    ```
    Does not allow overwriting existing elements, as this would leave an
    unfreed pointer. 

    Use the insert methods to overwrite object, or simply retrieve objects
    and modify them.

*/
void any_unique(Map* map, void* key, size_t key_size, void* data) {

    insert_no_resize(map, key, key_size, data, -1);

    // resize if neeeded
    if (map->data_size * 0.7 < map->len) {
        resize_map(map);
    }
}

/*
    Function to insert an object in the map using an int as the key.
    Does not allow overwriting existing elements, as this would leave an
    unfreed pointer. 

    Use the insert methods to overwrite object, or simply retrieve objects
    and modify them.
*/
void int_unique(Map* map, int key, void* data) {
    any_unique(map, &key, sizeof(int), data);
}

/*
    Function to insert an object in the map using a string as they key.
    Does not allow overwriting existing elements, as this would leave an
    unfreed pointer. 

    Use the insert methods to overwrite object, or simply retrieve objects
    and modify them.
*/
void unique(Map* map, char* key, void* data) {
    any_unique(map, key, (strlen(key) + 1) * sizeof(char), data);
}


/*
    Function to insert an object in the map, with any other object used as the key.

    This can be used like so:
    ```
    Map* map = new_map();
    MyStruct key = {1, "hi"};
    MyStruct2 object = {1, 3};

    any_insert(map, &key, sizeof(key), &object, sizeof(object));
    ```

*/
void any_insert(Map* map, void* key, size_t key_size, void* data, size_t data_size) {

    insert_no_resize(map, key, key_size, data, data_size);

    // resize if neeeded
    if (map->data_size * 0.7 < map->len) {
        resize_map(map);
    }
}

/*
    Function to insert an object in the map using an int as the key.
*/
void int_insert(Map* map, int key, void* data, size_t data_size) {
    any_insert(map, &key, sizeof(int), data, data_size);
}

/*
    Function to insert an object in the map using a string as the key.

    The key is copied so the key passed in can be freed before the map
    if needed. (the key copy is freed by the free_map() function)
*/
void insert(Map* map, char* key, void* data, size_t data_size) {
    any_insert(map, key, (strlen(key) + 1) * sizeof(char), data, data_size);
}


/*
    Function to get an object in the map, with any other object used as the key.

    This can be used like so:
    ```
    Map* map = new_map();
    MyStruct key = {1, "hi"};
    MyStruct2 object = (MyStruct2*) any_at(map, &key, sizeof(key));
    ```

    Returns a NULL pointer if no object exists at the key
*/
void* any_at(Map* map, void* key, size_t key_size) {
    int hash_collisions = 0;
    size_t index = probe(map, key, key_size, &hash_collisions);
    if(map->data[index] == NULL) {
        return NULL;
    }

    return map->data[index]->data;
}

/*
    Function to get an object in the map, using an int as the key
*/
void* int_at(Map* map, int key) {
    return any_at(map, &key, sizeof(key));
}

/*
    Function to get an object in the map, using an string as the key
*/
void* at(Map* map, char* key) {
    return any_at(map, key, (strlen(key) + 1) * sizeof(char));
}



/*
    Function to erase an object in the map, with any other object used as the key.

    This can be used like so:
    ```
    Map* map = new_map();
    MyStruct key = {1, "hi"};
    any_erase(map, &key, sizeof(key));
    ```

    If the key doesn't exist nothing happens
*/
void* any_erase(Map* map, void* key, size_t key_size) {
    int hash_collisions = 0;
    size_t index = probe(map, key, key_size, &hash_collisions);

    if (map->data[index] == NULL) {
        return NULL;
    }

    void* data = map->data[index]->data;
    Element* element = map->data[index];
    free(element->key);
    free(element);

    if (hash_collisions != 0) {

        // make deleted key
        size_t DELETED_KEY_SIZE = strlen(DELETED_KEY) + 1;
        char deleted_key_copy[DELETED_KEY_SIZE];
        strcpy(deleted_key_copy, DELETED_KEY);

        // set as deleted
        map->data[index]->key = deleted_key_copy;
        map->data[index]->key_size = DELETED_KEY_SIZE;
        map->data[index]->data = NULL;

        --map->len;
    }
    else {
        map->data[index] = NULL;
        --map->len;
    }

    return data;
}

/*
    Function to erase an object in the map, using an int as the key.

    If the key doesn't exist nothing happens
*/
void* int_erase(Map* map, int key) {
    return any_erase(map, &key, sizeof(key));
}

/*
    Function to erase an object in the map, using an string as the key.

    If the key doesn't exist nothing happens
*/
void* erase(Map* map, char* key) {
    return any_erase(map, key, (strlen(key) + 1) * sizeof(char));
}


/*
    Returns the elements in the map which are defined which
    contain both a key and a data array. Useful for iterating
    over map elements.

    These elements are only references to elements in the map.
    They should not be deleted or the map will be in a broken
    state. However, the array of elements must be cleaned up or
    there will be memory leaks. (so free the array, not the elements)
*/
Element** map_elements(Map* map) {
    Element** array = malloc(map->len * sizeof(Element));

    int l = 0;
    size_t DELETED_KEY_SIZE = strlen(DELETED_KEY) + 1;
    for (size_t i = 0; i < map->data_size; ++i) {
        if (map->data[i] != NULL) {
            
            int deleted = 0;
            if (map->data[i]->key_size == DELETED_KEY_SIZE) {
                if (strcmp(map->data[i]->key, DELETED_KEY) == 0) {
                    deleted = 1;
                }
            }

            if(!deleted) {
                array[l] = map->data[i];
                ++l;
            }
        }
    }

    return array;
}


void clear_map(Map* map) {
    free_map(map);
    map = new_map();
}


/*
    Function to determine if an object is in the map, with any other object used as the key.

    This can be used like so:
    ```
    Map* map = new_map();
    MyStruct key = {1, "hi"};
    if(any_contains(map, &key, sizeof(key))) {
        printf("yay!");
    }
    ```
*/
int any_contains(Map* map, void* key, size_t key_size) {

    int hash_collisions = 0;
    size_t index = probe(map, key, key_size, &hash_collisions);
    if (map->data[index] == NULL) {
        return 0; 
    }

    return 1;
}

/*
    Function to determine if an object is in the map, with an int used as the key.
*/
int int_contains(Map* map, int key) {
    return any_contains(map, &key, sizeof(key));
}

/*
    Function to determine if an object is in the map, with an string used as the key.
*/
int contains(Map* map, char* key) {
    return any_contains(map, key, (strlen(key) + 1) * sizeof(char));
}

#endif

