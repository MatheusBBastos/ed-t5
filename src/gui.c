#include "gui.h"

#ifdef CURSES_ON

typedef struct Ponto {
    int x, y;
} Ponto;

typedef enum TreeType {
    QUADRA, HIDRANTE, SEMAFORO, RADIOBASE, PREDIO, MURO
} TreeType;

int digitNumber(double d) {
    int no = d;
    int res = 0;
    while (no!=0) {
        no = no/10;
        res++;
    }
    return res;
}

void cleanDelete(WINDOW *output) {
    werase(output);
    wrefresh(output);
    delwin(output);
}

char *getKeyOf(RBTree tree, Node node, TreeType type) {
    Point p = RBTreeN_GetKey(tree, node);
    char *pp = calloc(128, sizeof(char));
    double x = Point_GetX(p), y = Point_GetY(p);
    if ((digitNumber(x) + digitNumber(y)) % 2 == 0)
        sprintf(pp, "(%.0lf   %.0lf)", x, y);
    else
        sprintf(pp, "(%.0lf  %.0lf)", x, y);
    return pp;
}

char *getFullKeyOf(RBTree tree, Node node, TreeType type) {

    Point p = RBTreeN_GetKey(tree, node);
    char *pp = calloc(128, sizeof(char));
    double x = Point_GetX(p), y = Point_GetY(p);
    sprintf(pp, "(%.10lf,  %.10lf)", x, y);
    return pp;
}

char *getValueOf(RBTree tree, Node node, TreeType type) {

    char *s = calloc(128, sizeof(char));

    Wall wall;
    switch (type) {
        case QUADRA:
            sprintf(s, "%s",Block_GetCep(RBTreeN_GetValue(tree, node)));
            break;
        case HIDRANTE:
            sprintf(s, "%s",Equip_GetID(RBTreeN_GetValue(tree, node)));
            break;
        case SEMAFORO:
            sprintf(s, "%s",Equip_GetID(RBTreeN_GetValue(tree, node)));
            break;
        case RADIOBASE:
            sprintf(s, "%s",Equip_GetID(RBTreeN_GetValue(tree, node)));
            break;
        case PREDIO:
            sprintf(s, "%s",Building_GetKey(RBTreeN_GetValue(tree, node)));
            break;
        case MURO:
            wall = RBTreeN_GetValue(tree, node);
            sprintf(s, "(%.3lf, %.3lf) (%.3lf, %.3lf)",
                    Wall_GetX1(wall), Wall_GetY1(wall),
                    Wall_GetX2(wall), Wall_GetY2(wall));
            break;
        default:
            sprintf(s, "%s", "Unhandled Type");
            break;
    }

    return s;
}

void doneScreen(bool success, char *msg) {
    WINDOW *done;
    if (msg == NULL) {
        done = newwin(3, 60, 24/2 - 3/2, 79/2 - 59/2);
    } else{
        done = newwin(4, 60, 24/2 - 4/2, 79/2 - 59/2);
    }

    if (!success) {
        init_pair(67, COLOR_RED, -1);
        wattron(done, COLOR_PAIR(67));
        box(done, 0,0);
        wattroff(done, COLOR_PAIR(67));
    } else{
        box(done, 0,0);
    }

    wrefresh(done);
    char *s = "Concluído!";
    char *e = "Erro!";
    if (success) {
        mvwprintw(done, 1, 60/2 - strlen(s)/2, "%s", s);
    } else{
        mvwprintw(done, 1, 60/2 - strlen(e)/2, "%s", e);
    }

    if (msg != NULL)
        mvwprintw(done, 2, 60/2 - strlen(msg)/2, "%s", msg);

    wrefresh(done);    
    sleep(3);
    cleanDelete(done);
    refresh();
}

TreeType menuTipo() {
    WINDOW *typew = newwin(10, 20, 24/2 - 10/2, 79/2 - 20/2);

    box(typew, 0,0);
    wrefresh(typew);

    wattron(typew, A_BOLD);
    mvwprintw(typew, 1, 1, " Selecione o tipo ");
    wattroff(typew, A_BOLD);

    int selected = 0;
    char *types[6] = {"Quadras", "Hidrantes", "Semaforos", "Radio-Base", "Predio", "Muro"};
    int ch = 0;

    noecho();

    do {
        for (int i = 0; i < 6; i++) {
            if (selected == i)
                wattron(typew, A_STANDOUT);
            mvwprintw(typew,i+3, 10 - strlen(types[i])/2, "%s", types[i]);
            if (selected == i)
                wattroff(typew, A_STANDOUT);
        }
        wrefresh(typew);
        ch = getch();
        if (ch == 10)
            break;

        if (ch == '\033') {
            if (getch() != '[')
                continue;

            ch = getch();
            if (ch == 'A') {
                if (selected-1 >= 0)
                    selected--;
            } else if (ch == 'B') {
                if (selected+1 <= 5)
                    selected++;
            }
        }

    }while (ch != 10);

    cleanDelete(typew);
    refresh();

    return selected;
}

