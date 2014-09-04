//
//  LineLables.h
//
//  Created by Patricio Gonzalez Vivo on 8/22/14.
//
//

#pragma once

#include "glmTools.h"

#include "ftgl.h"
#include "glmPolyline.h"
#include "glmRectangle.h"

enum LabelType {
    LABEL_POINT,
    LABEL_LINE,
    LABEL_AREA
};

class glmText{
public:
    
    glmText();
    virtual ~glmText();
    
    void setFont(FTFont *_font);
    void setText(const std::string &_text);
    
    glmRectangle getBoundingBox();
    
    void drawOnRectangle(const glmRectangle &_rectangle);
    void drawOnPosition(const glm::vec3 &_pos, const float &_margin = 5.0 ,const double &_angle = QUARTER_PI );
    void drawOnLine(const glmPolyline &_polyline, double _tryOffsetPct = 0., double _hOffsetPct = 0., bool _twoD = true);
    
private:
    void updateCache();

    std::vector<float> words_width;
    std::vector<float> letters_width;
    
    std::string text;
    
    glmRectangle bBox;
    
    FTFont *font;
};