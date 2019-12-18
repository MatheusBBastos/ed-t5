#include <string.h>
#include "commands.h"
#include "modules/util/file_util.h"

bool processGeometry(FILE *entryFile);

bool processCommerces(FILE *ecFile);

bool processPeople(FILE *pmFile);

bool processStreets(FILE *viaFile);

bool processQuery(FILE *queryFile, FILE *outputFile, FILE *txtFile, char baseDir[], char outputDir[], 
                  char svgFileName[], PathFindMode pathMode, PathStack *pathStack);

void writeObject(Object o, void *param) {
    FILE *svgFile = (FILE *) param;
    if (Object_GetType(o) == OBJ_CIRC) {
        putSVGCircle(svgFile, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
    } else if (Object_GetType(o) == OBJ_RECT) {
        putSVGRectangle(svgFile, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
    }
}

void writeText(Text text, void *param) {
    FILE *svgFile = (FILE *) param;
    putSVGText(svgFile, Text_GetX(text), Text_GetY(text), Text_GetString(text));
}

void writeSVG(FILE *outputSVGFile, bool svgTag) {
    if (svgTag)
        putSVGStart(outputSVGFile);
    RBTree_Execute(getObjTree(), writeObject, outputSVGFile);
    RBTree_Execute(getTextTree(), writeText, outputSVGFile);
    RBTree_Execute(getBlockTree(), putSVGBlock, outputSVGFile);
    RBTree_Execute(getBuildingTree(), putSVGBuilding, outputSVGFile);
    RBTree_Execute(getWallTree(), putSVGWall, outputSVGFile);
    RBTree_Execute(getHydTree(), putSVGHydrant, outputSVGFile);
    RBTree_Execute(getTLightTree(), putSVGTrafficLight, outputSVGFile);
    RBTree_Execute(getCTowerTree(), putSVGCellTower, outputSVGFile);
    if (svgTag)
        putSVGEnd(outputSVGFile);
}

void processAll(Files files) {
    processGeometry(Files_GetEntryFile(files));
    writeSVG(Files_GetOutputSVGFile(files), true);

    if (Files_GetPmFile(files) != NULL)
        processPeople(Files_GetPmFile(files));
    if (Files_GetEcFile(files) != NULL)
        processCommerces(Files_GetEcFile(files));
    if (Files_GetViaFile(files) != NULL)
        processStreets(Files_GetViaFile(files));
    
    if (Files_GetQueryFile(files) != NULL) {
        processAndGenerateQuery(files, ALL, NULL);
    }
}

void processAndGenerateQuery(Files files, PathFindMode pathMode, PathStack *pathStack) {
    FILE *outputQryFile = Files_GetOutputQryFile(files);
    putSVGStart(outputQryFile);
    putSVGQueryStart(outputQryFile);
    processQuery(Files_GetQueryFile(files), outputQryFile, Files_GetTxtFile(files), Files_GetBaseDir(files),
                    Files_GetOutputDir(files), Files_GetQrySVGFileName(files), pathMode, pathStack);
    putSVGQueryEnd(outputQryFile);
    writeSVG(outputQryFile, false);
    putSVGUseQuery(outputQryFile);
    putSVGEnd(outputQryFile);

    fclose(outputQryFile);
    fclose(Files_GetTxtFile(files));
    fclose(Files_GetQueryFile(files));
}

bool processGeometry(FILE *entryFile) {
    int nx = DEFAULT_MAXIMUM;
    int nb = DEFAULT_MAXIMUM;
    int nh = DEFAULT_MAXIMUM;
    int nc = DEFAULT_MAXIMUM;
    int nt = DEFAULT_MAXIMUM;
    int np = DEFAULT_MAXIMUM;
    int nw = DEFAULT_MAXIMUM;

    char cFillBlock[24];
    char cStrkBlock[24];
    char wStrkBlock[16];

    char cFillHydrant[24];
    char cStrkHydrant[24];
    char wStrkHydrant[16];

    char cFillCellTower[24];
    char cStrkCellTower[24];
    char wStrkCellTower[16];

    char cFillTrafficLight[24];
    char cStrkTrafficLight[24];
    char wStrkTrafficLight[16];

    char wStrkCircle[16] = "2";
    char wStrkRectangle[16] = "2";

    char buffer[128];
    while (fgets(buffer, 100, entryFile) != NULL) {
        char type[16];
        sscanf(buffer, "%15s", type);
        if (strcmp(type, "nx") == 0) {
            int nx1, nb1, nh1, nc1, nt1, np1, nw1;
            sscanf(buffer + 3, "%d %d %d %d %d %d %d", &nx1, &nb1, &nh1, &nc1, &nt1, &np1, &nw1);

            if (nx != nx1 || nb != nb1 || nh != nh1 || nc != nc1 || nt != nt1 || np != np1 || nw != nw1) {
                nx = nx1;
                nb = nb1;
                nh = nh1;
                nc = nc1;
                nt = nt1;
                np = np1;
                nw = nw1;
                //resizeLists(nx, nb, nh, nc, nt, np, nw);
            }
        } else if (strcmp(type, "c") == 0) {
            char id[8];
            double radius, x, y;
            char color1[32], color2[32];
            sscanf(buffer + 2, "%s %lf %lf %lf %s %s", id, &radius, &x, &y, color1, color2);

            Circle c = Circle_Create(radius, x, y);
            Object o = Object_Create(id, c, OBJ_CIRC, color1, color2, wStrkCircle);

            Object replacedObject = RBTree_Insert(getObjTree(), Circle_GetPoint(c), o);
            if (replacedObject != NULL)
                Object_Destroy(replacedObject);

        } else if (strcmp(type, "r") == 0) {
            char id[8];
            double width, height, x, y;
            char color1[32], color2[32];
            sscanf(buffer + 2, "%s %lf %lf %lf %lf %s %s", id, &width, &height, &x, &y, color1, color2);

            Rectangle r = Rectangle_Create(width, height, x, y);
            Object o = Object_Create(id, r, OBJ_RECT, color1, color2, wStrkRectangle);

            Object replacedObject = RBTree_Insert(getObjTree(), Rectangle_GetPoint(r), o);
            if (replacedObject != NULL)
                Object_Destroy(replacedObject);

        } else if (strcmp(type, "t") == 0) {
            double x, y;
            char string[128];
            sscanf(buffer + 2, "%lf %lf %128[^\n]", &x, &y, string);

            Text t = Text_Create(x, y, string);

            Text replacedText = RBTree_Insert(getTextTree(), Text_GetPoint(t), t);
            if (replacedText != NULL)
                Text_Destroy(replacedText);

        } else if (strcmp(type, "cq") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillBlock, cStrkBlock, wStrkBlock);
        } else if (strcmp(type, "ch") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillHydrant, cStrkHydrant, wStrkHydrant);
        } else if (strcmp(type, "cr") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillCellTower, cStrkCellTower, wStrkCellTower);
        } else if (strcmp(type, "cs") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillTrafficLight, cStrkTrafficLight, wStrkTrafficLight);
        } else if (strcmp(type, "q") == 0) {
            char cep[16];
            double x, y, w, h;
            sscanf(buffer + 2, "%15s %lf %lf %lf %lf", cep, &x, &y, &w, &h);

            Block block = Block_Create(cep, x, y, w, h, cFillBlock, cStrkBlock, wStrkBlock);

            Block replacedBlock = RBTree_Insert(getBlockTree(), Block_GetPoint(block), block);
            if (replacedBlock != NULL)
                Block_Destroy(replacedBlock);

            HashTable_Insert(getBlockTable(), Block_GetCep(block), block);

        } else if (strcmp(type, "h") == 0) {
            char id[16];
            double x, y;
            sscanf(buffer + 2, "%15s %lf %lf", id, &x, &y);
            
            Equip hydrant = Equip_Create(id, x, y, cFillHydrant, cStrkHydrant, wStrkHydrant);

            Equip replacedEquip = RBTree_Insert(getHydTree(), Equip_GetPoint(hydrant), hydrant);
            if (replacedEquip != NULL)
                Equip_Destroy(replacedEquip);

            HashTable_Insert(getHydTable(), Equip_GetID(hydrant), hydrant);

        } else if (strcmp(type, "s") == 0) {
            char id[16];
            double x, y;
            sscanf(buffer + 2, "%15s %lf %lf", id, &x, &y);
            
            Equip trLight = Equip_Create(id, x, y, cFillTrafficLight, cStrkTrafficLight, wStrkTrafficLight);
            
            Equip replacedEquip = RBTree_Insert(getTLightTree(), Equip_GetPoint(trLight), trLight);
            if (replacedEquip != NULL)
                Equip_Destroy(replacedEquip);

            HashTable_Insert(getTLightTable(), Equip_GetID(trLight), trLight);

        } else if (strcmp(type, "rb") == 0) {
            char id[16];
            double x, y;
            sscanf(buffer + 2, "%15s %lf %lf", id, &x, &y);
            
            Equip cellTower = Equip_Create(id, x, y, cFillCellTower, cStrkCellTower, wStrkCellTower);
            
            Equip replacedEquip = RBTree_Insert(getCTowerTree(), Equip_GetPoint(cellTower), cellTower);
            if (replacedEquip != NULL)
                Equip_Destroy(replacedEquip);

            HashTable_Insert(getCTowerTable(), Equip_GetID(cellTower), cellTower);

        } else if (strcmp(type, "prd") == 0) {
            char cep[16];
            char face;
            double num, f, p, mrg;
            sscanf(buffer + 3, "%15s %c %lf %lf %lf %lf", cep, &face, &num, &f, &p, &mrg);

            Block block = HashTable_Find(getBlockTable(), cep);
            if (block == NULL) {
                #ifdef __DEBUG__
                printf("Erro: Quadra de CEP %s não encontrada!\n", cep);
                #endif
                continue;
            }

            Building building = Building_Create(block, face, num, f, p, mrg);
            Block_InsertBuilding(block, building);

            Building replacedBuilding = RBTree_Insert(getBuildingTree(), Building_GetPoint(building), building);
            if (replacedBuilding != NULL)
                Building_Destroy(replacedBuilding);

        } else if (strcmp(type, "mur") == 0) {
            double x1, y1, x2, y2;
            sscanf(buffer + 3, "%lf %lf %lf %lf", &x1, &y1, &x2, &y2);

            Wall wall = Wall_Create(x1, y1, x2, y2);

            Wall replacedWall = RBTree_Insert(getWallTree(), Wall_GetPoint1(wall), wall);
            if (replacedWall != NULL)
                Wall_Destroy(wall);
        }
    }
    return true;
}