char *waitCommand() {
    curs_set(1);
    echo();
    WINDOW *output = newwin(3,79, 5, 1);
    refresh();
    box(output, 0, 0);
    wrefresh(output);
        mvwprintw(output, 0, 1, "Insira o comando de entrada");
        wrefresh(output);

    WINDOW *help = newwin(15, 79, 8, 1);
    wrefresh(help);

    wattron(help, A_ITALIC);
    mvwprintw(help, 1,1, "Lista de comandos: ");
    wattroff(help, A_ITALIC);
    mvwprintw(help, 1,22, "q <arquivo de entrada.qry> ");
    mvwprintw(help, 2,22, "dmprbt <tipo> <arquivo de destino> ");
    mvwprintw(help, 3,22 + 7, "tipos: q (quadras)");
    mvwprintw(help, 4,22 + 7 + 7, "h (hidrantes)");
    mvwprintw(help, 5,22 + 7 + 7, "s (semaforos)");
    mvwprintw(help, 6,22 + 7 + 7, "t (torres de radio)");
    mvwprintw(help, 7,22 + 7 + 7, "p (prédios)");
    mvwprintw(help, 8,22 + 7 + 7, "m (muros)");
    mvwprintw(help, 9,22, "nav [entrada interativa]");
    mvwprintw(help, 10,22, "sai [finaliza a execução]");
    wrefresh(help);

    move(6,2);
    char *str = calloc(120, sizeof(char));
    getstr(str);
    
    cleanDelete(output);
    curs_set(0);
    refresh();
    return str;
}

int mvwprintw_center(WINDOW *w, int linha, char *str) {
    mvwprintw(w, linha, 80/2 - strlen(str)/2, "%s", str);    
}

int mvwprintw_center_custom(WINDOW *w, int linha, int custom, char *str) {
    mvwprintw(w, linha, 80/2 - strlen(str)/2 - custom, "%s", str);    
}

void detailPopup(RBTree tree, Node node, TreeType type) {
    WINDOW *done = newwin(6, 59, 11,11);
    box(done, 0,0);
    wrefresh(done);

    char *keyAtual = getKeyOf(tree, node, type);
    wattron(done, A_BOLD);
    mvwprintw(done, 1, 59/2 - strlen(keyAtual)/2, "%s", keyAtual);

    free(keyAtual);

    init_pair(25, COLOR_BLACK, -1);
    init_pair(26, COLOR_RED, -1);

    if (RBTreeN_GetColor(node) == RED) {
        wattron(done, COLOR_PAIR(26));
        mvwprintw(done, 2, 59/2 - 14/2, "Node Vermelho");
        wattroff(done, COLOR_PAIR(26));
    } else{
        wattron(done, A_DIM);
        mvwprintw(done, 2, 59/2 - 11/2, "Node Preto");
        wattroff(done, A_DIM);
    }

    char *ky = getFullKeyOf(tree, node, type);
    int kC_meio = 59/2 - (strlen(ky)+15)/2;
    mvwprintw(done, 3, kC_meio, "Key completa: ");
    wattroff(done, A_BOLD);
    mvwprintw(done, 3, kC_meio + 15, "%s", ky);

    free(ky);

    wattron(done, A_BOLD);
    char *valueOf = getValueOf(tree, node, type);
    int kC_down = 59/2 - (strlen(valueOf)+8)/2;
    mvwprintw(done, 4, kC_down, "Valor: ");
    wattroff(done, A_BOLD);
    mvwprintw(done, 4, kC_down + 8, "%s", valueOf);

    free(valueOf);

    wrefresh(done);    
    getch();
    cleanDelete(done);
    refresh();
}

void cleanTree(WINDOW *output) {
    //19,79, 5, 1)
    for (int y = 3; y < 18; y++) {
        for (int x = 1; x < 78; x++) {
            mvwaddch(output, y, x, ' ');
        }
    }

    wrefresh(output);
}

