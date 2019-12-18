#include "query.h"

static int _dumpBlockResidents(RBTree tree, Node node, FILE *file);

typedef struct BBParameters {
    FILE *file;
    char *color;
} BBParameters;

static void insertBoundingBoxElement(void *o, void *parametersVoid) {
    BBParameters *params = (BBParameters*) parametersVoid;
    FILE *file = params->file;
    if (Object_GetType(o) == OBJ_CIRC) {
        putSVGCircle(file, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
        
        Rectangle surroundingRect = Rectangle_Create(0, 0, 0, 0);
        getSurroundingRect(o, surroundingRect);
        
        putSVGRectangle(file, surroundingRect, params->color, "none", "2");
        Rectangle_Destroy(surroundingRect);
    } else if (Object_GetType(o) == OBJ_RECT) {
        putSVGRectangle(file, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
        Rectangle rect = Object_GetContent(o);
        double cx, cy;
        getCenter(o, &cx, &cy);
        double rx = Rectangle_GetWidth(rect)/2, ry = Rectangle_GetHeight(rect)/2;
        putSVGEllipse(file, cx, cy, rx, ry, params->color, "none");
    }
}

static bool blockInDistanceL1(void *block, void *dVoid) {
    double *d = (double *) dVoid;
    bool topLeftCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block)) <= d[2];
    bool topRightCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), Block_GetY(block)) <= d[2];
    bool bottomLeftCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block) + Block_GetH(block)) <= d[2];
    bool bottomRightCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), 
        Block_GetY(block) + Block_GetH(block)) <= d[2];
    return topLeftCornerInside && topRightCornerInside && bottomLeftCornerInside && bottomRightCornerInside;
}

static bool blockInDistanceL2(void *block, void *dVoid) {
    double *d = (double *) dVoid;
    bool topLeftCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block)) <= d[2];
    bool topRightCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), Block_GetY(block)) <= d[2];
    bool bottomLeftCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block) + Block_GetH(block)) <= d[2];
    bool bottomRightCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), 
        Block_GetY(block) + Block_GetH(block)) <= d[2];
    return topLeftCornerInside && topRightCornerInside && bottomLeftCornerInside && bottomRightCornerInside;
}

bool Query_Overlaps(FILE *txtFile, FILE *outputFile, char idA[], char idB[]) {
    Object a = HashTable_Find(getObjTable(), idA);
    Object b = HashTable_Find(getObjTable(), idB);
    if (a == NULL || b == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado!\n");
        #endif
        fprintf(txtFile, "Elemento não encontrado\n\n");
        return true;
    }
    
    bool overlaps = checkOverlap(a, b);
    if (overlaps) {
        fprintf(txtFile, "SIM\n\n");
    } else {
        fprintf(txtFile, "NAO\n\n");
    }
    Rectangle rectA = Rectangle_Create(0, 0, 0, 0);
    getSurroundingRect(a, rectA);
    Rectangle rectB = Rectangle_Create(0, 0, 0, 0);
    getSurroundingRect(b, rectB);
    double minX = Rectangle_GetX(rectA) < Rectangle_GetX(rectB) ? Rectangle_GetX(rectA) : Rectangle_GetX(rectB);
    double minY = Rectangle_GetY(rectA) < Rectangle_GetY(rectB) ? Rectangle_GetY(rectA) : Rectangle_GetY(rectB);
    double maxX = Rectangle_GetX(rectA) + Rectangle_GetWidth(rectA) < Rectangle_GetX(rectB) + Rectangle_GetWidth(rectB) 
        ? Rectangle_GetX(rectB) + Rectangle_GetWidth(rectB) : Rectangle_GetX(rectA) + Rectangle_GetWidth(rectA);
    double maxY = Rectangle_GetY(rectA) + Rectangle_GetHeight(rectA) < Rectangle_GetY(rectB) + Rectangle_GetHeight(rectB) 
        ? Rectangle_GetY(rectB) + Rectangle_GetHeight(rectB) : Rectangle_GetY(rectA) + Rectangle_GetHeight(rectA);
    putSVGBox(outputFile, minX, minY, maxX - minX, maxY - minY, !overlaps);
    Rectangle_Destroy(rectA);
    Rectangle_Destroy(rectB);

    return true;
}

bool Query_Inside(FILE *txtFile, FILE *outputFile, char id[], double x, double y) {
    Object o = HashTable_Find(getObjTable(), id);
    if (o == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado: %s!\n", id);
        #endif
        fprintf(txtFile, "Elemento não encontrado\n\n");
        return true;
    }

    bool inside = checkInside(o, x, y);
    if (inside) {
        fprintf(txtFile, "INTERNO\n\n");
    } else {
        fprintf(txtFile, "NAO INTERNO\n\n");
    }
    double centerX, centerY;
    getCenter(o, &centerX, &centerY);
    putSVGLine(outputFile, centerX, centerY, x, y);
    putSVGPoint(outputFile, x, y, inside);

    return true;
}

bool Query_Distance(FILE *txtFile, FILE *outputFile, char j[], char k[]) {
    double c1x, c1y, c2x, c2y;
    Object a = HashTable_Find(getObjTable(), j);
    Object b = HashTable_Find(getObjTable(), k);
    if (a == NULL || b == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado!\n");
        #endif
        fprintf(txtFile, "Elemento não encontrado\n\n");
        return true;
    }
    getCenter(a, &c1x, &c1y);
    getCenter(b, &c2x, &c2y);
    double dist = euclideanDistance(c1x, c1y, c2x, c2y);
    fprintf(txtFile, "%lf\n\n", dist);
    putSVGLine(outputFile, c1x, c1y, c2x, c2y);
    char distText[16];
    sprintf(distText, "%lf", dist);
    putSVGText(outputFile, c1x + (c2x - c1x) / 2, c1y + (c2y - c1y) / 2, distText);
}

bool Query_Bb(FILE *txtFile, FILE *outputFile, char outputDir[], char svgFileName[], char suffix[], char color[]) {
    char nameWithSuffix[128];
    strcpy(nameWithSuffix, svgFileName);
    addSuffix(nameWithSuffix, suffix);
    FILE *bbFile = openFile(outputDir, nameWithSuffix, "w");
    if (bbFile == NULL) {
        return true;
    }

    putSVGStart(bbFile);
    BBParameters params = {bbFile, color};
    RBTree_Execute(getObjTree(), insertBoundingBoxElement, &params);
    putSVGEnd(bbFile);
    fclose(bbFile);
}

bool Query_Dq(FILE *txtFile, char metric[], char id[], double dist) {
    Equip e = HashTable_Find(getHydTable(), id);
    if (e == NULL)
        e = HashTable_Find(getCTowerTable(), id);
    if (e == NULL)
        e = HashTable_Find(getTLightTable(), id);
    if (e == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado: %s!\n", id);
        #endif
        return true;
    }

    fprintf(txtFile, "Equipamento ID: %s\n", Equip_GetID(e));

    bool (*_blockInDistance)(void *, void *);
    if (strcmp(metric, "L1") == 0) {
        _blockInDistance = blockInDistanceL1;
    } else if (strcmp(metric, "L2") == 0) {
        _blockInDistance = blockInDistanceL2;
    } else {
        printf("Métrica não reconhecida: %s\n", metric);
        fprintf(txtFile, "Métrica não reconhecida: %s\n\n", metric);
        return true;
    }

    Equip_SetHighlighted(e, true);
    fprintf(txtFile, "Quadras removidas: ");

    double dInfos[3] = {Equip_GetX(e), Equip_GetY(e), dist};
    Block b;
    while (b = RBTree_FindWhere(getBlockTree(), _blockInDistance, (void *) dInfos), b != NULL) {
        fprintf(txtFile, "\n\t- %s", Block_GetCep(b));
        RBTree_Remove(getBlockTree(), Block_GetPoint(b));
        HashTable_Remove(getBlockTable(), Block_GetCep(b));
        Block_Destroy(b);
    }

    fprintf(txtFile, "\n\n");
    return true;
}

