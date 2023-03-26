#pragma once

#include "SDL2/SDL.h"
#include "VHSEngine/CoreMinimal.h"
#include "VHSEngine/Mesh/Transformations.h"

class GraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();

	//create the window
	bool InitGE(const char* WTitle, bool bFullScreen, int WWidth, int WHeight);

	//show the newest frame graphics
	void PresentGraphics();

	//clear the old frame graphics
	void ClearGraphics();

	void Draw();

	//return the sdl window
	SDL_Window* GetWindow() const;

	//add a new VAO to the VAOStack using a shape define
	MeshPtr CreateSimpleMeshShape(GeometricShapes Shape, ShaderPtr MeshShader, TexturePtrStack MeshTextures);

	//add new Shader
	//@param 1 - vertex shader
	//@param 2 - fragment shader
	ShaderPtr CreateShader(VFShaderParams ShaderFilePaths);

	//create a texture and add ut to the stack
	//avoid duplicates
	TexturePtr CreateTexture(const char* FilePath);

	//Create a texture and add it to the texture pack
	//screen and camera coordinates
	void ApplyScreenTransformations(ShaderPtr Shader);

private:
	//this will hold the window
	SDL_Window* SdlWindow;
	//this will allow OpenGL to work with SDL
	SDL_GLContext SdlGLContext;

	//handle wireframe mode
	void HandleWireFrameMode(bool bShowWireframeMode);
	//set wireframe
	bool bWireframeMode;

	//single shader
	ShaderPtr Shader;
	//store a vector of textures
	TexturePtrStack TextureStack;
	//store all meshes in the game
	MeshPtrStack MeshStack;

public:
	//default camera
	CameraPtr EngineDefaultCam;
};;
