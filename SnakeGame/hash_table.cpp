// The HashTable class file


/*
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

*/



// Include the Public Interface
#include "hash_table.h"

/****************************************************************************
 * Include other private dependencies
 *
 * These other modules are used in the implementation of the hash table module,
 * but are not required by users of the hash table.
 ***************************************************************************/
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

/****************************************************************************
 * Hidden Definitions
 *
 * These definitions are not available outside of this file. However, because
 * the are forward declared in hash_table.h, the type names are
 * available everywhere and user code can hold pointers to these structs.
 ***************************************************************************/
/**
 * This structure represents an the hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable
{
    /** The array of pointers to the head of a singly linked list, whose nodes
        are HashTableEntry objects */
    HashTableEntry **buckets;

    /** The hash function pointer */
    HashFunction hash;

    /** The number of buckets in the hash table */
    unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry
{
    /** The key for the hash table entry */
    unsigned int key;

    /** The value associated with this hash table entry */
    void *value;


    // A pointer pointing to the next hash table entry
    HashTableEntry *next;
};

/****************************************************************************
 * Private Functions
 *
 * These functions are not available outside of this file, since they are not
 * declared in hash_table.h.
 ***************************************************************************/
/**
 * createHashTableEntry
 *
 * Helper function that creates a hash table entry by allocating memory for it on
 * the heap. It initializes the entry with key and value, initialize pointer to
 * the next entry as NULL, and return the pointer to this hash table entry.
 *
 * @param key The key corresponds to the hash table entry
 * @param value The value stored in the hash table entry
 * @return The pointer to the hash table entry
 */
static HashTableEntry *createHashTableEntry(unsigned int key, void *value)
{
    HashTableEntry *pointer;
    pointer = (HashTableEntry*)malloc(sizeof(HashTableEntry)); // allocate space for entry
    pointer->key = key; // set key
    pointer->value = value; // set value
    pointer->next = NULL; // set pointer
    return pointer;
}

/**
 * findItem
 *
 * Helper function that checks whether there exists the hash table entry that
 * contains a specific key.
 *
 * @param hashTable The pointer to the hash table.
 * @param key The key corresponds to the hash table entry
 * @return The pointer to the hash table entry, or NULL if key does not exist
 */
static HashTableEntry *findItem(HashTable *hashTable, unsigned int key)
{
    HashTableEntry *curr = hashTable->buckets[hashTable->hash(key)]; // goes to first item in appropriate linked list
    while (curr) {
        if (curr->key == key) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/****************************************************************************
 * Public Interface Functions
 *
 * These functions implement the public interface as specified in the header
 * file, and make use of the private functions and hidden definitions in the
 * above sections.
 ****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable *createHashTable(HashFunction hashFunction, unsigned int numBuckets)
{
    // The hash table has to contain at least one bucket. Exit gracefully if
    // this condition is not met.
    if (numBuckets == 0)
    {
        printf("Hash table has to contain at least 1 bucket...\n");
        exit(1);
    }

    // Allocate memory for the new HashTable struct on heap.
    HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));

    // Initialize the components of the new HashTable struct.
    newTable->hash = hashFunction;
    newTable->num_buckets = numBuckets;
    newTable->buckets = (HashTableEntry **)malloc(numBuckets * sizeof(HashTableEntry *));

    // As the new buckets are empty, init each bucket as NULL.
    unsigned int i;
    for (i = 0; i < numBuckets; ++i)
    {
        newTable->buckets[i] = NULL;
    }

    // Return the new HashTable struct.
    return newTable;
}

// Loop through each bucket of the hash table to remove all items.
// set temp to be the first entry of the ith bucket
// delete all entries 

// Free buckets
    
// Free hash table
void destroyHashTable(HashTable *hashTable)
{
    for(int i = 0; i < hashTable->num_buckets; i++) {
        HashTableEntry *curr = hashTable->buckets[i]; // goes to first item in appropriate linked list
        while (curr) {
            HashTableEntry *ptr = curr->next; //removes the pointer
            free(curr->value);
            free(curr); // free's up the space needed
            curr = ptr; // moves to the next node
        }
    }
    free(hashTable->buckets);
    free(hashTable); // frees the hash table
}


// Check if the key is present in the hash table.
        
//If the key is present in the hash table, store new value and return old value

//If not, create entry for new value and return NULL
void *insertItem(HashTable *hashTable, unsigned int key, void *value)
{
    HashTableEntry *curr = hashTable->buckets[hashTable->hash(key)]; // goes to first item in appropriate linked list
    while (curr) {
        if (curr->key == key) { //checks for matching key
            void *temp = curr->value; // stores the value
            curr->value = value; // sets the new value
            return temp; // return old value
        }
        curr = curr->next; // move to next node
    }
    HashTableEntry *newNode = createHashTableEntry(key, value); // creates new entry
    newNode->next = hashTable->buckets[hashTable->hash(key)]; // attaches new entry to linked list
    hashTable->buckets[hashTable->hash(key)] = newNode; // reset head pointer
    return NULL; // return NULL
}


// Check if the key is present in the hash table.

// If the key exist, return the value
    
// If not. just return NULL
void *getItem(HashTable *hashTable, unsigned int key)
{
    HashTableEntry *x = findItem(hashTable,key); // call findItem function
    if (x == NULL) { // check if NULL value
        return NULL; //return NULL
    }
    return x->value; // return the node's value
}

//Get the bucket number and the head entry
    
// If the head holds the key, change the head to the next value, and return the old value
    
// If not the head, search for the key to be removed 
    
// If the key is not present in the list, return NULL
    
// Unlink node from the list and return old value
void *removeItem(HashTable *hashTable, unsigned int key)
{
    HashTableEntry *curr = hashTable->buckets[hashTable->hash(key)]; // goes to first item in appropriate linked list
    if (curr == NULL) { // check if NULL
        return NULL; // return NULL
    }
    if (curr->key == key) { //check if key is equal to key of head pointer
        hashTable->buckets[hashTable->hash(key)] = curr->next; // move head pointer forward
        void *temp = curr->value;
        free(curr);
        return temp; // return old value in head
    }
    while (curr->next) { // loop
        if(curr->next->key == key) { // check if next's key matches
            HashTableEntry *temp = curr->next->next; // retrieve next's next
            void *tempData = curr->next->value; //store data to return
            free(curr->next);
            curr->next = temp; // use pointer to skip and thus remove a node from linked list
            return tempData; //return data
        }
        curr = curr->next;
    }
    return NULL; // return NULL
}

// Remove the entry and free the returned data
void deleteItem(HashTable *hashTable, unsigned int key)
{
    HashTableEntry *curr = hashTable->buckets[hashTable->hash(key)]; // goes to first item in appropriate linked list
    if (curr != NULL) { // Make sure head is not null
        if (curr->key == key) { // check if key matches the head's key
            hashTable->buckets[hashTable->hash(key)] = curr->next; // moves the head pointer
            free(curr->value);
            free(curr); //free the head pointer
        } else {
            while (curr->next) { // check if curr has a non-NULL next
                if(curr->next->key == key) { // check if next's key matches given key
                    HashTableEntry *temp = curr->next->next; // retrieve next's next
                    free(curr->next->value);
                    free(curr->next); // Sets next free
                    curr->next = temp; //resets curr's next
                    return;
                }
                curr = curr->next; // moves to the next node
            }
        }
    }
}