void renderTreeLine(WINDOW *output, bool *isActive) {

    if (isActive[0]) {
        if (isActive[1]) {
            for (int i = 0; i < 15; i++) {
                mvwaddch(output, 5, i + 44, ACS_HLINE);
            }

            mvwaddch(output, 5,59, ACS_URCORNER);

            for (int i = 0; i < 1; i++) {
                mvwaddch(output, i + 6, 59, ACS_VLINE);
            }

            if (isActive[6] || isActive[5]) {
                for (int i = 0; i < 1; i++) {
                    mvwaddch(output, i + 10, 59, ACS_VLINE);
                }

                if (isActive[6]) {
                    for (int i = 0; i < 9; i++) {
                        mvwaddch(output, 11, i + 50, ACS_HLINE);
                    }
                    mvwaddch(output, 11, 49, ACS_ULCORNER);
                    mvwaddch(output, 11, 59, ACS_LRCORNER);

                }

                if (isActive[5]) {
                    for (int i = 0; i < 9; i++) {
                        mvwaddch(output, 11, i + 60, ACS_HLINE);
                    }
                    mvwaddch(output, 11, 69, ACS_URCORNER);
                    mvwaddch(output, 11, 59, ACS_LLCORNER);
                }

                if (isActive[5] && isActive[6])
                    mvwaddch(output, 11, 59, ACS_BTEE);

            }
        }
    }
        
    if (isActive[2]) {
        for (int i = 0; i < 15; i++) {
            mvwaddch(output, 5, i + 19, ACS_HLINE);
        }

        mvwaddch(output, 5, 19, ACS_ULCORNER);

        for (int i = 0; i < 1; i++) {
            mvwaddch(output, i + 6, 19, ACS_VLINE);
        }

        if (isActive[4] || isActive[3]) {
            for (int i = 0; i < 1; i++) {
                mvwaddch(output, i + 10, 19, ACS_VLINE);
            }

            if (isActive[4]) {
                for (int i = 0; i < 9; i++) {
                    mvwaddch(output, 11, i + 10, ACS_HLINE);
                }

                mvwaddch(output, 11, 9, ACS_ULCORNER);
                mvwaddch(output, 11, 19, ACS_LRCORNER);
            }

            if (isActive[3]) {
                for (int i = 0; i < 9; i++) {
                    mvwaddch(output, 11, i + 20, ACS_HLINE);
                }
                
                mvwaddch(output, 11, 29, ACS_URCORNER);
                mvwaddch(output, 11, 19, ACS_LLCORNER);
            }

            if (isActive[4] && isActive[3])
                mvwaddch(output, 11, 19, ACS_BTEE);

        }

    }
    

    wrefresh(output);
}

void setNodeData(RBTree arvore, Node node, bool *isActive, bool *isRed, char** chaves, Node *nodeList, int index, TreeType type) {
    isActive[index] = true;
/*     if (chaves[index] != NULL)
        free(chaves[index]); */
    chaves[index] = getKeyOf(arvore, node, type);
    isRed[index] = RBTreeN_GetColor(node) == RED;
    nodeList[index] = node;
}

void renderTree(Ponto *coord, WINDOW** nodes, char** chaves, bool *isRed, bool *isActive, int selected) {
    for (int i = 0; i < 7; i++) {
        if (!isActive[i]) {
            werase(nodes[i]);
            wrefresh(nodes[i]);
            continue;
        }

        if (!isRed[i]) {
            wattron(nodes[i], A_DIM);
        } else{
            wattron(nodes[i], COLOR_PAIR(9));
        }

        if (i == selected)
            wattron(nodes[i], A_STANDOUT);

        box(nodes[i], 0, 0);
        wrefresh(nodes[i]);
        if (!isRed[i]) {
            wattroff(nodes[i],  A_DIM);
        } else{
            wattroff(nodes[i], COLOR_PAIR(9));
        }

        char *str = chaves[i];
         //str = calloc(10, sizeof(char));
        //sprintf(str, "%d", i); 
        int strl = strlen(str);
        if (strl < 14) {
            int x = (14-strl);
            x = (x % 2 == 0 ? x/2 : x/2 + 1);

            mvwprintw(nodes[i], 1, 1, "%s", "             ");
            //Completa com espaço em branco 
            //depois ele da repaint com o texto por cima
            
            mvwprintw(nodes[i], 1, x, "%s", str);
        } else{
            mvwprintw(nodes[i], 1, 1, "%.12s..", str);
        }
        wrefresh(nodes[i]);

        if (i == selected)
            wattroff(nodes[i], A_STANDOUT);

    }
}