bool processPeople(FILE *pmFile) {
    char buffer[128];
    while (fgets(buffer, 100, pmFile) != NULL) {
        char type[16];
        sscanf(buffer, "%15s", type);

        if (strcmp(type, "p") == 0) {
            char cpf[16], name[32], surname[32], sex, birthDate[16];
            sscanf(buffer + 2, "%s %s %s %c %s", cpf, name, surname, &sex, birthDate);

            Person person = Person_Create(cpf, name, surname, sex, birthDate);

            Person replaced = HashTable_Insert(getPersonTable(), Person_GetCpf(person), person);
            if (replaced != NULL) {
                Building building = Person_GetBuilding(replaced);
                if (building != NULL)
                    Building_RemoveResident(building, replaced);
                Block block = Person_GetBlock(replaced);
                if (block != NULL) {
                    Block_RemoveResident(block, replaced);
                }
                Person_Destroy(replaced);
            }
        } else if (strcmp(type, "m") == 0) {
            char cpf[16], cep[24], face, complement[16];
            int num;
            sscanf(buffer + 1, "%s %s %c %d %s", cpf, cep, &face, &num, complement);

            Person person = HashTable_Find(getPersonTable(), cpf);
            if (person == NULL) {
                printf("Erro: Pessoa de CPF %s não encontrada!\n", cpf);
            }

            char address[64];
            Building_MakeAddress(address, cep, face, num);

            Block block = HashTable_Find(getBlockTable(), cep);
            if (block == NULL) {
                printf("Erro: Quadra de CEP %s não encontrada!\n", cep);
                continue;
            }

            Person_SetBlock(person, block);
            Person_SetAddress(person, address);
            Person_SetComplement(person, complement);

            RBTree buildings = Block_GetBuildings(block);
            Building building = RBTree_Find(buildings, address);
            if (building != NULL) {
                Building_InsertResident(building, person);
                Person_SetBuilding(person, building);
            }

            Block_InsertResident(block, person);
        }
    }
}