bool Query_Del(FILE *txtFile, char id[]) {
    Block b = HashTable_Find(getBlockTable(), id);
    if (b != NULL) {
        fprintf(txtFile, "Informações da quadra removida:\n"
                            "CEP: %s\nPos: (%.2lf, %.2lf)\n"
                            "Largura: %.2lf\nAltura: %.2lf\n\n",
                            Block_GetCep(b), Block_GetX(b), Block_GetY(b), 
                            Block_GetW(b), Block_GetH(b));
        RBTree_Remove(getBlockTree(), Block_GetPoint(b));
        HashTable_Remove(getBlockTable(), id);
        Block_Destroy(b);
        return true;
    }

    Equip e = HashTable_Find(getHydTable(), id);
    if (e != NULL) {
        fprintf(txtFile, "Informações do hidrante removido:\n"
                            "ID: %s\nPos: (%.2lf, %.2lf)\n\n",
                            Equip_GetID(e), Equip_GetX(e), Equip_GetY(e));
        RBTree_Remove(getHydTree(), Equip_GetPoint(e));
        HashTable_Remove(getHydTable(), id);
        Equip_Destroy(e);

        return true;
    }

    e = HashTable_Find(getCTowerTable(), id);
    if (e != NULL) {
        fprintf(txtFile, "Informações da rádio-base removida:\n"
                            "ID: %s\nPos: (%.2lf, %.2lf)\n\n",
                            Equip_GetID(e), Equip_GetX(e), Equip_GetY(e));
        
        RBTree_Remove(getCTowerTree(), Equip_GetPoint(e));
        HashTable_Remove(getCTowerTable(), id);
        Equip_Destroy(e);

        return true;
    }

    e = HashTable_Find(getTLightTable(), id);
    if (e != NULL) {
        fprintf(txtFile, "Informações do semáforo removido:\n"
                            "ID: %s\nPos: (%.2lf, %.2lf)\n\n",
                            Equip_GetID(e), Equip_GetX(e), Equip_GetY(e));
        
        RBTree_Remove(getTLightTree(), Equip_GetPoint(e));
        HashTable_Remove(getTLightTable(), id);
        Equip_Destroy(e);

        return true;
    }
    

    fprintf(txtFile, "Elemento não encontrado\n\n");
    #ifdef __DEBUG__
    printf("Erro: Elemento não encontrado: %s!\n", id);
    #endif
    return true;
}


typedef struct InfosCbq {
    double dInfos[3];
    char *cStrk;
    FILE *txtFile;
} InfosCbq;

static bool _canBeOnLeftSubtree(double x, double r, double xChild) {
    return xChild >= x - r;
}

static bool _canBeOnRightSubtree(double x, double r, double xChild) {
    return xChild <= x + r;
}

static void _changeBlockColorTree(RBTree tree, Node node, InfosCbq *infos) {
    if (node == NULL)
        return;
    double *d = infos->dInfos;
    Block b = RBTreeN_GetValue(tree, node);
    if (_canBeOnLeftSubtree(d[0], d[2], Block_GetX(b)))
        _changeBlockColorTree(tree, RBTreeN_GetLeftChild(tree, node), infos);
    if (blockInDistanceL2(b, (void *) d)) {
        fprintf(infos->txtFile, "\n\t- %s", Block_GetCep(b));
        Block_SetCStroke(b, infos->cStrk);
    }
    if (_canBeOnRightSubtree(d[0], d[2], Block_GetX(b))) {
        _changeBlockColorTree(tree, RBTreeN_GetRightChild(tree, node), infos);
    }
}

bool Query_Cbq(FILE *txtFile, double x, double y, double r, char cStrk[]) {
    fprintf(txtFile, "Quadras que tiveram as bordas alteradas: ");

    InfosCbq infos = {{x, y, r}, cStrk, txtFile};
    _changeBlockColorTree(getBlockTree(), RBTree_GetRoot(getBlockTree()), &infos);

    fprintf(txtFile, "\n\n");
    return true;
}

bool Query_Crd(FILE *txtFile, char id[]) {
    char eqType[24] = "";
    double x, y;

    Equip e;
    Block b;
    if (b = HashTable_Find(getBlockTable(), id), b != NULL) {
        strcpy(eqType, "Quadra");
        x = Block_GetX(b);
        y = Block_GetY(b);
    } else if (e = HashTable_Find(getHydTable(), id), e != NULL) {
        strcpy(eqType, "Hidrante");
        x = Equip_GetX(e);
        y = Equip_GetY(e);
    } else if (e = HashTable_Find(getCTowerTable(), id), e != NULL) {
        strcpy(eqType, "Rádio-base");
        x = Equip_GetX(e);
        y = Equip_GetY(e);
    } else if (e = HashTable_Find(getTLightTable(), id), e != NULL) {
        strcpy(eqType, "Semáforo");
        x = Equip_GetX(e);
        y = Equip_GetY(e);
    }

    if (eqType[0] != '\0') {
        fprintf(txtFile, "Espécie do equipamento: %s\n"
                            "Pos: (%.2lf, %.2lf)\n\n",
                            eqType, x, y);
    } else {
        fprintf(txtFile, "Elemento não encontrado\n\n");
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado: %s!\n", id);
        #endif
    }
    return true;
}

//
// Comando trns
//

typedef struct InfosTrns {
    double x, y, w, h;
} InfosTrns;

typedef struct list_node_t {
    void *element;
    RBTree tree;
    HashTable table;
    struct list_node_t *next;
} ListNode;

static void _translateBlockTree(RBTree tree, Node node, InfosTrns *infos, ListNode **list) {
    if (node == NULL)
        return;
    Block block = RBTreeN_GetValue(tree, node);
    if (_canBeOnLeftSubtree(infos->x + infos->w, infos->w, Block_GetX(block)))
        _translateBlockTree(tree, RBTreeN_GetLeftChild(tree, node), infos, list);

    if (Block_GetX(block) >= infos->x && Block_GetX(block) + Block_GetW(block) <= infos->x + infos->w &&
            Block_GetY(block) >= infos->y && Block_GetY(block) + Block_GetH(block) <= infos->y + infos->h) {
        // Se a quadra estiver dentro do retângulo, adicioná-la à lista
        ListNode *node = malloc(sizeof(struct list_node_t));
        node->element = block;
        node->tree = tree;
        node->next = NULL;
        (*list)->next = node;
        *list = node;
    }

    if (_canBeOnRightSubtree(infos->x, infos->x + infos->w, Block_GetX(block))) {
        _translateBlockTree(tree, RBTreeN_GetRightChild(tree, node), infos, list);
    }
}

