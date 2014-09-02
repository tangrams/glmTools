//
//  glmPointLabel.h
//
//  Created by Patricio Gonzalez Vivo on 9/2/14.
//
//

#pragma once

#include "glmTools.h"

#include "glmRectangle.h"

class glmPointLabel : public glm::vec3 {
public:
    
    glmPointLabel(){};
    glmPointLabel(float _x, float _y, float _text_width);
    
    bool compute(glmPointLabel &_other);
    
    void update();
    
    void draw(bool _bDebug);
    
    glmRectangle label;
    double      angle;
    float       margin;
    
private:
    float       area;
};