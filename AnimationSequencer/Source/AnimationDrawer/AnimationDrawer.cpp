#include "AnimationDrawer.h"


Texture* AnimationDrawer::GetVelocityTexture() {
	return velocityTexture;
}
Texture* AnimationDrawer::GetImageTexture() {
	return imageTexture;
}
Texture* AnimationDrawer::GetFinalTexture() {
	return finalTexture;
}

void AnimationDrawer::SetClearColor(const glm::vec3 color) {
	ClearColor = color;
}

glm::vec2 AnimationDrawer::GetVirtualScreenSize() {
	return VirtualScreenSize;
}
void AnimationDrawer::SetVirtualScreenSize(const glm::vec2& size) {
	VirtualScreenSize = size;
}
void AnimationDrawer::SetTime(const float& time) {
	this->time = time;
}
void AnimationDrawer::SetPrevTime(const float& value) {
	this->prev_time = value;
}

void AnimationDrawer::SetSmothness(const float& value) {
	smoothness = value;

	MSDF_Shader->use();
	MSDF_Shader->setFloat("smoothness", smoothness);
}
void AnimationDrawer::SetTargetFPS(const int& value) {
	targetFPS = value;

	Final_Shader->use();
	Final_Shader->setFloat("targetFPS", targetFPS);
}

void AnimationDrawer::RenderVelocity() {
	velocityFramebuffer->Bind();
	Velocity_Shader->use();

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(BufferBit::COLOR_BUFFER_BIT | BufferBit::DEPTH_BUFFER_BIT);

	SymbolsDrawer->Draw();

}

void AnimationDrawer::RenderImage() {
	imageFramebuffer->Bind();
	MSDF_Shader->use();

	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, 1.f);
	glClear(BufferBit::COLOR_BUFFER_BIT | BufferBit::DEPTH_BUFFER_BIT);

	SymbolsDrawer->Draw();
}

void AnimationDrawer::RenderFinalImage() {
	finalFramebuffer->Bind();

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(BufferBit::COLOR_BUFFER_BIT | BufferBit::DEPTH_BUFFER_BIT);

	Final_Shader->use();
	ScreenQuad.Draw();
}


void AnimationDrawer::Render() {
	RenderVelocity();
	RenderImage();
	RenderFinalImage();

	Framebuffer::UnBind();
}
void AnimationDrawer::Update() {
	SymbolsDrawer->Update(prev_time);
	SymbolsDrawer->Update(time);
	UpdateUBO();
}


void AnimationDrawer::UpdateUBO() {

	view = glm::lookAt(
		glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f)
	);

	float aspect_ratio;

	if (VirtualScreenSize.x > VirtualScreenSize.y)
		aspect_ratio = VirtualScreenSize.y / VirtualScreenSize.x;
	else
		aspect_ratio = VirtualScreenSize.x / VirtualScreenSize.y;

	projection = glm::ortho(
		-1.f,
		 1.f,
		-1.f * aspect_ratio,
		 1.f * aspect_ratio,
		-100.f,
		 100.f
	);

	/*
	projection = glm::ortho(
		-(VirtualScreenSize.x), VirtualScreenSize.x,
		-(VirtualScreenSize.y), (VirtualScreenSize.y),
		-1000.0f, 1000.0f
	);
	*/


	UBO_data.view = view;
	UBO_data.projection = projection;

	UBO_data.times.x = glfwGetTime();
	UBO_data.times.y = engine->GetDeltaTime();
	UBO_data.times.z = VirtualScreenSize.x;
	UBO_data.times.w = VirtualScreenSize.y;

	UBO_object->Update(UBO_data);
}

void AnimationDrawer::InitUBO() {
	UBO_object = new UBO<DataBlockShader>();
	UBO_object->SetName("DataBlock");
	UBO_object->BindUBO(0);
	UBO_object->BindProgramm(*MSDF_Shader);
	UBO_object->BindProgramm(*Velocity_Shader);
	UBO_object->BindProgramm(*Final_Shader);
}