static void _translateEquipTree(RBTree tree, Node node, InfosTrns *infos, ListNode **list) {
    if (node == NULL)
        return;
    Equip equip = RBTreeN_GetValue(tree, node);
    if (_canBeOnLeftSubtree(infos->x + infos->w, infos->w, Equip_GetX(equip)))
        _translateEquipTree(tree, RBTreeN_GetLeftChild(tree, node), infos, list);

    if (Equip_GetX(equip) >= infos->x && Equip_GetX(equip) <= infos->x + infos->w &&
            Equip_GetY(equip) >= infos->y && Equip_GetY(equip) <= infos->y + infos->h) {
        // Se o equipamento estiver dentro do retângulo, adicioná-lo à lista
        ListNode *node = malloc(sizeof(struct list_node_t));
        node->element = equip;
        node->tree = tree;
        node->next = NULL;
        (*list)->next = node;
        *list = node;
    }

    if (_canBeOnRightSubtree(infos->x, infos->x + infos->w, Equip_GetX(equip))) {
        _translateEquipTree(tree, RBTreeN_GetRightChild(tree, node), infos, list);
    }
}

bool Query_Trns(FILE *txtFile, double x, double y, double w, double h, double dx, double dy) {
    InfosTrns infos;
    
    infos.x = x;
    infos.y = y;
    infos.w = w;
    infos.h = h;

    fprintf(txtFile, "Equipamentos movidos:");
    
    // Primeiro nó vazio
    ListNode *node = malloc(sizeof(struct list_node_t));
    node->next = NULL;
    ListNode *nodeP = node;

    // Preencher lista
    _translateBlockTree(getBlockTree(), RBTree_GetRoot(getBlockTree()), &infos, &nodeP);
    
    // Descartar primeiro nó (vazio)
    ListNode *next = node->next;
    free(node);
    node = next;

    // Percorrer a lista de quadras a serem transladadas
    while (node != NULL) {
        // Remover devido à mudança de chave
        Block block = RBTree_Remove(node->tree, Block_GetPoint(node->element));

        fprintf(txtFile, "\n%s:"
                         "\n\tPosição anterior: (%.2lf, %.2lf)"
                         "\n\tNova posição: (%.2lf, %.2lf)",
                         Block_GetCep(block), Block_GetX(block), Block_GetY(block),
                         Block_GetX(block) + dx, Block_GetY(block) + dy);

        Block_SetX(block, Block_GetX(block) + dx);
        Block_SetY(block, Block_GetY(block) + dy);

        // Inserir novamente com a chave nova
        RBTree_Insert(node->tree, Block_GetPoint(block), block);

        next = node->next;
        free(node);
        node = next;
    }

    // Primeiro nó vazio
    node = malloc(sizeof(struct list_node_t));
    node->next = NULL;
    nodeP = node;

    // Preencher lista
    _translateEquipTree(getHydTree(), RBTree_GetRoot(getHydTree()), &infos, &nodeP);
    _translateEquipTree(getCTowerTree(), RBTree_GetRoot(getCTowerTree()), &infos, &nodeP);
    _translateEquipTree(getTLightTree(), RBTree_GetRoot(getTLightTree()), &infos, &nodeP);

    // Descartar primeiro nó (vazio)
    next = node->next;
    free(node);
    node = next;

    // Percorrer a lista de equipamentos a serem transladados
    while (node != NULL) {
        // Remover devido à mudança de chave
        Equip equip = RBTree_Remove(node->tree, Equip_GetPoint(node->element));

        fprintf(txtFile, "\n%s:"
                         "\n\tPosição anterior: (%.2lf, %.2lf)"
                         "\n\tNova posição: (%.2lf, %.2lf)",
                         Equip_GetID(equip), Equip_GetX(equip), Equip_GetY(equip),
                         Equip_GetX(equip) + dx, Equip_GetY(equip) + dy);
        Equip_SetX(equip, Equip_GetX(equip) + dx);
        Equip_SetY(equip, Equip_GetY(equip) + dy);
        
        // Inserir novamente com a chave nova
        RBTree_Insert(node->tree, Equip_GetPoint(equip), equip);

        next = node->next;
        free(node);
        node = next;
    }
    
    fprintf(txtFile, "\n\n");

    return true;
}

static int compareDistancesDescending(const void *a, const void *b) {
    double d1 = Distance_GetDist(a);
    double d2 = Distance_GetDist(b);

    return d1 < d2 ? 1 
         : d1 > d2 ? -1 
         : 0;
}

static int compareDistancesAscending(const void *a, const void *b) {
    double d1 = Distance_GetDist(a);
    double d2 = Distance_GetDist(b);

    return d1 > d2 ? 1 
         : d1 < d2 ? -1 
         : 0;
}

static void buildDistances(RBTree tree, Node node, Distance **distances, int x, int y) {
    if (node == NULL)
        return;
    
    buildDistances(tree, RBTreeN_GetLeftChild(tree, node), distances, x, y);
    Equip equip = RBTreeN_GetValue(tree, node);
    double dist = euclideanDistance(Equip_GetX(equip), Equip_GetY(equip), x, y);
    // Adicionar distâcia ao vetor
    *((*distances)++) = Distance_Create(dist, equip);
    buildDistances(tree, RBTreeN_GetRightChild(tree, node), distances, x, y);
}

bool Query_Fi(FILE *txtFile, FILE *outputFile, double x, double y, int ns, double r) {
    fprintf(txtFile, "Semáforos com a programação alterada:");

    int n = RBTree_GetLength(getTLightTree());
    Distance *distances = malloc(n * sizeof(Distance));
    Distance *distancesP = distances;

    // Construir vetor de distâncias dos semáforos ao ponto (x, y)
    buildDistances(getTLightTree(), RBTree_GetRoot(getTLightTree()), &distancesP, x, y);

    // Ordenar as distâncias até que se tenha as 'ns' maiores
    heapsort(distances, n, ns, compareDistancesDescending);

    int limit = n - ns;
    for (int i = n - 1; i >= limit; i--) {
        Equip tLight = Distance_GetEquip(distances[i]);
        Equip_SetHighlighted(tLight, true);
        putSVGLine(outputFile, x, y, Equip_GetX(tLight), Equip_GetY(tLight));
        fprintf(txtFile, "\n\t- %s", Equip_GetID(tLight));
    }

    for (int i = 0; i < n; i++)
        Distance_Destroy(distances[i]);
    free(distances);

    fprintf(txtFile, "\n");
    fprintf(txtFile, "Hidrantes ativados:");

    // Percorrer a árvore de hidrantes
    for (Node node = RBTree_GetFirstNode(getHydTree()); node != NULL; node = RBTreeN_GetSuccessor(getHydTree(), node)) {
        Equip hydrant = RBTreeN_GetValue(getHydTree(), node);
        double dist = euclideanDistance(Equip_GetX(hydrant), Equip_GetY(hydrant), x, y);
        if (dist <= r) {
            Equip_SetHighlighted(hydrant, true);
            putSVGLine(outputFile, x, y, Equip_GetX(hydrant), Equip_GetY(hydrant));
            fprintf(txtFile, "\n\t- %s", Equip_GetID(hydrant));
        }
    }

    fprintf(txtFile, "\n\n");

    return true;
}

