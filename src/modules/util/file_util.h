#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Abre o arquivo no diretório especificado
FILE *openFile(char *baseDir, char *path, char *flags);

// Remove a extensão (o que vier depois do ponto, incluindo o ponto) de uma string caminho
void removeExtension(char *filePath);

// Muda a extensão da string para uma nova informada
void changeExtension(char *filePath, char *newExtension);

// Adiciona um sufixo precedido por um hífen no nome do arquivo
void addSuffix(char *filePath, char *suffix);

// Remove o diretório e a extensão da string, deixando apenas o nome do arquivo
void removeDirAndExt(char *filePath);

#endif