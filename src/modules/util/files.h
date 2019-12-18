#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "file_util.h"

typedef void *Files;

Files Files_Create();

bool Files_OpenQueryFiles(Files *files, char *baseDir, char *entryFileName, char *queryFileName);

FILE *Files_GetEntryFile(Files files);

FILE *Files_GetOutputSVGFile(Files files);

FILE *Files_GetOutputQryFile(Files files);

FILE *Files_GetQueryFile(Files files);

FILE *Files_GetTxtFile(Files files);

FILE *Files_GetEcFile(Files files);

FILE *Files_GetPmFile(Files files);

FILE *Files_GetViaFile(Files files);

char *Files_GetBaseDir(Files files);

char *Files_GetOutputDir(Files files);

char *Files_GetQrySVGFileName(Files files);

void Files_SetEntryFile(Files files, FILE *entryFile);

void Files_SetOutputSVGFile(Files files, FILE *outputSVGFile);

void Files_SetOutputQryFile(Files files, FILE *outputQryFile);

void Files_SetQueryFile(Files files, FILE *queryFile);

void Files_SetTxtFile(Files files, FILE *txtFile);

void Files_SetEcFile(Files files, FILE *ecFile);

void Files_SetPmFile(Files files, FILE *pmFile);

void Files_SetViaFile(Files files, FILE *viaFile);

void Files_SetOutputDir(Files files, char *outputDir);

void Files_SetQrySVGFileName(Files files, char *svgFileName);

void Files_Destroy(Files files);

#endif
