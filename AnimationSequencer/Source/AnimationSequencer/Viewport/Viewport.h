#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "imgui.h"
#include "AnimationDrawer/AnimationDrawer.h"
#include "AnimationSequencer/AnimationSequencer.h"

class Viewport
{
public:
	Viewport(AnimationDrawer* animationDrawer);
	~Viewport();

	void Draw();

private:
    AnimationDrawer* animationDrawer = nullptr;

    bool DrawMSDFTexture = false;
    bool DrawVelocityTexture = false;
    bool DrawImageTexture = false;
    bool DrawFinalTexture = true;
};


#endif VIEWPORT_H