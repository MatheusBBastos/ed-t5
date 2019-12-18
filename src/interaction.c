#include "interaction.h"

#ifndef CURSES_ON

void navigation(PathStack pathStack, bool quickest);
void recalculate(GraphNode currentNode, PathStack *pathStack, GraphNode *currentTarget, bool quickest);

void startInteraction(Files *files, char *baseDir, char *entryFileName) {
    printf("-- MODO INTERAÇÃO --\n");
    char buffer[128];
    char command[16];
 
    do {
        printf("> ");
        fgets(buffer, 100, stdin);
        sscanf(buffer, "%15s", command);

        if (strcmp(command, "q") == 0) {
            char qryFileName[64];
            sscanf(buffer + 2, "%63[^\n]", qryFileName);

            if (!Files_OpenQueryFiles(files, baseDir, entryFileName, qryFileName))
                continue;
            
            processAndGenerateQuery(files, ALL, NULL);
        } else if (strcmp(command, "dmprbt") == 0) {
            char t = '\0', arq[64] = "";
            sscanf(buffer + 7, "%c %63[^\n]", &t, arq);

            if (t == '\0') {
                printf("Forneça um tipo!\n");
                continue;
            }

            if (arq[0] == '\0') {
                printf("Forneça um arquivo!\n");
                continue;
            }

            if (Query_Dmprbt(Files_GetOutputDir(files), t, arq))
                printf("Árvore escrita no arquivo %s!\n", arq);

        } else if (strcmp(command, "nav") == 0) {
            char t;
            sscanf(buffer + 4, "%c", &t);

            RBTree tree = NULL;

            switch (t) {
                case 'q':
                    tree = getBlockTree();
                    break;
                case 'h':
                    tree = getHydTree();
                    break;
                case 's':
                    tree = getTLightTree();
                    break;
                case 't':
                    tree = getCTowerTree();
                    break;
                case 'p':
                    tree = getBuildingTree();
                    break;
                case 'm':
                    tree = getWallTree();
                    break;
            }

            if (tree == NULL) {
                printf("Árvore inexistente: %c!\n", t);
                continue;
            }

            Node node = RBTree_GetRoot(tree);
            Node parentNode = NULL;

            char internalCommand;
            do {
                printf("Nó atual\n");
                if (node == NULL) {
                    printf("- Cor: PRETA\n- nil\n");
                } else {
                    printf("- Cor: %s\n", RBTreeN_GetColor(node) == RED ? "vermelho" : "preto");
                    Wall wall;
                    switch (t) {
                        case 'q':
                            printf("- Quadra %s\n", Block_GetCep(RBTreeN_GetValue(tree, node)));
                            break;
                        case 'h':
                            printf("- Hidrante %s\n", Equip_GetID(RBTreeN_GetValue(tree, node)));
                            break;
                        case 's':
                            printf("- Semáforo %s\n", Equip_GetID(RBTreeN_GetValue(tree, node)));
                            break;
                        case 't':
                            printf("- Rádio-base %s\n", Equip_GetID(RBTreeN_GetValue(tree, node)));
                            break;
                        case 'p':
                            printf("- Prédio %s\n", Building_GetKey(RBTreeN_GetValue(tree, node)));
                            break;
                        case 'm':
                            wall = RBTreeN_GetValue(tree, node);
                            printf("- Muro (%.2lf, %.2lf) - (%.2lf, %.2lf)\n",
                                    Wall_GetX1(wall), Wall_GetY1(wall),
                                    Wall_GetX2(wall), Wall_GetY2(wall));
                            break;
                    }
                }
                printf(">> ");
                scanf("%c%*c", &internalCommand);
                if (internalCommand == 'e') {
                    if (node == NULL) {
                        printf("Nó folha!\nAperte ENTER\n");
                        scanf("%*c");
                        continue;
                    }
                    parentNode = node;
                    node = RBTreeN_GetLeftChild(tree, node);
                } else if (internalCommand == 'd') {
                    if (node == NULL) {
                        printf("Nó folha!\nAperte ENTER\n");
                        scanf("%*c");
                        continue;
                    }
                    parentNode = node;
                    node = RBTreeN_GetRightChild(tree, node);
                } else if (internalCommand == 'p') {
                    if (node == NULL)
                        node = parentNode;
                    else {
                        Node parent = RBTreeN_GetParent(tree, node);
                        if (parent == NULL) {
                            printf("Nó raiz!\nAperte ENTER\n");
                            scanf("%*c");
                        } else {
                            node = parent;
                        }
                    }
                }
            } while (internalCommand != 'x');
        } else if (strcmp(command, "qr") == 0 || strcmp(command, "qc") == 0) {
            char qryFileName[64];
            sscanf(buffer + 3, "%63[^\n]", qryFileName);

            if (!Files_OpenQueryFiles(files, baseDir, entryFileName, qryFileName))
                continue;

            PathStack pathStack = NULL;
            
            processAndGenerateQuery(files, command[1] == 'r' ? QUICKEST : SHORTEST, &pathStack);
            printf("-- INÍCIO NAVEGAÇÃO --\n");
            navigation(pathStack, command[1] == 'r');
            printf("-- FIM NAVEGAÇÃO --\n");
            scanf("%*c");
        } else if (strcmp(command, "sai") != 0) {
            printf("Comando não reconhecido!\n");
        }
    } while (strcmp(command, "sai") != 0);
}

