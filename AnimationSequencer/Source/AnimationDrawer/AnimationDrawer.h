#ifndef ANIMATION_DRAWER_H
#define ANIMATION_DRAWER_H

#include "Engine/Engine.h"
#include "Shaders/Shader.h"
#include "Font/Font.h"
#include "RenderObject/OpenGL.Quad.SymbolMSDF.Instance.h"
#include "CurveBezier/ImGui/CurveArrayEditor.h"
#include "UBO/UBO.h"
#include "UBO/DataUBO.h"
#include "Framebuffer/Framebuffer.h"

class AnimationDrawer
{
public:
	AnimationDrawer(Engine* engine);
	~AnimationDrawer();

	void Render();
	void Update();

	void SetTime(const float& value);
	void SetPrevTime(const float& value);

	void SetSmothness(const float& value);
	void SetTargetFPS(const int& value);

	glm::vec2 GetVirtualScreenSize();
	void SetVirtualScreenSize(const glm::vec2& size);


	void SetClearColor(const glm::vec3 color);

	Texture* GetVelocityTexture();
	Texture* GetImageTexture();
	Texture* GetFinalTexture();


	Quad_SymbolMSDF_instance* Get_ptr_Quad_SymbolMSDF_instance();
	Font* GetMSDF_Font();

private:

	float time = 0.f;
	float prev_time = 0.f;

	float smoothness = 0.2f;
	float targetFPS = 120.f;


	glm::vec2 VirtualScreenSize = glm::vec2(1280,720);

	glm::vec3 ClearColor = glm::vec3(0.f,0.f,0.f);

	DataBlockShader       UBO_data;
	glm::mat4 projection, view;
	glm::vec4 Times;
	UBO<DataBlockShader>* UBO_object;


	Engine* engine;

	Font* MSDF_Font = nullptr;
	Quad_SymbolMSDF_instance* SymbolsDrawer = nullptr;
	Quad ScreenQuad;


    Shader* MSDF_Shader;
	Shader* Velocity_Shader;
	Shader* Final_Shader;

	Texture* velocityTexture, *imageTexture, *finalTexture;
	Framebuffer *velocityFramebuffer, *imageFramebuffer, *finalFramebuffer;

	void LoadShaders();
	void Load_MSDF_Font();
	void InitRenderTextures();
	void BindRenderTextureToShader();
	void InitFramebuffers();
	void InitUBO();

	void UpdateUBO();

	void RenderVelocity();
	void RenderImage();
	void RenderFinalImage();
};






#endif