bool processCommerces(FILE *ecFile) {
    char buffer[128];
    while (fgets(buffer, 100, ecFile) != NULL) {
        char type[16];
        sscanf(buffer, "%15s", type);

        if (strcmp(type, "t") == 0) {
            char codt[16], desc[64];
            sscanf(buffer + 2, "%s %63[^\n]", codt, desc);

            CommerceType commType = CommerceType_Create(codt, desc);

            CommerceType replaced = HashTable_Insert(getCommTypeTable(), 
                                                     CommerceType_GetCode(commType), 
                                                     commType);
            if (replaced != NULL)
                CommerceType_Destroy(replaced);

        } else if (strcmp(type, "e") == 0) {
            char cnpj[24], cpf[16], codt[16], cep[24], face, name[64];
            int num;

            sscanf(buffer + 2, "%s %s %s %s %c %d %63[^\n]", cnpj, cpf, codt, cep, &face, &num, name);
            
            CommerceType cType = HashTable_Find(getCommTypeTable(), codt);
            if (cType == NULL) {
                printf("Erro: Tipo de estabelecimento não encontrado: %s\n", codt);
                continue;
            }

            char address[64];
            Building_MakeAddress(address, cep, face, num);

            Block block = HashTable_Find(getBlockTable(), cep);
            if (block == NULL) {
                printf("Erro: Quadra de CEP %s não encontrada!\n", cep);
                continue;
            }

            RBTree buildings = Block_GetBuildings(block);
            Building building = RBTree_Find(buildings, address);

            Person person = HashTable_Find(getPersonTable(), cpf);
            if (person == NULL) {
                printf("Erro: Pessoa de CPF %s não encontrada!\n", cpf);
            }

            Commerce commerce = Commerce_Create(cType, address, block, building, name, cnpj, person);
            Commerce replaced = HashTable_Insert(getCommerceTable(), Commerce_GetCnpj(commerce), commerce);
            if (replaced != NULL) {
                if (Commerce_GetBuilding(replaced) != NULL)
                    Building_RemoveCommerce(Commerce_GetBuilding(replaced), replaced);
                Block_RemoveCommerce(Commerce_GetBlock(replaced), replaced);
                Commerce_Destroy(replaced);
            }

            if (building != NULL)
                Building_InsertCommerce(building, commerce);
            Block_InsertCommerce(block, commerce);
        }
    }
}

