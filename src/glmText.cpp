//
//  glmLables.cpp
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/22/14.
//
//

#include "glmText.h"

using namespace glm;

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
    
    FTBBox box = font->BBox( &content[0], 1);
    bBox.set(box.Lower().Xf(), box.Lower().Yf(), box.Upper().Xf(), box.Upper().Yf());
}

glmRectangle glmText::getBoundingBox(){
    return bBox;
}

void glmText::drawOnLine(const glmPolyline &_polyline, double _offsetPct, double _hOffsetPct, bool _twoD ){
    float width = _polyline.getLength()*_offsetPct;
    
    for (int i = 0; i < content.length(); i++) {
        
//        while ( _polyline.getFractAt(width,10.) > 0.2) {
//            width += 1.;
//        }
        
        glm::vec3 src = _polyline.getPositionAt(width);
        double rot = _polyline.getAngleAt(width);
        
        glPushMatrix();
        glTranslated(src.x, src.y, src.z+2.);
        
        if(_twoD){
            glScalef(1,-1,1);
            glRotated(rot*RAD_TO_DEG, 0, 0, -1);
        } else {
            glRotated(rot*RAD_TO_DEG, 0, 0, 1);
        }
        glTranslatef(0., -bBox.height*_hOffsetPct,0.);
        font->Render( &content[i] , 1);
        glPopMatrix();
        width += letters_width[i];
    }
};