bool Query_Fh(FILE *txtFile, FILE *outputFile, char signal, int k, char cep[], char face, double num) {

    Block b = HashTable_Find(getBlockTable(), cep);
    if (b == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado!\n");
        #endif
        fprintf(txtFile, "Elemento não encontrado\n\n");
        return true;
    }

    double x, y;

    if (!Block_GetCoordinates(b, face, num, &x, &y)) {
        #ifdef __DEBUG__
        printf("Erro: Direção desconhecida: '%c'!\n", face);
        #endif
        fprintf(txtFile, "Direção desconhecida: '%c'\n\n", face);
        return true;
    }

    int n = RBTree_GetLength(getHydTree());
    Distance *distances = malloc(n * sizeof(Distance));
    Distance *distancesP = distances;

    // Construir vetor de distâncias dos hidrantes ao ponto (x, y)
    buildDistances(getHydTree(), RBTree_GetRoot(getHydTree()), &distancesP, x, y);

    if (signal == '+') {
        fprintf(txtFile, "%d hidrantes mais distantes:", k);
        //qsort(distances, n, sizeof(EquipDist), compareDistancesAscending);
        heapsort(distances, n, k, compareDistancesAscending);
    } else if (signal == '-') {
        fprintf(txtFile, "%d hidrantes mais próximos:", k);
        //qsort(distances, n, sizeof(EquipDist), compareDistancesDescending);
        heapsort(distances, n, k, compareDistancesDescending);
    } else {
        #ifdef __DEBUG__
        printf("Erro: Sinal desconhecido: '%c'!\n", signal);
        #endif
        fprintf(txtFile, "Sinal desconhecido: '%c'\n\n", signal);
        free(distances);
        return true;
    }

    int limit = n - k;
    for (int i = n - 1; i >= limit; i--) {
        Equip hyd = Distance_GetEquip(distances[i]);
        Equip_SetHighlighted(hyd, true);
        putSVGLine(outputFile, x, y, Equip_GetX(hyd), Equip_GetY(hyd));
        fprintf(txtFile, "\n\t- %s", Equip_GetID(hyd));
    }

    for (int i = 0; i < n; i++)
        Distance_Destroy(distances[i]);
    free(distances);

    fprintf(txtFile, "\n\n");

    return true;
}

bool Query_Fs(FILE *txtFile, FILE *outputFile, int k, char cep[], char face, double num) {

    Block b = HashTable_Find(getBlockTable(), cep);
    if (b == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado!\n");
        #endif
        fprintf(txtFile, "Elemento não encontrado\n\n");
        return true;
    }

    double x, y;

    if (!Block_GetCoordinates(b, face, num, &x, &y)) {
        #ifdef __DEBUG__
        printf("Erro: Direção desconhecida: '%c'!\n", face);
        #endif
        fprintf(txtFile, "Direção desconhecida: '%c'\n\n", face);
        return true;
    }

    int n = RBTree_GetLength(getTLightTree());
    Distance *distances = malloc(n * sizeof(Distance));
    Distance *distancesP = distances;

    // Construir vetor de distâncias dos semáforos ao ponto (x, y)
    buildDistances(getTLightTree(), RBTree_GetRoot(getTLightTree()), &distancesP, x, y);

    fprintf(txtFile, "%d semáforos mais próximos:", k);
    heapsort(distances, n, k, compareDistancesDescending);

    int limit = n - k;
    for (int i = n - 1; i >= limit; i--) {
        Equip hyd = Distance_GetEquip(distances[i]);
        Equip_SetHighlighted(hyd, true);
        putSVGLine(outputFile, x, y, Equip_GetX(hyd), Equip_GetY(hyd));
        fprintf(txtFile, "\n\t- %s", Equip_GetID(hyd));
    }

    for (int i = 0; i < n; i++)
        Distance_Destroy(distances[i]);
    free(distances);

    fprintf(txtFile, "\n\n");

    return true;
}

bool compareAddr(Segment s1, Segment s2) {
    return s1 == s2;
}

