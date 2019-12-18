#ifndef GUI_H
#define GUI_H

#ifdef CURSES_ON

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>
#include "modules/util/files.h"
#include "commands.h"
#include "modules/aux/point.h"

void startGui(Files *files, char *baseDir, char *entryFileName);

#endif

#endif