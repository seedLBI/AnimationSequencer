#ifndef TEXT_CREATOR_H
#define TEXT_CREATOR_H

#include "imgui.h"
#include "AnimationDrawer/AnimationDrawer.h"

#include "Utils/Text/Encoding/Unicode/Utils.Encoding.Unicode.h"
#include "Utils/Text/Encoding/UTF8/Utils.Encoding.UTF8.h"

#include "Engine/Engine.h"
#include "RenderObject/OpenGL.Quad.SymbolMSDF.Instance.h"

class TextCreator
{
public:
	TextCreator(AnimationDrawer* animationDrawer);
	~TextCreator();

	void Draw();

private:
	AnimationDrawer* animationDrawer = nullptr;
    Font* fontMSDF = nullptr;
    std::vector< SymbolMSDF_Data*> symbols_temp;

	void DeleteLastSymbols();
};


#endif //!TEXT_CREATOR_H