void navigation(PathStack pathStack, bool quickest) {
    if (pathStack == NULL) {
        printf("Caminho não encontrado!\n");
    } else {
        GraphNode previousTarget = peekPathStack(pathStack);
        GraphNode end = getPathStackBase(pathStack);

        while (previousTarget != NULL) {
            pathStack = popPathStack(pathStack);
            if (pathStack != NULL) {
                GraphNode currentTarget = peekPathStack(pathStack);
                GraphNode currentNode = previousTarget;

                printf("Siga na direção ");
                double xt = GraphNode_GetX(currentTarget), yt = GraphNode_GetY(currentTarget);
                double dx = xt - GraphNode_GetX(previousTarget);
                double dy = yt - GraphNode_GetY(previousTarget);
                if (dx > 0) {
                    printf("Leste.\n");
                } else if (dx < 0) {
                    printf("Oeste.\n");
                } else if (dy > 0) {
                    printf("Norte.\n");
                } else {
                    printf("Sul.\n");
                }

                while (currentNode != currentTarget) {
                    printf(">> ");
                    char internalCommand[3];
                    scanf("%2s", internalCommand);
                    if (strcmp(internalCommand, "n") == 0
                            || strcmp(internalCommand, "s") == 0
                            || strcmp(internalCommand, "l") == 0
                            || strcmp(internalCommand, "o") == 0) {
                        GraphNode newNode = GraphNode_GoTo(currentNode, internalCommand[0]);
                        if (newNode == NULL) {
                            printf("Direção não é possivel!\n");
                        } else {
                            currentNode = newNode;
                            double xc = GraphNode_GetX(currentNode);
                            double yc = GraphNode_GetY(currentNode);
                            if (euclideanDistance(xt, yt, xc, yc) >= 200) {
                                printf("Se afastou mais de 200 unidades do alvo, recalculando...\n");
                                recalculate(currentNode, &pathStack, &currentTarget, quickest);
                                if (pathStack == NULL) {
                                    printf("Impossível recalcular o caminho!\n");
                                    return;
                                }
                            }
                        }
                    } else if (strcmp(internalCommand, "rr") == 0 
                               || strcmp(internalCommand, "rc") == 0) {
                        printf("Recalculando...\n");
                        quickest = internalCommand[1] == 'r';
                        recalculate(currentNode, &pathStack, &currentTarget, quickest);
                        if (pathStack == NULL) {
                            printf("Impossível recalcular o caminho!\n");
                            return;
                        }
                    } else if (strcmp(internalCommand, "x") == 0) {
                        destroyPathStack(pathStack);
                        printf("Saiu da navegação.\n");
                        return;
                    } else {
                        printf("Comando não reconhecido!\n");
                    }
                }
                previousTarget = currentTarget;
            } else {
                printf("Você chegou ao seu destino.\n");
                return;
            }
        }
    }
}

void recalculate(GraphNode currentNode, PathStack *pathStack, GraphNode *currentTarget, bool quickest) {
    GraphNode end = getPathStackBase(*pathStack);
    destroyPathStack(*pathStack);
    *pathStack = findPathStack(currentNode, end, quickest);
    *currentTarget = currentNode;
}

#else
void startInteraction(Files *files, char *baseDir, char *entryFileName) {
    startGui(files, baseDir, entryFileName);
}

#endif