bool _pointVisibility(FILE *outputFile, double x, double y, bool buildings, Polygon poly) {

    if (x == 0 && y == 0) {
        Polygon_InsertPoint(poly, 0, 0);
    }

    // int nBuildings = StList_GetNumElements(getBuildingList());
    // int nWalls = StList_GetNumElements(getWallList());
    int nBuildings = RBTree_GetLength(getBuildingTree());
    int nWalls = RBTree_GetLength(getWallTree());

    int nSegments = nBuildings * 4 + nWalls;

    Segment *segments = malloc((nSegments * 2 + 5) * sizeof(Segment));
    Segment *segmentsP = segments;

    double maxX = x, maxY = y;

    if (buildings) {
        // Colocar segmentos dos prédios na lista
        for (Node node = RBTree_GetFirstNode(getBuildingTree()); node != NULL; node = RBTreeN_GetSuccessor(getBuildingTree(), node)) {
            //Building b = StList_Get(getBuildingList(), p);
            Building b = RBTreeN_GetValue(getBuildingTree(), node);
            // if (Building_GetNum(b) != 78)
            //     continue;
            segmentsP = Building_PutSegments(b, segmentsP, x, y);
            double blockMaxX = Building_GetX(b) + Building_GetW(b);
            if (blockMaxX > maxX)
                maxX = blockMaxX;
            double blockMaxY = Building_GetY(b) + Building_GetH(b);
            if (blockMaxY > maxY)
                maxY = blockMaxY;
        }
    }
    
    // Colocar segmentos dos muros na lista
    //for (int p = StList_GetFirstPos(getWallList()); p != -1; p = StList_GetNextPos(getWallList(), p)) {
    for (Node node = RBTree_GetFirstNode(getWallTree()); node != NULL; node = RBTreeN_GetSuccessor(getWallTree(), node)) {
        //Wall w = StList_Get(getWallList(), p);
        Wall w = RBTreeN_GetValue(getWallTree(), node);
        segmentsP = Wall_PutSegments(w, segmentsP, x, y);
        double wallMaxX = max(Wall_GetX1(w), Wall_GetX2(w));
        if (wallMaxX > maxX)
            maxX = wallMaxX;
        double wallMaxY = max(Wall_GetY1(w), Wall_GetY2(w));
        if (wallMaxY > maxY)
            maxY = wallMaxY;
    }

    maxX += 100;
    maxY += 100;

    // Bordas da cidade
    Wall borders[4];
    borders[0] = Wall_Create(0, 0, maxX, 0);
    borders[1] = Wall_Create(maxX, 0, maxX, maxY);
    borders[2] = Wall_Create(maxX, maxY, 0, maxY);
    borders[3] = Wall_Create(0, maxY, 0, 0);
    for (int i = 0; i < 4; i++) {
        segmentsP = Wall_PutSegments(borders[i], segmentsP, x, y);
        Wall_Destroy(borders[i]);
    }

    // Novo número de segmentos
    nSegments = segmentsP - segments;

    int nVertexes = nSegments * 2;
    Vertex *vertexes = malloc(nVertexes * sizeof(Vertex));

    // Criar o vetor de pontos
    for (int i = 0; i < nSegments; i++) {
        Segment s = segments[i];
        // Vertex st = Segment_GetPStart(s);
        // Vertex end = Segment_GetPEnd(s);
        // putSVGSegment(outputFile, Vertex_GetX(st),Vertex_GetY(st), Vertex_GetX(end), Vertex_GetY(end));
        vertexes[2 * i] = Segment_GetPStart(s);
        vertexes[2 * i + 1] = Segment_GetPEnd(s);
    }

    // Ordenar pontos
    qsort(vertexes, nVertexes, sizeof(Vertex), Vertex_Compare);

    //StList activeSegments = StList_Create(nSegments);
    RBTree activeSegments = RBTree_Create(Vertex_Compare);

    for (int i = 0; i < nVertexes; i++) {
        Vertex p = vertexes[i];
        Segment s = Vertex_GetSegment(p);
        double dist = Vertex_GetDistance(p);

        // Coeficiente angular da reta formada pelo ponto central e o ponto atual
        double a1;
        bool vertical = false;
        if (Vertex_GetX(p) == x)
            vertical = true;
        else
            a1 = (Vertex_GetY(p) - y) / (Vertex_GetX(p) - x);
        // Termo independente da reta
        double b1 = y - a1 * x;

        // Distância do segmento ativo mais perto que está atrás do segmento analisado
        double minDist = -1;
        Segment closestSegmentBehind = NULL;
        double xInter, yInter;
        bool inFront = true;

        //for (int pos = StList_GetFirstPos(activeSegments); pos != -1; pos = StList_GetNextPos(activeSegments, pos)) {
        for (Node node = RBTree_GetFirstNode(activeSegments); node != NULL; node = RBTreeN_GetSuccessor(activeSegments, node)) {
            //Segment currentSegment = StList_Get(activeSegments, pos);
            Segment currentSegment = RBTreeN_GetValue(activeSegments, node);
            if (currentSegment == s)
                continue;

            Vertex p1 = Segment_GetPStart(currentSegment);
            Vertex p2 = Segment_GetPEnd(currentSegment);

            // Ponto de intersecção entre a reta anterior e uma nova reta (formada pelo segmento analisado)
            double currentXInter, currentYInter;

            // Evitar divisão por zero
            if (Vertex_GetX(p2) == Vertex_GetX(p1)) {
                currentXInter = Vertex_GetX(p1);
                currentYInter = a1 * currentXInter + b1;
            } else {
                // Coeficiente angular
                double a2 = (Vertex_GetY(p2) - Vertex_GetY(p1)) / (Vertex_GetX(p2) - Vertex_GetX(p1));

                // Termo independente
                double b2 = Vertex_GetY(p1) - a2 * Vertex_GetX(p1);

                if (vertical)
                    currentXInter = Vertex_GetX(p);
                else
                    currentXInter = (b2 - b1) / (a1 - a2);

                currentYInter = a2 * currentXInter + b2;
            }
                

            // Distância entre o ponto de intersecção e o ponto central
            double distInter = euclideanDistance(x, y, currentXInter, currentYInter);

            // Segmento do ponto analisado não está à frente
            if (distInter < dist || fabs(distInter - dist) < 0.000001) {
                inFront = false;
                break;
            } else if (distInter >= dist && (minDist == -1 || distInter <= minDist)) {
                minDist = distInter;
                closestSegmentBehind = currentSegment;
                xInter = currentXInter;
                yInter = currentYInter;
            }
        }

        if (inFront) {
            // Se o segmento estiver na frente de todos os ativos
            if (!Vertex_IsStarting(p)) {
                // Se o ponto for final
                double xBiombo = Segment_GetXBiombo(s);
                double yBiombo = Segment_GetYBiombo(s);
                
                // Colocar luz a partir do biombo do segmento até o ponto
                Polygon_InsertPoint(poly, xBiombo, yBiombo);
                Polygon_InsertPoint(poly, Vertex_GetX(p), Vertex_GetY(p));
                
                //putSVGTriangle(outputFile, xBiombo, yBiombo, x, y, Vertex_GetX(p), Vertex_GetY(p));
                if (closestSegmentBehind != NULL) {
                    // Se houver um segmento atrás
                    // Definir o biombo deste segmento como o ponto de intersecção
                    // entre ele e a reta
                    // fprintf(outputFile, "<circle cx=\"%lf\" cy=\"%lf\" r=\"1\" style=\"stroke:rgb(0,255,255);stroke-width:3\"/>\n",
                    //     xInter, yInter);
                    Segment_SetXBiombo(closestSegmentBehind, xInter);
                    Segment_SetYBiombo(closestSegmentBehind, yInter);
                }
            } else if (closestSegmentBehind != NULL) {
                // Se o ponto for inicial e houver um segmento atrás
                double xBiombo = Segment_GetXBiombo(closestSegmentBehind);
                double yBiombo = Segment_GetYBiombo(closestSegmentBehind); 

                if (vertical || fabs(a1 * xBiombo + b1 - yBiombo) > 0.00001) {
                    // Colocar luz a partir do biombo deste segmento até o ponto de intersecção
                    // entre ele e a reta
                    Polygon_InsertPoint(poly, xBiombo, yBiombo);
                    Polygon_InsertPoint(poly, xInter, yInter);
                    //putSVGTriangle(outputFile, xBiombo, yBiombo, x, y, xInter, yInter);
                }
            }
        }

        if (Vertex_IsStarting(p)) {
            //printf("-\n");
            RBTree_Insert(activeSegments, Segment_GetKey(s), s);  
            //StList_Add(activeSegments, s);         
        } else {
            RBTree_Remove(activeSegments, Segment_GetKey(s));
            //StList_Remove(activeSegments, compareAddr, s);
        }
    }

    Polygon_Connect(poly);

    putSVGBomb(outputFile, x, y);

    free(vertexes);

    RBTree_Destroy(activeSegments, NULL);
    //StList_Destroy(activeSegments, Segment_Destroy);
    for (int i = 0; i < nSegments; i++) {
        Segment_Destroy(segments[i]);
    }
    free(segments);

    return true;
}

bool Query_Brl(FILE *outputFile, double x, double y) {
    Polygon poly = Polygon_Create();
    
    _pointVisibility(outputFile, x, y, true, poly);
    putSVGPolygon(outputFile, poly, "gold");
    Polygon_Destroy(poly);

    return true;
}

static int _dumpBlockResidents(RBTree tree, Node node, FILE *file) {
    if (node == NULL)
        return 0;
    int i = 1;
    i += _dumpBlockResidents(tree, RBTreeN_GetLeftChild(tree, node), file);
    fprintf(file, "%d)\n", i);
    Person_DumpToFile(RBTreeN_GetValue(tree, node), file);
    i += _dumpBlockResidents(tree, RBTreeN_GetRightChild(tree, node), file);
    return i;
}

static int _dumpBuildingResidents(RBTree tree, Node node, FILE *file) {
    if (node == NULL)
        return 0;
    int i = 1;
    i += _dumpBuildingResidents(tree, RBTreeN_GetLeftChild(tree, node), file);
    fprintf(file, "%d)\n", i);
    Person_DumpToFile(RBTreeN_GetValue(tree, node), file);
    i += _dumpBuildingResidents(tree, RBTreeN_GetRightChild(tree, node), file);
    return i;
}

