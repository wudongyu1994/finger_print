#ifndef __FINGER_H
#define __FINGER_H	 
#include "sys.h"
#include "as608.h"

bool Add_FR(void);
bool press_FR(SearchResult *seach);
void ShowErrMessage(u8 ensure);
bool checkFinger(SearchResult *search);
#endif
