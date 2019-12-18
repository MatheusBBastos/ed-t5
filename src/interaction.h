#ifndef INTERACTION_H
#define INTERACTION_H

#include <stdio.h>
#include <string.h>

#include "modules/util/files.h"
#include "commands.h"
#include "gui.h"

void startInteraction(Files *files, char *baseDir, char *entryFileName);

#endif