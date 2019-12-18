#include "hash_table.h"

static inline unsigned long long int _hashFunc(char *key);

typedef struct node_t {
    char *key;
    void *value;
    struct node_t *next;
} *Node;

typedef struct hashtable_t {
    int size;
    Node *nodes;
} *HashTablePtr;

HashTable HashTable_Create(int size) {
    HashTablePtr table = malloc(sizeof(struct hashtable_t));
    table->nodes = malloc(size * sizeof(Node));
    memset(table->nodes, 0, size * sizeof(Node));
    table->size = size;
    return table;
}

void *HashTable_Insert(HashTable tableVoid, char *key, void *value) {
    HashTablePtr table = (HashTablePtr) tableVoid;
    Node node = malloc(sizeof(struct node_t));
    node->next = NULL;
    node->key = key;
    node->value = value;
    int index = _hashFunc(key) % table->size;
    if (table->nodes[index] == NULL) {
        table->nodes[index] = node;
    } else {
        Node pastNode = table->nodes[index];
        Node currentNode = pastNode;
        while (currentNode != NULL) {
            if (strcmp(currentNode->key, key) == 0) {
                void *oldVal = currentNode->value;
                currentNode->value = value;
                free(node);
                return oldVal;
            }
            pastNode = currentNode;
            currentNode = currentNode->next;
        }
        pastNode->next = node;
    }
    return NULL;
}

void *HashTable_Remove(HashTable tableVoid, char *key) {
    HashTablePtr table = (HashTablePtr) tableVoid;
    int index = _hashFunc(key) % table->size;
    Node pastNode = NULL;
    Node currentNode = table->nodes[index];
    while (currentNode != NULL && strcmp(currentNode->key, key) != 0) {
        pastNode = currentNode;
        currentNode = currentNode->next;
    }

    if (currentNode == NULL)
        return NULL;

    void *returnVal = currentNode->value;
    if (pastNode != NULL) {
        pastNode->next = currentNode->next;
    } else {
        table->nodes[index] = currentNode->next;
    }
    free(currentNode);

    return returnVal;
}

void *HashTable_Find(HashTable tableVoid, char *key) {
    HashTablePtr table = (HashTablePtr) tableVoid;
    int index = _hashFunc(key) % table->size;

    Node currentNode = table->nodes[index];
    while (currentNode != NULL && strcmp(currentNode->key, key) != 0) {
        currentNode = currentNode->next;
    }

    if (currentNode == NULL)
        return NULL;
    
    return currentNode->value;
}

void HashTable_Destroy(HashTable tableVoid, void (*destroy)(void *)) {
    HashTablePtr table = (HashTablePtr) tableVoid;
    for (int i = 0; i < table->size; i++) {
        Node currentNode = table->nodes[i];
        while (currentNode != NULL) {
            if (destroy != NULL)
                destroy(currentNode->value);
            Node nextNode = currentNode->next;
            free(currentNode);
            currentNode = nextNode;
        }
    }
    free(table->nodes);
    free(table);
}

static unsigned long long _hashFunc(char *key) {
    unsigned long long int hash = 197;
    int len = strlen(key);
    for (int i = 0; i < len; i++) {
        hash = hash * 31 + key[i];
    }
    return hash;
}