bool processStreets(FILE *viaFile) {
    char buffer[128];
    while (fgets(buffer, 100, viaFile) != NULL) {
        char type[16];
        sscanf(buffer, "%15s", type);

        if (strcmp(type, "v") == 0) {
            char id[32];
            double x, y;

            sscanf(buffer + 2, "%s %lf %lf", id, &x, &y);

            GraphNode node = GraphNode_Create(id, x, y);

            RBTree_Insert(getNodeTree(), GraphNode_GetPoint(node), node);
            GraphNode replaced = HashTable_Insert(getNodeTable(), GraphNode_GetId(node), node);
            if (replaced != NULL) {
                GraphNode_Destroy(replaced);
            }
        } else if (strcmp(type, "e") == 0) {
            char i[32], j[32], cepLeft[24], cepRight[24], name[64];
            double length, speed;

            sscanf(buffer + 2, "%s %s %s %s %lf %lf %s", 
                    i, j, cepRight, cepLeft, &length, &speed, name);

            GraphNode node1 = HashTable_Find(getNodeTable(), i);
            if (node1 == NULL) {
                printf("Vértice não encontrado: %s!\n", i);
                continue;
            }
            GraphNode node2 = HashTable_Find(getNodeTable(), j);
            if (node1 == NULL) {
                printf("Vértice não encontrado: %s!\n", j);
                continue;
            }

            Block blockLeft = HashTable_Find(getBlockTable(), cepLeft);
            Block blockRight = HashTable_Find(getBlockTable(), cepRight);

            GraphNode_InsertEdge(node1, node2, blockLeft, blockRight, length, speed, name);
        }
    }
}

