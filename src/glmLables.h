//
//  LineLables.h
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/22/14.
//
//

#pragma once

#include "glmTools.h"

#include "ftgl.h"
#include "glmPolyline.h"

using namespace std;

class glmText{
public:
    
    glmText(){};
    void set(FTFont *_font, std::string _text);
    
    vector<float> letters_width;
    vector<float> words_width;
    string content;
};

void drawText(FTFont *font, const glmText & _text, const glmPolyline &_polyline, double _offsetPct = 0.);