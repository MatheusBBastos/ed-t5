#include "file_util.h"

FILE *openFile(char *baseDir, char *path, char *flags) {
    char *fullPath;
    int len = strlen(path) + 2;
    if (baseDir != NULL)
        len += strlen(baseDir);
    fullPath = malloc(len * sizeof(char));
    if (baseDir != NULL) {
        if (baseDir[strlen(baseDir) - 1] == '/') {
            sprintf(fullPath, "%s%s", baseDir, path);
        } else {
            sprintf(fullPath, "%s/%s", baseDir, path);
        }
    } else {
        strcpy(fullPath, path);
    }
    FILE *newFile = fopen(fullPath, flags);
    if (newFile == NULL) {
        char stringErro[128];
        sprintf(stringErro, "Erro ao abrir arquivo '%s'", fullPath);
        perror(stringErro);
    }
    free(fullPath);
    return newFile;
}

void removeExtension(char *filePath) {
    int index = strlen(filePath) - 1;

    while (index >= 0 && filePath[index] != '.')
        index--;
    
    if (index >= 0)
        filePath[index] = '\0';
}

void changeExtension(char *filePath, char *newExtension) {
    removeExtension(filePath);
    strcat(filePath, ".");
    strcat(filePath, newExtension);
}

void addSuffix(char *filePath, char *suffix) {
    int index = strlen(filePath) - 1;

    while (index >= 0 && filePath[index] != '.')
        index--;
    
    char extension[16];
    strcpy(extension, filePath + index);
    char appendix[64];
    sprintf(appendix, "-%s%s", suffix, extension);
    filePath[index] = '\0';
    strcat(filePath, appendix);
}

void removeDirAndExt(char *filePath) {
    removeExtension(filePath);
    int index = strlen(filePath) - 1;

    while (index >= 0 && filePath[index] != '/')
        index--;

    if (index >= 0) {
        char tempFilePath[128];
        strcpy(tempFilePath, filePath + index + 1);
        strcpy(filePath, tempFilePath);
    }
}