bool processQuery(FILE *queryFile, FILE *outputFile, FILE *txtFile, char baseDir[], char outputDir[], 
                  char svgFileName[], PathFindMode pathMode, PathStack *pathStack) {
    Point registries[11];
    memset(registries, 0, 11 * sizeof(Point));
    
    char buffer[128];
    while (fgets(buffer, 128, queryFile) != NULL) {

        int len = strlen(buffer);
        if (buffer[len - 1] != '\n') {
            buffer[len] = '\n';
            buffer[len + 1] = '\0';
        }

        char type[16];
        sscanf(buffer, "%15s", type);
        if (strcmp(type, "o?") == 0) {

            char idA[8], idB[8];
            sscanf(buffer + 3, "%7s %7s", idA, idB);
            
            fputs(buffer, txtFile);
            if (!Query_Overlaps(txtFile, outputFile, idA, idB))
                return false;

        } else if (strcmp(type, "i?") == 0) {
            
            char id[8];
            double x, y;
            sscanf(buffer + 3, "%7s %lf %lf", id, &x, &y);

            fputs(buffer, txtFile);
            if (!Query_Inside(txtFile, outputFile, id, x, y))
                return false;

        } else if (strcmp(type, "d?") == 0) {

            char j[8], k[8];
            sscanf(buffer + 3, "%7s %7s", j, k);
            
            if (!Query_Distance(txtFile, outputFile, j, k))
                return false;

        } else if (strcmp(type, "bb") == 0) {

            char suffix[32], color[16];
            sscanf(buffer + 3, "%s %s", suffix, color);

            if (!Query_Bb(txtFile, outputFile, outputDir, svgFileName, suffix, color))
                return false;
        
        } else if (strcmp(type, "dq") == 0) {

            char metric[8], id[16];
            double dist;
            sscanf(buffer + 3, "%7s %15s %lf", metric, id, &dist);

            fputs(buffer, txtFile);
            if (!Query_Dq(txtFile, metric, id, dist))
                return false;
        
        } else if (strcmp(type, "del") == 0) {

            char id[16];
            sscanf(buffer + 4, "%15s", id);

            fputs(buffer, txtFile);
            if (!Query_Del(txtFile, id))
                return false;
        
        } else if (strcmp(type, "cbq") == 0) {

            double x, y, r;
            char cStrk[24];
            sscanf(buffer + 4, "%lf %lf %lf %23s", &x, &y, &r, cStrk);

            fputs(buffer, txtFile);
            if (!Query_Cbq(txtFile, x, y, r, cStrk))
                return false;
        
        } else if (strcmp(type, "crd?") == 0) {

            char id[16];
            sscanf(buffer + 5, "%15s", id);

            fputs(buffer, txtFile);
            if (!Query_Crd(txtFile, id))
                return false;

        } else if (strcmp(type, "trns") == 0) {

            double x, y, w, h, dx, dy;
            sscanf(buffer + 5, "%lf %lf %lf %lf %lf %lf", 
                   &x, &y, &w, &h, &dx, &dy);

            fputs(buffer, txtFile);
            if (!Query_Trns(txtFile, x, y, w, h, dx, dy))
                return false;

        } else if (strcmp(type, "brl") == 0) {

            double x, y;
            sscanf(buffer + 4, "%lf %lf", &x, &y);

            if (!Query_Brl(outputFile, x, y))
                return false;

        } else if (strcmp(type, "fi") == 0) {

            double x, y, r;
            int ns;

            sscanf(buffer + 3, "%lf %lf %d %lf", &x, &y, &ns, &r);

            fputs(buffer, txtFile);
            if (!Query_Fi(txtFile, outputFile, x, y, ns, r))
                return false;

        } else if (strcmp(type, "fh") == 0) {

            char signal, cep[16], face;
            double num;
            int k;

            sscanf(buffer + 3, "%c%d %15s %c %lf", &signal, &k, cep, &face, &num);

            fputs(buffer, txtFile);
            if (!Query_Fh(txtFile, outputFile, signal, k, cep, face, num))
                return false;

        } else if (strcmp(type, "fs") == 0) {

            char cep[16], face;
            double num;
            int k;

            sscanf(buffer + 3, "%d %15s %c %lf", &k, cep, &face, &num);

            fputs(buffer, txtFile);
            if (!Query_Fs(txtFile, outputFile, k, cep, face, num))
                return false;

        } else if (strcmp(type, "brn") == 0) {

            char arqPol[32];
            double x, y;

            sscanf(buffer + 4, "%lf %lf %31[^\n]", &x, &y, arqPol);

            fputs(buffer, txtFile);
            Query_Brn(txtFile, outputFile, x, y, outputDir, arqPol);

        } else if (strcmp(type, "m?") == 0) {

            char cep[16];

            sscanf(buffer + 3, "%s", cep);

            fputs(buffer, txtFile);
            if (!Query_M(txtFile, cep))
                return false;

        } else if (strcmp(type, "mplg?") == 0) {
            
            char arqPol[32];
            sscanf(buffer + 6, "%31[^\n]", arqPol);

            fputs(buffer, txtFile);
            Query_Mplg(txtFile, outputFile, baseDir, arqPol);

        } else if (strcmp(type, "dm?") == 0) {

            char cpf[16];
            sscanf(buffer + 4, "%s", cpf);

            fputs(buffer, txtFile);
            if (!Query_Dm(txtFile, cpf))
                return false;

        } else if (strcmp(type, "de?") == 0) {

            char cnpj[24];
            sscanf(buffer + 4, "%s", cnpj);

            fputs(buffer, txtFile);
            if (!Query_De(txtFile, cnpj))
                return false;

        } else if (strcmp(type, "mud") == 0) {

            char cpf[16], cep[24], face, compl[16];
            int num;
            sscanf(buffer + 4, "%s %s %c %d %s", cpf, cep, &face, &num, compl);

            fputs(buffer, txtFile);
            if (!Query_Mud(txtFile, cpf, cep, face, num, compl))
                return false;

        } else if (strcmp(type, "eplg?") == 0) {

            char arqPolig[32], commType[16];
            sscanf(buffer + 6, "%s %s", arqPolig, commType);

            fputs(buffer, txtFile);
            Query_Eplg(txtFile, outputFile, baseDir, arqPolig, commType);

        } else if (strcmp(type, "catac") == 0) {
            
            char arqPolig[32];
            sscanf(buffer + 6, "%31[^\n]", arqPolig);

            fputs(buffer, txtFile);
            Query_Catac(outputFile, txtFile, baseDir, arqPolig);

        } else if (strcmp(type, "dmprbt") == 0) {

            char t, arq[32];

            sscanf(buffer + 7, "%c %31[^\n]", &t, arq);

            Query_Dmprbt(outputDir, t, arq);
        } else if (strcmp(type, "@m?") == 0) {
            int r;
            char cpf[16];

            sscanf(buffer + 4, "R%d %s", &r, cpf);

            Person person = HashTable_Find(getPersonTable(), cpf);
            if (person == NULL) {
                printf("Pessoa não encontrada: %s!\n", cpf);
                continue;
            }

            char cep[24], face;
            int num;
            sscanf(Person_GetAddress(person), "%s %c %d", cep, &face, &num);

            Block block = HashTable_Find(getBlockTable(), cep);
            if (block == NULL) {
                printf("Quadra não encontrada: %s!\n", cep);
                continue;
            }

            double x, y;
            Block_CalculateAddress(block, face, num, &x, &y);

            if (registries[r] != NULL)
                Point_Destroy(registries[r]);

            registries[r] = Point_Create(x, y);
        } else if (strcmp(type, "@e?") == 0) {
            char cep[24], face;
            int r, num;

            sscanf(buffer + 4, "R%d %s %c %d", &r, cep, &face, &num);

            Block block = HashTable_Find(getBlockTable(), cep);
            if (block == NULL) {
                printf("Quadra não encontrada: %s!\n", cep);
                continue;
            }

            double x, y;
            Block_CalculateAddress(block, face, num, &x, &y);

            if (registries[r] != NULL)
                Point_Destroy(registries[r]);

            registries[r] = Point_Create(x, y);
        } else if (strcmp(type, "@g?") == 0) {
            int r;
            char id[24];

            sscanf(buffer + 4, "R%d %s", &r, id);

            Equip e;
            e = HashTable_Find(getHydTable(), id);
            if (e == NULL)
                e = HashTable_Find(getTLightTable(), id);
            if (e == NULL)
                e = HashTable_Find(getCTowerTable(), id);
            if (e == NULL) {
                printf("Equipamento não encontrado: %s!\n", id);
                continue;
            }

            if (registries[r] != NULL)
                Point_Destroy(registries[r]);

            registries[r] = Point_Create(Equip_GetX(e), Equip_GetY(e));
        } else if (strcmp(type, "@xy") == 0) {
            int r;
            double x, y;

            sscanf(buffer + 4, "R%d %lf %lf", &r, &x, &y);

            if (registries[r] != NULL)
                Point_Destroy(registries[r]);

            registries[r] = Point_Create(x, y);
        } else if (strcmp(type, "p?") == 0) {
            char sufixo[32], cormc[24], cormr[24];
            int r1, r2;

            sscanf(buffer + 3, "%s R%d R%d %s %s", sufixo, &r1, &r2, cormc, cormr);

            if (registries[r1] == NULL) {
                printf("Registrador não definido: R%d!\n", r1);
                continue;
            } else if (registries[r2] == NULL) {
                printf("Registrador não definido: R%d!\n", r2);
                continue;
            }

            char nameWithSuffix[128];
            strcpy(nameWithSuffix, svgFileName);
            addSuffix(nameWithSuffix, sufixo);
            FILE *file = openFile(outputDir, nameWithSuffix, "w");
            if (file == NULL) {
                return true;
            }

            fputs(buffer, txtFile);

            putSVGStart(file);
            writeSVG(file, false);
            PathStack s = findPath(registries[r1], registries[r2], file, txtFile, cormc, cormr, pathMode);
            if (pathMode != ALL)
                *pathStack = s;
            putSVGEnd(file);
            fclose(file);
        }
    }

    for (int i = 0; i < 11; i++) {
        if (registries[i] != NULL)
            Point_Destroy(registries[i]);
    }

    return true;
}