Quad_SymbolMSDF_instance* AnimationDrawer::Get_ptr_Quad_SymbolMSDF_instance() {
	return SymbolsDrawer;
}
Font* AnimationDrawer::GetMSDF_Font() {
	return MSDF_Font;
}

void AnimationDrawer::InitFramebuffers() {
	velocityFramebuffer = new Framebuffer(*velocityTexture);
	imageFramebuffer = new Framebuffer(*imageTexture);
	finalFramebuffer = new Framebuffer(*finalTexture);
}


void AnimationDrawer::BindRenderTextureToShader() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, MSDF_Font->GetTexture()->GetID());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, velocityTexture->GetID());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, imageTexture->GetID());
}

void AnimationDrawer::InitRenderTextures() {
	TextureSetting settingVelocity;
	settingVelocity.Min = TextureFilter::LINEAR;
	settingVelocity.Max = TextureFilter::LINEAR;
	settingVelocity.WrapX = TextureWrap::MIRRORED_REPEAT;
	settingVelocity.WrapY = TextureWrap::MIRRORED_REPEAT;

	glm::vec2 screenSize = engine->GetMonitorScreenSize();

	velocityTexture = new Texture(
		settingVelocity,
		0,
		TextureInternalFormat::RG16F,
		screenSize.x, screenSize.y,
		TextureFormat::RG,
		TextureType::_FLOAT,
		0
	);

	imageTexture = new Texture(
		settingVelocity,
		0,
		TextureInternalFormat::RGB8,
		screenSize.x, screenSize.y,
		TextureFormat::RGB,
		TextureType::_UNSIGNED_BYTE,
		0
	);

	finalTexture = new Texture(
		settingVelocity,
		0,
		TextureInternalFormat::RGB8,
		screenSize.x, screenSize.y,
		TextureFormat::RGB,
		TextureType::_UNSIGNED_BYTE,
		0
	);
}


void AnimationDrawer::LoadShaders() {
	MSDF_Shader = new Shader(
		std::string("Data/FontMSDF/Shaders/Symbol_MSDF_Instance.vert"),
		std::string("Data/FontMSDF/Shaders/Symbol_MSDF_Instance.frag")
	);

	Velocity_Shader = new Shader(
		std::string("Data/FontMSDF/Shaders/Symbol_MSDF_Velocity_Instance.vert"),
		std::string("Data/FontMSDF/Shaders/Symbol_MSDF_Velocity_Instance.frag")
	);

	Final_Shader = new Shader(
		std::string("Data/FontMSDF/Shaders/FinalFrameBuffer.vert"),
		std::string("Data/FontMSDF/Shaders/FinalFrameBuffer.frag")
	);

	Final_Shader->use();
		Final_Shader->setInt("texture_color", 2);
		Final_Shader->setInt("texture_velocity", 1);
		Final_Shader->setFloat("targetFPS", targetFPS);
		Final_Shader->setInt("MAX_SAMPLES", 32);


	MSDF_Shader->use();
		MSDF_Shader->setInt("Texture_MSDF_Atlas", 0);
		MSDF_Shader->setFloat("pxRange", MSDF_Font->GetMetrics().pxRange);
		MSDF_Shader->setFloat("smoothness", smoothness);
}


void AnimationDrawer::Load_MSDF_Font() {
	MSDF_Font = new Font(
		"Data/FontMSDF/Atlas.png",
		"Data/FontMSDF/DataAtlas.txt"
	);
}


AnimationDrawer::AnimationDrawer(Engine* engine) {
	this->engine = engine;

	Load_MSDF_Font();
	LoadShaders();
	InitRenderTextures();
	BindRenderTextureToShader();
	InitFramebuffers();
	InitUBO();

	SymbolsDrawer = new Quad_SymbolMSDF_instance(MSDF_Font);



}

AnimationDrawer::~AnimationDrawer()
{
}