int processInputMovement(int atual, int read, bool *isActive) {
    int ret = atual;
    if (read == 'A') { //Up
        if (atual == 0)
            ret = -1;
        if (atual == 1 || atual == 2)
            ret = 0;
        if (atual == 4 || atual == 3)
            ret = 2;
        if (atual == 6 || atual == 5)
            ret = 1;
    } else if (read == 'B') { //Down
        if (atual == 5 || atual == 6 || atual == 4 || atual == 3) {
            ret = -1;
        }
    } else if (read == 'C') { //Right
        if (atual == 0)
            ret = 1;
        if (atual == 1)
            ret = 5;
        if (atual == 2)
            ret = 3;
        if (atual == 4)
            ret = 3;
        if (atual == 6)
            ret = 5;
    } else if (read == 'D') { //Left
        if (atual == 0)
            ret = 2;
        if (atual == 1)
            ret = 6;
        if (atual == 2)
            ret = 4;
        if (atual == 3)
            ret = 4;
        if (atual == 5)
            ret = 6;
    }

    if (isActive[ret] || ret == -1)
        return ret;
    else
        return atual;

}

void loadTreeData(RBTree tree, Node root, bool isActive[7], bool isRed[7], char *chaves[7], Node nodeList[7], TreeType type) {

    for (int i = 0; i < 7; i++) {
        isActive[i] = false;
        isRed[i] = false;
        chaves[i] = NULL;
    }

    if (root == NULL) {
        isActive[0] = true;
        chaves[0] = NULL;
    } else{
        setNodeData(tree, root, isActive, isRed, chaves, nodeList, 0, type);

        Node left = RBTreeN_GetLeftChild(tree, root);
        Node right = RBTreeN_GetRightChild(tree, root);

        if (right != NULL) {
            setNodeData(tree, right, isActive, isRed, chaves, nodeList, 1, type);

            Node sonLeft = RBTreeN_GetLeftChild(tree, right);
            Node sonRight = RBTreeN_GetRightChild(tree, right);

            if (sonLeft != NULL)
                setNodeData(tree, sonLeft, isActive, isRed, chaves, nodeList, 6, type);

            if (sonRight != NULL)
                setNodeData(tree, sonRight, isActive, isRed, chaves, nodeList, 5, type);
        }

        if (left != NULL) {
            setNodeData(tree, left, isActive, isRed, chaves, nodeList, 2, type);

            Node sonLeft = RBTreeN_GetLeftChild(tree, left);
            Node sonRight = RBTreeN_GetRightChild(tree, left);

            if (sonLeft != NULL)
                setNodeData(tree, sonLeft, isActive, isRed, chaves, nodeList, 4, type);

            if (sonRight != NULL)
                setNodeData(tree, sonRight, isActive, isRed, chaves, nodeList, 3, type);
        }
    }
}

