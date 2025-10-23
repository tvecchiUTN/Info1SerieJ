#include "getTime.h"
#include <string.h>

const char *dias[] = {"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};
const char *mes[] = {"Ene", "Feb", "Mar", "Abr", "May", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"};

void getCurrentTime(char* formatTime)
{
    time_t t = time(NULL);
    struct tm *pt = localtime(&t);

    char fecha[32];
    char hora[16];

    sprintf(fecha,"%s %d de %s de %d",dias[pt->tm_wday],pt->tm_mday,mes[pt->tm_mon],pt->tm_year+1900);
	sprintf(hora,"%02d:%02d:%02d",pt->tm_hour,pt->tm_min,pt->tm_sec);

    strcat(formatTime, fecha);
    strcat(formatTime, " ");
    strcat(formatTime, hora);
}