static void _executeBrnBlocks(RBTree tree, Node node, Polygon polygon, FILE *txtFile) {
    if (node == NULL)
        return;
    Block block = RBTreeN_GetValue(tree, node);
    if (Block_GetX(block) >= Polygon_GetMinX(polygon))
        _executeBrnBlocks(tree, RBTreeN_GetLeftChild(tree, node), polygon, txtFile);
    if (Polygon_IsBlockInside(polygon, block, true)) {
        fprintf(txtFile, "Moradores da quadra %s:\n", Block_GetCep(block));
        RBTree residents = Block_GetResidents(block);
        int total = _dumpBlockResidents(residents, RBTree_GetRoot(residents), txtFile);
        fprintf(txtFile, "TOTAL: %d\n", total);
    }
    if (Block_GetX(block) <= Polygon_GetMaxX(polygon))
        _executeBrnBlocks(tree, RBTreeN_GetRightChild(tree, node), polygon, txtFile);
}

static void _executeBrnBuildings(RBTree tree, Node node, Polygon polygon, FILE *txtFile) {
    if (node == NULL)
        return;
    Building building = RBTreeN_GetValue(tree, node);
    if (Building_GetX(building) >= Polygon_GetMinX(polygon))
        _executeBrnBuildings(tree, RBTreeN_GetLeftChild(tree, node), polygon, txtFile);
    if (Polygon_IsBuildingInside(polygon, building)) {
        fprintf(txtFile, "Moradores do predio %s:\n", Building_GetKey(building));
        RBTree residents = Building_GetResidents(building);
        int total = _dumpBuildingResidents(residents, RBTree_GetRoot(residents), txtFile);
        fprintf(txtFile, "TOTAL: %d\n", total);
    }
    if (Building_GetX(building) <= Polygon_GetMaxX(polygon))
        _executeBrnBuildings(tree, RBTreeN_GetRightChild(tree, node), polygon, txtFile);
}

static void _executeBrnNodes(RBTree tree, Node node, Polygon polygon) {
    if (node == NULL)
        return;
    GraphNode graphNode = RBTreeN_GetValue(tree, node);
    _executeBrnNodes(tree, RBTreeN_GetLeftChild(tree, node), polygon);
    GraphNode_DestroyEdgesAffected(graphNode, polygon);
    _executeBrnNodes(tree, RBTreeN_GetRightChild(tree, node), polygon);
}

bool Query_Brn(FILE *txtFile, FILE *outputFile, double x, double y, char *outputDir, char *arqPol) {
    FILE *polyFile = openFile(outputDir, arqPol, "w");
    if (polyFile == NULL)
        return false;
    
    Polygon poly = Polygon_Create();
    
    _pointVisibility(outputFile, x, y, false, poly);
    putSVGPolygon(outputFile, poly, "gold");

    Polygon_DumpToFile(poly, polyFile);
    fclose(polyFile);

    _executeBrnBlocks(getBlockTree(), RBTree_GetRoot(getBlockTree()), poly, txtFile);
    _executeBrnBuildings(getBuildingTree(), RBTree_GetRoot(getBuildingTree()), poly, txtFile);
    _executeBrnNodes(getNodeTree(), RBTree_GetRoot(getNodeTree()), poly);

    fputs("\n", txtFile);
    Polygon_Destroy(poly);

    return true;
}

bool Query_M(FILE *txtFile, char *cep) {
    Block block = HashTable_Find(getBlockTable(), cep);
    if (block == NULL) {
        fprintf(txtFile, "Quadra de CEP %s não encontrada\n\n", cep);
        return true;
    }

    fprintf(txtFile, "Moradores da quadra %s:\n", cep);
    RBTree residents = Block_GetResidents(block);
    int total = _dumpBlockResidents(residents, RBTree_GetRoot(residents), txtFile);
    fprintf(txtFile, "TOTAL: %d\n", total);

    fputs("\n", txtFile);

    return true;
}

static void _executeMplgBlocks(RBTree tree, Node node, Polygon polygon, FILE *outputFile) {
    if (node == NULL)
        return;
    Block block = RBTreeN_GetValue(tree, node);
    if (Block_GetX(block) >= Polygon_GetMinX(polygon))
        _executeMplgBlocks(tree, RBTreeN_GetLeftChild(tree, node), polygon, outputFile);
    if (Polygon_IsBlockInside(polygon, block, true)) {
        Block_SetWStroke(block, "4.00000");
        RBTree residents = Block_GetResidents(block);
        int total = RBTree_GetLength(residents);
        fprintf(outputFile, "<text x=\"%lf\" y=\"%lf\" text-anchor=\"middle\" "
                            "dominant-baseline=\"middle\" font-size=\"20\">%d</text>",
                            Block_GetX(block) + Block_GetW(block) / 2,
                            Block_GetY(block) + Block_GetH(block) / 2,
                            total);
    }
    if (Block_GetX(block) + Block_GetW(block) <= Polygon_GetMaxX(polygon))
        _executeMplgBlocks(tree, RBTreeN_GetRightChild(tree, node), polygon, outputFile);
}

static void _executeMplgBuildings(RBTree tree, Node node, Polygon polygon, FILE *txtFile) {
    if (node == NULL)
        return;
    Building building = RBTreeN_GetValue(tree, node);
    if (Building_GetX(building) >= Polygon_GetMinX(polygon))
        _executeMplgBuildings(tree, RBTreeN_GetLeftChild(tree, node), polygon, txtFile);
    if (Polygon_IsBuildingInside(polygon, building)) {
        fprintf(txtFile, "Moradores do predio %s:\n", Building_GetKey(building));
        RBTree residents = Building_GetResidents(building);
        if (RBTree_GetLength(residents) > 0)
            Building_SetPainted(building, true);
        int total = _dumpBuildingResidents(residents, RBTree_GetRoot(residents), txtFile);
        fprintf(txtFile, "TOTAL: %d\n", total);
    }
    if (Building_GetX(building) + Building_GetW(building) <= Polygon_GetMaxX(polygon))
        _executeMplgBuildings(tree, RBTreeN_GetRightChild(tree, node), polygon, txtFile);
}

bool Query_Mplg(FILE *txtFile, FILE *outputFile, char* baseDir, char *arqPolig) {
    FILE *polyFile = openFile(baseDir, arqPolig, "r");
    if (polyFile == NULL)
        return false;
    
    Polygon poly = Polygon_Create();
    Polygon_ReadFromFile(poly, polyFile);
    fclose(polyFile);

    _executeMplgBlocks(getBlockTree(), RBTree_GetRoot(getBlockTree()), poly, outputFile);
    _executeMplgBuildings(getBuildingTree(), RBTree_GetRoot(getBuildingTree()), poly, txtFile);

    Polygon_Destroy(poly);
}

bool Query_Dm(FILE *txtFile, char *cpf) {
    Person person = HashTable_Find(getPersonTable(), cpf);
    if (person == NULL) {
        fputs("Morador não encontrado\n\n", txtFile);
        return true;
    }

    fprintf(txtFile, "Dados do morador: \n");
    Person_DumpToFile(person, txtFile);
    fputs("\n", txtFile);

    return true;
}

