#include "files.h"

typedef struct files_t {
    FILE *entryFile;
    FILE *outputSVGFile;
    FILE *outputQryFile;
    FILE *queryFile;
    FILE *txtFile;
    FILE *ecFile;
    FILE *pmFile;
    FILE *viaFile;
    char baseDir[64];
    char *outputDir;
    char qrySVGFileName[64];
} *FilesPtr;

Files Files_Create() {
    FilesPtr files = malloc(sizeof(struct files_t));
    files->entryFile = NULL;
    files->outputSVGFile = NULL;
    files->outputQryFile = NULL;
    files->queryFile = NULL;
    files->txtFile = NULL;
    files->ecFile = NULL;
    files->pmFile = NULL;
    files->viaFile = NULL;
    files->outputDir = NULL;
    files->qrySVGFileName[0] = '\0';
    return files;
}

bool Files_OpenQueryFiles(Files *filesVoid, char *baseDir, char *entryFileName, char *queryFileName) {
    FilesPtr files = (FilesPtr) filesVoid;

    strcpy(files->baseDir, baseDir);

    char outputTXTFileName[64];
	char outputQrySVGFileName[64];
    files->queryFile = openFile(baseDir, queryFileName, "r");
    if (files->queryFile == NULL)
        return false;
    
    char noExtName[32];
    // Copiar nome completo do arquivo de consulta
    strcpy(noExtName, queryFileName);
    // Remover diretório e extensão
    removeDirAndExt(noExtName);

    // Copiar nome do arquivo de entrada
    strcpy(outputTXTFileName, entryFileName);
    // Adicionar sufixo (que é o nome do arquivo de consulta)
    addSuffix(outputTXTFileName, noExtName);
    // Copiar o resultado para o arquivo SVG de consulta
    strcpy(outputQrySVGFileName, outputTXTFileName);

    // Atribuir as extensões correspondentes
    changeExtension(outputTXTFileName, "txt");
    changeExtension(outputQrySVGFileName, "svg");

    files->txtFile = openFile(files->outputDir, outputTXTFileName, "w");
    if (files->txtFile == NULL)
        return false;

    files->outputQryFile = openFile(files->outputDir, outputQrySVGFileName, "w");
    if (files->outputQryFile == NULL)
        return false;

    strcpy(files->qrySVGFileName, outputQrySVGFileName);

    return true;
}

FILE *Files_GetEntryFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->entryFile;
}

FILE *Files_GetOutputSVGFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->outputSVGFile;
}

FILE *Files_GetOutputQryFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->outputQryFile;
}

FILE *Files_GetQueryFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->queryFile;
}

FILE *Files_GetTxtFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->txtFile;
}

FILE *Files_GetEcFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->ecFile;
}

FILE *Files_GetPmFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->pmFile;
}

FILE *Files_GetViaFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->viaFile;
}

char *Files_GetBaseDir(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->baseDir;
}

char *Files_GetOutputDir(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->outputDir;
}

char *Files_GetQrySVGFileName(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->qrySVGFileName;
}

void Files_SetEntryFile(Files filesVoid, FILE *entryFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->entryFile = entryFile;
}

void Files_SetOutputSVGFile(Files filesVoid, FILE *outputSVGFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->outputSVGFile = outputSVGFile;
}

void Files_SetOutputQryFile(Files filesVoid, FILE *outputQryFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->outputQryFile = outputQryFile;
}

void Files_SetQueryFile(Files filesVoid, FILE *queryFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->queryFile = queryFile;
}

void Files_SetTxtFile(Files filesVoid, FILE *txtFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->txtFile = txtFile;
}

void Files_SetEcFile(Files filesVoid, FILE *ecFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->ecFile = ecFile;
}

void Files_SetPmFile(Files filesVoid, FILE *pmFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->pmFile = pmFile;
}

void Files_SetViaFile(Files filesVoid, FILE *viaFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->viaFile = viaFile;
}

void Files_SetOutputDir(Files filesVoid, char *outputDir) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->outputDir = outputDir;
}

void Files_SetQrySVGFileName(Files filesVoid, char *svgFileName) {
    FilesPtr files = (FilesPtr) filesVoid;
    strcpy(files->qrySVGFileName, svgFileName);
}

void Files_Destroy(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    free(files);
}
