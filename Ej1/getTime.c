#include "getTime.h"

const char *dias[] = {"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};
const char *mes[] = {"Ene", "Feb", "Mar", "Abr", "May", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"};

void getCurrentTime(char* fecha, char* hora)
{
    time_t t = time(NULL);
    struct tm *pt = localtime(&t);

    sprintf (fecha,"%s %d de %s de %d",dias[pt->tm_wday],pt->tm_mday,mes[pt->tm_mon],pt->tm_year+1900);
	sprintf (hora,"%02d:%02d:%02d",pt->tm_hour,pt->tm_min,pt->tm_sec);
}