bool Query_De(FILE *txtFile, char *cnpj) {
    Commerce commerce = HashTable_Find(getCommerceTable(), cnpj);
    if (commerce == NULL) {
        fputs("Estabelecimento não encontrado\n\n", txtFile);
        return true;
    }

    Commerce_DumpToFile(commerce, txtFile);
    fprintf(txtFile, "Dados do proprietário:\n");
    Person_DumpToFile(Commerce_GetOwner(commerce), txtFile);
    fputs("\n", txtFile);
    return true;
}

bool Query_Mud(FILE *txtFile, char *cpf, char *cep, char face, int num, char *compl) {
    Person person = HashTable_Find(getPersonTable(), cpf);
    if (person == NULL) {
        fputs("Morador não encontrado\n\n", txtFile);
        return true;
    }

    char address[64];
    Building_MakeAddress(address, cep, face, num);

    Block newBlock = HashTable_Find(getBlockTable(), cep);
    if (newBlock == NULL) {
        fprintf(txtFile, "Quadra de CEP %s não encontrada\n\n", cep);
        return true;
    }
    Building newBuilding = RBTree_Find(Block_GetBuildings(newBlock), address);

    fprintf(txtFile, "Dados do morador da mudança: \n");
    Person_DumpToFile(person, txtFile);
    fprintf(txtFile, "\tNovo endereço: %s %s\n\n", address, compl);

    if (Person_GetBuilding(person) != NULL)
        Building_RemoveResident(Person_GetBuilding(person), person);
    if (Person_GetBlock(person) != NULL)
        Block_RemoveResident(Person_GetBlock(person), person);
    Person_SetBlock(person, newBlock);
    Person_SetBuilding(person, newBuilding);
    Person_SetAddress(person, address);
    Person_SetComplement(person, compl);
    if (newBuilding != NULL)
        Building_InsertResident(newBuilding, person);
    Block_InsertResident(newBlock, person);

    return true;
}

static int _dumpCommerces(RBTree tree, Node node, FILE *file, char *type, bool isBuilding) {
    if (node == NULL)
        return 0;
    int i = 0;
    i += _dumpCommerces(tree, RBTreeN_GetLeftChild(tree, node), file, type, isBuilding);
    Commerce commerce = RBTreeN_GetValue(tree, node);
    CommerceType commType = Commerce_GetType(commerce);
    if (strcmp(type, "*") == 0 || strcmp(type, CommerceType_GetCode(commType)) == 0) {
        i++;
        if (isBuilding || Commerce_GetBuilding(commerce) == NULL) {
            fprintf(file, "%d)\n", i);
            Commerce_DumpToFile(commerce, file);
            Person owner = Commerce_GetOwner(commerce);
            if (owner != NULL) {
                fprintf(file, "\tNome do proprietário: %s\n", Person_GetName(owner));
            }
        }
    }
    i += _dumpCommerces(tree, RBTreeN_GetRightChild(tree, node), file, type, isBuilding);
    return i;
}

static void _executeEplgBlocks(RBTree tree, Node node, Polygon polygon, FILE *txtFile, FILE *outputFile, char *type) {
    if (node == NULL)
        return;
    Block block = RBTreeN_GetValue(tree, node);
    if (Block_GetX(block) >= Polygon_GetMinX(polygon))
        _executeEplgBlocks(tree, RBTreeN_GetLeftChild(tree, node), polygon, txtFile, outputFile, type);
    if (Polygon_IsBlockInside(polygon, block, false)) {
        RBTree commerces = Block_GetCommerces(block);
        fprintf(txtFile, "Estabelecimentos comerciais do tipo %s na quadra %s:\n", type, Block_GetCep(block));
        int total = _dumpCommerces(commerces, RBTree_GetRoot(commerces), txtFile, type, false);
        if (total > 0) {
            if (strcmp(Block_GetCFill(block), "darkolivegreen") == 0)
                Block_SetCFill(block, "indigo");
            else
                Block_SetCFill(block, "darkolivegreen");
        }
    }
    if (Block_GetX(block) + Block_GetW(block) <= Polygon_GetMaxX(polygon))
        _executeEplgBlocks(tree, RBTreeN_GetRightChild(tree, node), polygon, txtFile, outputFile, type);
}

static void _executeEplgBuildings(RBTree tree, Node node, Polygon polygon, FILE *txtFile, char *type) {
    if (node == NULL)
        return;
    Building building = RBTreeN_GetValue(tree, node);
    if (Building_GetX(building) >= Polygon_GetMinX(polygon))
        _executeEplgBuildings(tree, RBTreeN_GetLeftChild(tree, node), polygon, txtFile, type);
    if (Polygon_IsBuildingInside(polygon, building)) {
        fprintf(txtFile, "Estabelecimentos comerciais do tipo %s no prédio %s:\n", type, Building_GetKey(building));
        RBTree commerces = Building_GetCommerces(building);
        int total = _dumpCommerces(commerces, RBTree_GetRoot(commerces), txtFile, type, true);
        if (total > 0) {
            Building_SetHighlighted(building, true);
        }
    }
    if (Building_GetX(building) + Building_GetW(building) <= Polygon_GetMaxX(polygon))
        _executeEplgBuildings(tree, RBTreeN_GetRightChild(tree, node), polygon, txtFile, type);
}

bool Query_Eplg(FILE *txtFile, FILE *outputFile, char *baseDir, char *arqPolig, char *type) {
    FILE *polyFile = openFile(baseDir, arqPolig, "r");
    if (polyFile == NULL)
        return false;
    
    Polygon poly = Polygon_Create();
    Polygon_ReadFromFile(poly, polyFile);
    fclose(polyFile);
    
    _executeEplgBlocks(getBlockTree(), RBTree_GetRoot(getBlockTree()), poly, txtFile, outputFile, type);
    _executeEplgBuildings(getBuildingTree(), RBTree_GetRoot(getBuildingTree()), poly, txtFile, type);

    Polygon_Destroy(poly);
}

static void _executeCatacBlocks(RBTree tree, Node node, Polygon polygon, ListNode **list) {
    if (node == NULL)
        return;
    Block block = RBTreeN_GetValue(tree, node);
    if (Block_GetX(block) >= Polygon_GetMinX(polygon))
        _executeCatacBlocks(tree, RBTreeN_GetLeftChild(tree, node), polygon, list);
    if (Polygon_IsBlockInside(polygon, block, false)) {
        ListNode *node = malloc(sizeof(struct list_node_t));
        node->element = block;
        node->tree = tree;
        node->next = NULL;
        (*list)->next = node;
        *list = node;
    }
    if (Block_GetX(block) <= Polygon_GetMaxX(polygon) + 5.0)
        _executeCatacBlocks(tree, RBTreeN_GetRightChild(tree, node), polygon, list);
}

static void _executeCatacBuildings(RBTree tree, Node node, Polygon polygon, ListNode **list) {
    if (node == NULL)
        return;
    Building building = RBTreeN_GetValue(tree, node);
    if (Building_GetX(building) >= Polygon_GetMinX(polygon))
        _executeCatacBuildings(tree, RBTreeN_GetLeftChild(tree, node), polygon, list);
    if (Polygon_IsBuildingInside(polygon, building)) {
        ListNode *node = malloc(sizeof(struct list_node_t));
        node->element = building;
        node->tree = tree;
        node->next = NULL;
        (*list)->next = node;
        *list = node;
    }
    if (Building_GetX(building) <= Polygon_GetMaxX(polygon))
        _executeCatacBuildings(tree, RBTreeN_GetRightChild(tree, node), polygon, list);
}

