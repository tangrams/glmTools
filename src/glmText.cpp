//
//  glmLables.cpp
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/22/14.
//
//

#include "glmText.h"

void glmText::set(FTFont *_font, std::string _text){
    
    font = _font;
    
    content = _text;
    letters_width.clear();
    words_width.clear();
    
    std::string word = "";
    double word_width = 0;
    
    for(int i = 0; i < _text.size(); i++){
        FTBBox bbox = _font->BBox( &_text[i], 1);
        
        if( _text[i] == ' '){
            letters_width.push_back(_font->BBox( "A", 1).Upper().Xf());
            words_width.push_back(word_width);
            word_width = 0.;
            word = "";
        } else {
            float char_width = bbox.Upper().Xf();
            letters_width.push_back(char_width);
            word_width += char_width;
            word += &_text[i];
        }
    }
    
    FTBBox box = font->BBox(content.c_str(), -1, FTPoint());
    bBox.set(box.Lower().Xf(), box.Lower().Yf(), box.Upper().Xf(), box.Upper().Yf());
}

glmRectangle glmText::getBoundingBox(){
    return bBox;
}

void glmText::drawOnRectangle(const glmRectangle &_rectangle){
    glPushMatrix();
    glm::vec3 anchorpoint = _rectangle.getBottomLeft();
    glTranslatef(anchorpoint.x, anchorpoint.y, anchorpoint.z);
    glScaled(1, -1, 1);
    font->Render( &content[0], -1);
    glPopMatrix();
}

void glmText::drawOnLine(const glmPolyline &_polyline, double _offsetPct, double _hOffsetPct, bool _twoD ){
    
    float lineLength = _polyline.getLength();
    
    if(lineLength<0.0){
        return;
    }

    if( bBox.width < lineLength && _polyline.size() > 1.0){   //  Does the text actually fits on line???
        
        //  Ancher point
        //
        float offsetPct = _offsetPct;
        float labelOffsetPct = _offsetPct;
        while (lineLength*offsetPct - bBox.width*_offsetPct + bBox.width > lineLength) {
            offsetPct -= 0.01;
        }
        
        float offset = lineLength*offsetPct-bBox.width*_offsetPct;
        if(offset<0.0|| offset > lineLength){
            return;
        }
        
        //  Orientation
        //
        float angle = PI;
        if(_twoD){
            glm::vec3 diff = _polyline[0]-_polyline[_polyline.size()-1];
            angle = atan2f(-diff.y, diff.x);
        }
        
        if(angle < PI*0.5 && angle > -PI*0.5){
            for (int i = content.length()-1; i >=0 ; i--) {
                
                glm::vec3 src = _polyline.getPositionAt(offset);
                double rot = _polyline.getAngleAt(offset);
                
                glPushMatrix();
                glTranslated(src.x, src.y, src.z);
                
                if(_twoD){
                    glScalef(1,-1,1);
                    glRotated(rot*RAD_TO_DEG, 0, 0, -1);
                } else {
                    glTranslated(0.,0.,2.);
                    glRotated(rot*RAD_TO_DEG, 0, 0, 1);
                }
                
                glScaled(-1, -1, 1);
                glTranslated(-letters_width[i], 0, 0);
                
                glTranslatef(0., -bBox.height*_hOffsetPct,0.);
                font->Render( &content[i] , 1);
                glPopMatrix();
                offset += letters_width[i];
            }
        } else {
            for (int i = 0; i < content.length(); i++) {
                
                glm::vec3 src = _polyline.getPositionAt(offset);
                double rot = _polyline.getAngleAt(offset);
                
                glPushMatrix();
                glTranslated(src.x, src.y, src.z);
                
                if(_twoD){
                    glScalef(1,-1,1);
                    glRotated(rot*RAD_TO_DEG, 0, 0, -1);
                } else {
                    glTranslated(0.,0.,2.);
                    glRotated(rot*RAD_TO_DEG, 0, 0, 1);
                }
                
                glTranslatef(0., -bBox.height*_hOffsetPct,0.);
                font->Render( &content[i] , 1);
                glPopMatrix();
                offset += letters_width[i];
            }
        }
    } else {
        // TODO: what happen if don't fit ??
    }
};

void glmText::drawOnPosition(const glm::vec3 &_pos, const float &_margin, const double &_angle ){
    glmRectangle label = bBox;
    
    label.x = _pos.x + _margin * cos(_angle);
    label.y = _pos.y + _margin * sin(-_angle);
    
    if (_angle <= QUARTER_PI && _angle >= -QUARTER_PI){
        //  EAST
        //
        label.x += 0.0;
        label.y += mapValue(_angle,
                            QUARTER_PI,     -QUARTER_PI,
                            -label.height,  0);
        
    } else if (_angle >= QUARTER_PI && _angle <= QUARTER_PI*3.){
        //  NORTH
        //
        label.x += mapValue(_angle,
                            QUARTER_PI*3.,  QUARTER_PI,
                            -label.width,   0);
        label.y += -label.height;
    } else if (_angle <= -QUARTER_PI && _angle >= -QUARTER_PI*3.){
        //  SOUTH
        //
        label.x += mapValue(_angle,
                            -QUARTER_PI*3., -QUARTER_PI,
                            -label.width,   0);
        label.y += 0.0;
    } else if (_angle > QUARTER_PI*3. || _angle < -QUARTER_PI*3. ){
        //  WEST
        //
        label.x -= label.width;
        
        if(_angle > 0){
            label.y += mapValue(_angle,
                                QUARTER_PI*3., PI,
                                -label.height, -label.height*0.5);
        } else {
            label.y += mapValue(_angle,
                                -PI, -QUARTER_PI*3.,
                                -label.height*0.5,0.0);
        }
    }
    
    drawOnRectangle(label);
}