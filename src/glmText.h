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
#include "glmRectangle.h"

class glmText{
public:
    
    glmText(){};
    void set(FTFont *_font, std::string _text);
    
    glmRectangle getBoundingBox();
    
    void drawOnLine(const glmPolyline &_polyline, double _lineOffsetPct = 0., double _hOffsetPct = 0., bool _twoD = true);
    void drawOnRectangle(const glmRectangle &_rectangle);
    
private:
    FTFont *font;
    
    glmRectangle bBox;
    
    std::vector<float> letters_width;
    std::vector<float> words_width;
    std::string content;
};