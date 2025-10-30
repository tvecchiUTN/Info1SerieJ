#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <string.h>

#define OK 1
#define ERR 0
#define ERR_PUERTO -1
#define ERR_MISSING_ARGS -2

int checkPuerto(int argc, char **argv, int *puerto);

int invertir(const char* src, char* dest);

#endif