#ifndef HGMACRODATA_H
#define HGMACRODATA_H

#include <string>

typedef struct tagRect2D{
    int x1;
    int y1;
    int x2;
    int y2;
    tagRect2D(int x1,int y1,int x2,int y2){
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
    }
} HGRect2D,*PHGRect2D;

typedef struct tagImg2D{
    const unsigned char *data;
    int width;
    int height;
    int type;
    int channels;
    int steps;
} HGImg2D, *PHGImg2D;

typedef struct tagMatchResult2D{
    // int offsetx;
    // int offsety;
    // int width;
    // int height;
    std::string name; 
    float score;
    HGImg2D dst;
    bool flag;
} MatchResult2D;

#define SAFE_DELETE(ptr) if (ptr) {delete ptr; ptr = NULL;}


#endif // HGMACRODATA_H