void navTree(TreeType type) {
    curs_set(0);
    noecho();

    char *sub2;

    RBTree tree; 
    
    switch(type) {
        case HIDRANTE:
            tree = getHydTree();
            sub2 = "Arvore de Hidrantes";
            break;
        case QUADRA:
            tree = getBlockTree();
            sub2 = "Arvore de Quadras";
            break;
        case SEMAFORO:
            tree = getTLightTree();
            sub2 = "Arvore de Semáforos";
            break;
        case RADIOBASE:
            tree = getCTowerTree();
            sub2 = "Arvore de Radio-Base";
            break;
        case PREDIO:
            tree = getBuildingTree();
            sub2 = "Arvore de Prédios";
            break;
        case MURO:
            tree = getWallTree();
            sub2 = "Arvore de Muros";
            break;
        default:
            tree = getBlockTree();
            sub2 = "Default Tree - Quadras";
            break;
    }

    WINDOW *output = newwin(19,79, 5, 1);
    wattron(output, A_BOLD);
    char *sub1 = "Navegação Interativa";
    mvwprintw_center(output, 1, sub1);
    wattroff(output, A_BOLD);
    wattron(output, A_DIM);
    mvwprintw_center_custom(output, 2, 1, sub2);
    refresh();
    box(output, 0, 0);
    wrefresh(output);
    wattroff(output, A_DIM);

    init_pair(4, COLOR_GREEN, -1);
    init_pair(8, COLOR_BLUE, -1);
    init_pair(9, COLOR_RED, -1);


    Node root = RBTree_GetRoot(tree);

    WINDOW *nodes[7];

    int altura = 3, largura = 15;

    Ponto coord[7] = {{9,(80/2) - (largura/2)},  //Pai
                      {12, 40 + 40/2 -  (largura/2)},  //Primeira Linha - Direita
                      {12, 40/2 - (largura/2)},  //Primeira Linha - Esquerda
                      {17, (40/2) + (20/2) - (largura/2)},  //Linha 2 - Bloco Esquerdo - Direita
                      {17, (20/2) - (largura/2)},   //Linha 2 - Bloco Esquerdo - Esquerdo
                      {17, 40 + (40/2) + (20/2) - (largura/2)},  //Linha 2 - Bloco Direito - Direita
                      {17, 40 + (20/2) - (largura/2)}}; //Linha 2 - Bloco Direito - Esquerda

    for (int i = 0; i < 7; i++)
        nodes[i] = newwin(altura, largura, coord[i].x, coord[i].y);
    
    char *chaves[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    bool isRed[7];
    Node nodeList[7];
    bool isActive[7];
    int selected = 0;

    loadTreeData(tree, root, isActive, isRed, chaves, nodeList, type);

    renderTreeLine(output, isActive);
    renderTree(coord, nodes, chaves, isRed, isActive, selected);

    while (true) {
        int ch = getch();
        if (ch == '\033') {
            if (getch() == '[')//Comando de escape
                switch(getch()) {
                    case 'A': //up
                    {
                        int resp = processInputMovement(selected, 'A', isActive);
                        if (resp == -1) {
                            Node actual = nodeList[0];
                            Node parent = RBTreeN_GetParent(tree, actual);
                            if (parent != NULL) {
                                parent = RBTreeN_GetParent(tree, parent);
                                if (parent!= NULL) {
                                    loadTreeData(tree, parent, isActive, isRed, chaves, nodeList, type);
                                    selected = 0;
                                    for (int i = 0; i < 7; i++) {
                                        if (nodeList[i] == actual)
                                            selected = i;
                                    }
                                }
                            }
                                
                        } else{
                            selected = resp;
                        }
                        break;
                    }
                    case 'B': //down
                    {
                        int resp = processInputMovement(selected, 'B', isActive);
                        if (resp == -1) {
                            loadTreeData(tree, nodeList[selected], isActive, isRed, chaves, nodeList, type);
                            selected = 0;
                        } else{
                            selected = resp;
                        }
                        break;
                    }
                    case 'C': //right
                    {
                        selected = processInputMovement(selected, 'C', isActive);
                        break;
                    }
                    case 'D': //left
                    {
                        selected = processInputMovement(selected, 'D', isActive);
                        break;
                    }
                }
            else
                break;
        } else if (ch == 'x' || ch == 'X') {
            break;
        } else if (ch == 10) { //Enter
            detailPopup(tree, nodeList[selected], type);
        } else if (ch == 'e' || ch == 'E') {
            if (selected == 5 || selected == 6 || selected == 4 || selected == 3) {
                loadTreeData(tree, nodeList[selected], isActive, isRed, chaves, nodeList, type);
                selected = 0;
            } else{
                selected = processInputMovement(selected, 'D', isActive);
            }

        } else if (ch == 'd' || ch == 'D') {
            if (selected == 5 || selected == 6 || selected == 4 || selected == 3) {
                loadTreeData(tree, nodeList[selected], isActive, isRed, chaves, nodeList, type);
                selected = 0;
            } else{
                selected = processInputMovement(selected, 'C', isActive);
            }
            
        } else if (ch == 'p' || ch == 'p') {
            int resp = processInputMovement(selected, 'A', isActive);
            if (resp == -1) {
                Node actual = nodeList[0];
                Node parent = RBTreeN_GetParent(tree, actual);
                if (parent != NULL) {
                    parent = RBTreeN_GetParent(tree, parent);
                    if (parent!= NULL) {
                        loadTreeData(tree, parent, isActive, isRed, chaves, nodeList, type);
                        selected = 0;
                        for (int i = 0; i < 7; i++) {
                            if (nodeList[i] == actual)
                                selected = i;
                        }
                    }
                }
                    
            } else{
                selected = resp;
            }
        }

        cleanTree(output);
        renderTreeLine(output, isActive);
        renderTree(coord, nodes, chaves, isRed, isActive, selected);
    }

    for (int i = 0; i < 7; i++) {
        cleanDelete(nodes[i]);
        free(chaves[i]);
    }
    cleanDelete(output);

    refresh();
}

void setResult(WINDOW *processamento, bool success) {
    for (int n = 2; n < 77; n++) {
        mvwaddch(processamento, 2, n, '#');
        wrefresh(processamento);
        refresh();
    }
    wattroff(processamento, COLOR_PAIR(5));
    doneScreen(success, NULL);
}

void processCommand(char *buffer, Files *files, char *baseDir, char *entryFileName) {
    WINDOW *processamento = newwin(4, 79, 5, 1);
    wattron(processamento, A_BOLD);
    mvwprintw(processamento, 1, 1, "Executando o comando ");
    wattroff(processamento, A_BOLD);
    wattron(processamento, A_ITALIC);
    mvwprintw(processamento, 1, 22, "%.50s", buffer);
    wattroff(processamento, A_ITALIC);
    box(processamento, 0, 0);
    wrefresh(processamento);
    refresh();
    wattron(processamento, COLOR_PAIR(15) | A_BOLD);
    mvwaddch(processamento, 2, 1, '[');
    mvwaddch(processamento, 2, 77, ']');
    wrefresh(processamento);
    wattroff(processamento, COLOR_PAIR(15) | A_BOLD);
    refresh();
    init_pair(5,  COLOR_GREEN, -1);
    wattron(processamento, COLOR_PAIR(5));
    wrefresh(processamento);

    char comando[16];
    sscanf(buffer, "%15s", comando);
    if (strcmp(comando, "q") == 0) {
        char qryFileName[64];
        sscanf(buffer + 2, "%63[^\n]", qryFileName);

        if (!Files_OpenQueryFiles(files, baseDir, entryFileName, qryFileName)) {
            setResult(processamento, false);
            return cleanDelete(processamento);
        }
        
        processAndGenerateQuery(files, ALL, NULL);
        setResult(processamento, true);
        return cleanDelete(processamento);
    } else if (strcmp(comando, "dmprbt") == 0) {
        char t = '\0', arq[64] = "";
        sscanf(buffer + 7, "%c %63[^\n]", &t, arq);

        if (t == '\0') {
            doneScreen(false, "Forneça um tipo!");
            return cleanDelete(processamento);
        }

        if (!(t == 'q' || t == 'h' || t == 's' || t == 't' || t == 'p' || t == 'm')) {
            doneScreen(false, "Tipo inválido!");
            return cleanDelete(processamento);
        }

        if (arq[0] == '\0') {
            doneScreen(false, "Forneça um arquivo!");
            return cleanDelete(processamento);
        }

        if (Query_Dmprbt(Files_GetOutputDir(files), t, arq)) {
            setResult(processamento, true);
            return cleanDelete(processamento);
		}

    } else if (strcmp(comando, "nav") == 0) {
        TreeType t = menuTipo();
        navTree(t);
    }

    cleanDelete(processamento);

}


void startGui(Files *files, char *baseDir, char *entryFileName) {
    WINDOW * mainwin;

    if ( (mainwin = initscr()) == NULL ) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
    }

    //Gambiarra pra dar supress no pError

    fclose(stderr);
    stderr = fopen("/dev/null","w");

    start_color();                    
    use_default_colors();
    init_pair(16,  -1,     -1);
    wbkgd(mainwin, COLOR_PAIR(16));
    WINDOW *mainText = newwin(4,79,1,1);    
    init_pair(15,  COLOR_YELLOW, -1);
    box(mainText, 0, 0);
    wattron(mainwin, A_BOLD);
    wattron(mainText, COLOR_PAIR(15)); 
    refresh();
    wrefresh(mainText);
    mvaddstr(1, 2, "Trabalho de Estrutura de Dados");
    refresh();
    wattroff(mainwin, A_BOLD);
    wattroff(mainwin, A_UNDERLINE);
    wattron(mainText, A_ITALIC);
    wattroff(mainText, COLOR_PAIR(15));
    mvwprintw(mainText, 2, 1, "Modo Interativo");
    wattroff(mainText, A_ITALIC);
    wattron(mainText, COLOR_PAIR(15));
    mvwprintw(mainText, 1, 1, "Copyright 2019 © Matheus Bastos, Melvi Trevisan");
    wrefresh(mainText);
    refresh();
    

    char *str = waitCommand();
    while (strcmp(str, "sai") != 0) {
        processCommand(str, files, baseDir, entryFileName);
        free(str);
        str = waitCommand();

    }

    free(str);

    cleanDelete(mainwin);
    endwin();
    refresh();
}

#endif