static void _executeCatacResidents(RBTree tree, Node node, FILE *txtFile, bool removeFromTrees) {
    if (node == NULL)
        return;
    Person person = RBTreeN_GetValue(tree, node);
    _executeCatacResidents(tree, RBTreeN_GetLeftChild(tree, node), txtFile, removeFromTrees);

    fprintf(txtFile, "\t- Pessoa %s\n", Person_GetCpf(person));

    if (Person_GetBlock(person) != NULL) {
        if (removeFromTrees)
            Block_RemoveResident(Person_GetBlock(person), person);
        Person_SetBlock(person, NULL);
    }
    if (Person_GetBuilding(person) != NULL) {
        //Building_RemoveResident(Person_GetBuilding(person), person);
        Person_SetBuilding(person, NULL);
    }

    HashTable_Remove(getPersonTable(), Person_GetCpf(person));

    Person_Destroy(person);

    _executeCatacResidents(tree, RBTreeN_GetRightChild(tree, node), txtFile, removeFromTrees);
}

static void _executeCatacEquips(RBTree tree, Node node, HashTable table, Polygon polygon, ListNode **list) {
    if (node == NULL)
        return;

    Equip equip = RBTreeN_GetValue(tree, node);

    if (Equip_GetX(equip) >= Polygon_GetMinX(polygon));
        _executeCatacEquips(tree, RBTreeN_GetLeftChild(tree, node), table, polygon, list);


    if (Polygon_IsPointInside(polygon, Equip_GetX(equip), Equip_GetY(equip))) {
        ListNode *node = malloc(sizeof(struct list_node_t));
        node->element = equip;
        node->tree = tree;
        node->table = table;
        node->next = NULL;
        (*list)->next = node;
        *list = node;
    }

    if (Equip_GetX(equip) <= Polygon_GetMaxX(polygon));
        _executeCatacEquips(tree, RBTreeN_GetRightChild(tree, node), table, polygon, list);
}

bool Query_Catac(FILE *outputFile, FILE *txtFile, char *baseDir, char *arqPolig) {
    FILE *polyFile = openFile(baseDir, arqPolig, "r");
    if (polyFile == NULL)
        return false;
    
    Polygon poly = Polygon_Create();
    Polygon_ReadFromFile(poly, polyFile);
    fclose(polyFile);


    fprintf(txtFile, "Itens removidos:\n");

    // Prédios
    // Primeiro nó vazio
    ListNode *node = malloc(sizeof(struct list_node_t));
    node->next = NULL;
    ListNode *nodeP = node;

    // Preencher lista de prédios a serem removidos
    _executeCatacBuildings(getBuildingTree(), RBTree_GetRoot(getBuildingTree()), poly, &nodeP);
    
    // Descartar primeiro nó (vazio)
    ListNode *next = node->next;
    free(node);
    node = next;

    while (node != NULL) {
        Building building = node->element;

        RBTree residents = Building_GetResidents(building);
        // X nas diagonais do prédio
        putSVGCross(outputFile, building);
        // Número de residentes
        fprintf(outputFile, "<text x=\"%lf\" y=\"%lf\" font-size=\"12\">%d</text>",
                            Building_GetX(building) + Building_GetW(building) / 2,
                            Building_GetY(building) + Building_GetH(building) / 2,
                            RBTree_GetLength(residents));

        // Excluir residentes
        _executeCatacResidents(residents, RBTree_GetRoot(residents), txtFile, true);
        fprintf(txtFile, "\t- Prédio %s\n", Building_GetKey(building));
        // Remover prédio das estruturas
        RBTree_Remove(getBuildingTree(), Building_GetPoint(building));
        RBTree_Remove(Block_GetBuildings(Building_GetBlock(building)), Building_GetKey(building));

        Building_Destroy(building);

        next = node->next;
        free(node);
        node = next;
    }

    // Quadras
    // Primeiro nó vazio
    node = malloc(sizeof(struct list_node_t));
    node->next = NULL;
    nodeP = node;

    // Preencher lista de quadras a serem removidas
    _executeCatacBlocks(getBlockTree(), RBTree_GetRoot(getBlockTree()), poly, &nodeP);
    
    // Descartar primeiro nó (vazio)
    next = node->next;
    free(node);
    node = next;

    while (node != NULL) {
        Block block = node->element;

        RBTree residents = Block_GetResidents(block);

        // Excluir residentes
        _executeCatacResidents(residents, RBTree_GetRoot(residents), txtFile, false);
        fprintf(txtFile, "\t- Quadra %s\n", Block_GetCep(block));
        // Remover quadra das estruturas
        RBTree_Remove(getBlockTree(), Block_GetPoint(block));
        HashTable_Remove(getBlockTable(), Block_GetCep(block));

        Block_Destroy(block);

        next = node->next;
        free(node);
        node = next;
    }

    // Equipamentos
    // Primeiro nó vazio
    node = malloc(sizeof(struct list_node_t));
    node->next = NULL;
    nodeP = node;

    // Preencher lista dos equipamentos urbanos a serem removidos
    _executeCatacEquips(getHydTree(), RBTree_GetRoot(getHydTree()), getHydTable(), poly, &nodeP);
    _executeCatacEquips(getCTowerTree(), RBTree_GetRoot(getCTowerTree()), getCTowerTable(), poly, &nodeP);
    _executeCatacEquips(getTLightTree(), RBTree_GetRoot(getTLightTree()), getTLightTable(), poly, &nodeP);

    // Descartar primeiro nó (vazio)
    next = node->next;
    free(node);
    node = next;

    while (node != NULL) {
        Equip equip = node->element;

        fprintf(txtFile, "\t- Equipamento %s\n", Equip_GetID(equip));
        // Excluir equipamento de suas estruturas
        RBTree_Remove(node->tree, Equip_GetPoint(equip));
        HashTable_Remove(node->table, Equip_GetID(equip));

        Equip_Destroy(equip);

        next = node->next;
        free(node);
        node = next;
    }

    // Desenhar polígono no SVG
    putSVGPolygon(outputFile, poly, "white");
    Polygon_Destroy(poly);

    return true;
}

bool Query_Dmprbt(char *outputDir, char t, char *arq) {
    strcat(arq, ".svg");
    FILE *file = openFile(outputDir, arq, "w");
    if (file == NULL)
        return false;

    char description[64];

    switch (t) {
        case 'q':
            putSVGRBTree(file, getBlockTree(), Block_Describe);
            break;
        case 'h':
            putSVGRBTree(file, getHydTree(), Equip_Describe);
            break;
        case 's':
            putSVGRBTree(file, getTLightTree(), Equip_Describe);
            break;
        case 't':
            putSVGRBTree(file, getCTowerTree(), Equip_Describe);
            break;
        case 'p':
            putSVGRBTree(file, getBuildingTree(), Building_Describe);
            break;
        case 'm':
            putSVGRBTree(file, getWallTree(), Wall_Describe);
            break;
        default:
            printf("Árvore inexistente: %c!\n", t);
            fclose(file);
            return false;
    }

    fclose(file);

    return true;
}
