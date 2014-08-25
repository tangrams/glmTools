//
//  glmLables.cpp
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/22/14.
//
//

#include "glmLabel.h"

using namespace glm;

void glmText::set(FTFont *_font, string _text){
    
    content = _text;
    letters_width.clear();
    words_width.clear();
    
    string word = "";
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
}

void drawText(FTFont *font, const glmText & _text, const glmPolyline &_polyline, double _offsetPct ){
    float width = _polyline.getLength()*_offsetPct;
    
    for (int i = 0; i < _text.content.length(); i++) {
        
//        while ( _polyline.getFractAt(width,10.) > 0.2) {
//            width += 1.;
//        }
        
        vec2 src = _polyline.getPositionAt(width);
        double rot = _polyline.getAngleAt(width);
        
        glPushMatrix();
        glTranslated(src.x, src.y, 0.);
        glScalef(1,-1,1);
        glRotated(rot*RAD_TO_DEG, 0, 0, -1);
        font->Render( &_text.content[i] , 1);
        glPopMatrix();
        width += _text.letters_width[i];
    }
};