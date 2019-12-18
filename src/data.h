#ifndef DATA_H
#define DATA_H

#include "modules/aux/graph_node.h"
#include "modules/data_structures/hash_table.h"
#include "modules/data_structures/redblack_tree.h"
#include "modules/sig/object.h"
#include "modules/sig/text.h"
#include "modules/sig/block.h"
#include "modules/sig/equipment.h"
#include "modules/sig/building.h"
#include "modules/sig/wall.h"
#include "modules/sig/commerce_type.h"

void initializeTrees();

void initializeTables();

void destroyTrees();

void destroyTables();

RBTree getObjTree();

HashTable getObjTable();

RBTree getTextTree();

RBTree getBlockTree();

HashTable getBlockTable();

RBTree getHydTree();

RBTree getCTowerTree();

RBTree getTLightTree();

RBTree getBuildingTree();

RBTree getWallTree();

RBTree getNodeTree();

HashTable getHydTable();

HashTable getCTowerTable();

HashTable getTLightTable();

HashTable getCommTypeTable();

HashTable getCommerceTable();

HashTable getPersonTable();

HashTable getNodeTable();

#endif