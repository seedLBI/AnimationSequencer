#ifndef OPENGL_QUAD_SYMBOL_MSDF_INSTANCE_H
#define OPENGL_QUAD_SYMBOL_MSDF_INSTANCE_H

#include "OpenGL.Quad.h"
#include "Font/Font.h"
#include "DrawableObjects/SymbolMSDF.h"

class Quad_SymbolMSDF_instance : public Quad {
public:
	Quad_SymbolMSDF_instance(Font* font);
	~Quad_SymbolMSDF_instance();

	void AddSymbol(SymbolMSDF_Data* symbol);
	void DeleteSymbol(SymbolMSDF_Data* symbol);
	void Clear();

	std::vector<SymbolMSDF_Data*>* GetSymbols();
	void Update(const float& time);

	void Draw();
private:
	Font* font;
	std::vector<SymbolMSDF_Data*> symbols;
	GLuint VBO_Instanced;
	void Init();
};


#endif