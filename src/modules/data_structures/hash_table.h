#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef void *HashTable;

// Cria a Tabela de Espalhamento com o tamanho dado (de preferência número primo)
HashTable HashTable_Create(int size);

// Insere um par chave-valor na tabela
void *HashTable_Insert(HashTable table, char *key, void *value);

// Retorna o valor correspondente à chave (NULL se não existir)
void *HashTable_Find(HashTable table, char *key);

// Remove o par chave-valor e retorna o valor removido (NULL se não existir)
void *HashTable_Remove(HashTable table, char *key);

// Destrói a tabela, executando destroy em todos os valores
void HashTable_Destroy(HashTable table, void (*destroy)(void *));

#endif