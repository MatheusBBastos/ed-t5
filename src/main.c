#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "modules/util/file_util.h"
#include "commands.h"
#include "interaction.h"
#include "modules/sig/object.h"
#include "modules/util/files.h"

int main(int argc, char *argv[]) {
	Files files = Files_Create();

	bool interactive = false;

	char *baseDir = NULL;
	char *entryFileName = NULL;
	char *queryFileName = NULL;
	char *outputDir = NULL;
	char *outputSVGFileName = NULL;
	char *ecFileName = NULL;
	char *pmFileName = NULL;
	char *viaFileName = NULL;

	FILE *entryFile = NULL;
	FILE *outputSVGFile = NULL;
	FILE *ecFile = NULL;
	FILE *pmFile = NULL;
	FILE *viaFile = NULL;

	// Processamento dos argumentos passados ao programa
	for (int i = 1; i < argc; i++) {
		if (strcmp("-e", argv[i]) == 0) {
			if (++i >= argc) {
				printf("O argumento '-e' requer um diretório!\n");
				return 1;
			}
			if (baseDir != NULL) {
				free(baseDir);
			}
			baseDir = malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(baseDir, argv[i]);
		} else if (strcmp("-f", argv[i]) == 0) {
			if (++i >= argc) {
				printf("O argumento '-f' requer o nome de um arquivo!\n");
				return 1;
			}
			if (entryFileName != NULL) {
				free(entryFileName);
			}
			entryFileName = malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(entryFileName, argv[i]);
		} else if (strcmp("-q", argv[i]) == 0) {
			if (++i >= argc) {
				printf("O argumento '-q' requer o nome de um arquivo!\n");
				return 1;
			}
			if (queryFileName != NULL) {
				free(queryFileName);
			}
			queryFileName = malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(queryFileName, argv[i]);
		} else if (strcmp("-o", argv[i]) == 0) {
			if (++i >= argc) {
				printf("O argumento '-o' requer um diretório!\n");
				return 1;
			}
			if (outputDir != NULL) {
				free(outputDir);
			}
			outputDir = malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(outputDir, argv[i]);
		} else if (strcmp("-ec", argv[i]) == 0) {
			if (++i >= argc) {
				printf("O argumento '-ec' requer o nome de um arquivo!\n");
				return 1;
			}
			if (ecFileName != NULL) {
				free(ecFileName);
			}
			ecFileName = malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(ecFileName, argv[i]);
		} else if (strcmp("-pm", argv[i]) == 0) {
			if (++i >= argc) {
				printf("O argumento '-pm' requer o nome de um arquivo!\n");
				return 1;
			}
			if (pmFileName != NULL) {
				free(pmFileName);
			}
			pmFileName = malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(pmFileName, argv[i]);
		} else if (strcmp("-v", argv[i]) == 0) {
			if (++i >= argc) {
				printf("O argumento '-v' requer o nome de um arquivo!\n");
				return 1;
			}
			if (viaFileName != NULL) {
				free(viaFileName);
			}
			viaFileName = malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(viaFileName, argv[i]);
		} else if (strcmp("-i", argv[i]) == 0) {
			interactive = true;
		} else {
			printf("Comando não reconhecido: '%s'\n", argv[i]);
			return 1;
		}
	}

	// Verificação se os argumentos obrigatórios foram passados
	if (entryFileName == NULL) {
		printf("O argumento '-f' é obrigatório!\n");
		return 1;
	}
	if (outputDir == NULL) {
		printf("O argumento '-o' é obrigatório!\n");
		return 1;
	}
	Files_SetOutputDir(files, outputDir);

	outputSVGFileName = malloc((strlen(entryFileName) + 4) * sizeof(char));
	strcpy(outputSVGFileName, entryFileName);
	changeExtension(outputSVGFileName, "svg");

	// Abertura do arquivo de entrada padrão
	entryFile = openFile(baseDir, entryFileName, "r");
	if (entryFile == NULL) {
		return 1;
	}
	Files_SetEntryFile(files, entryFile);

	// Abertura dos arquivos referentes à consulta
	if (queryFileName != NULL) {
		if (!Files_OpenQueryFiles(files, baseDir, entryFileName, queryFileName))
			return 1;
	}

	// Abertura do arquivo de saída padrão
	outputSVGFile = openFile(outputDir, outputSVGFileName, "w");
	if (outputSVGFile == NULL) {
		return 1;
	}
	Files_SetOutputSVGFile(files, outputSVGFile);

	if (ecFileName != NULL) {
		ecFile = openFile(baseDir, ecFileName, "r");
		if (ecFile == NULL)
			return 1;
		Files_SetEcFile(files, ecFile);
	}

	if (pmFileName != NULL) {
		pmFile = openFile(baseDir, pmFileName, "r");
		if (pmFile == NULL)
			return 1;
		Files_SetPmFile(files, pmFile);
	}

	if (viaFileName != NULL) {
		viaFile = openFile(baseDir, viaFileName, "r");
		if (viaFile == NULL)
			return 1;
		Files_SetViaFile(files, viaFile);	
	}

	initializeTrees();
    initializeTables();
	
	processAll(files);

	if (interactive) {
		startInteraction(files, baseDir, entryFileName);
	}

	destroyTables();
    destroyTrees();

	// Limpeza
	fclose(entryFile);
	fclose(outputSVGFile);
	if (queryFileName != NULL) {
		free(queryFileName);
	}
	if (baseDir != NULL)
		free(baseDir);
	free(outputSVGFileName);
	free(entryFileName);
	if (outputDir != NULL)
		free(outputDir);
	if (ecFile != NULL) {
		free(ecFileName);
		fclose(ecFile);
	}
	if (pmFile != NULL) {
		free(pmFileName);
		fclose(pmFile);
	}
	if (viaFile != NULL) {
		free(viaFileName);
		fclose(viaFile);
	}

	Files_Destroy(files);
}