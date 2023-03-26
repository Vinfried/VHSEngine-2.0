#include "VHSEngine/Game.h"
#include "VHSEngine/Graphics/GraphicsEngine.h"
#include "VHSEngine/Graphics/Mesh.h"
#include "VHSEngine/Input.h"
#include "VHSEngine/Graphics/Camera.h"

Game& Game::GetGameInstance()
{
	static Game* GameInstance = new Game();
	return *GameInstance;
}

void Game::DestroyGameInstance()
{
	static Game* GameInstance = &GetGameInstance();
	delete GameInstance;
}

void Game::Start(const char* WTitle, bool bFullscreen, int WWidth, int WHeight)
{
	Graphics = make_shared<GraphicsEngine>();

	if (Graphics->InitGE(WTitle, bFullscreen, WWidth, WHeight)) {
		bIsGameOver = false;
	}

	Run();
}

Game::Game()
{
	cout << "Game Initialised" << endl;

	Graphics = nullptr;
	bIsGameOver = false;
	
}

Game::~Game()
{
	Graphics = nullptr;
	cout << "Game Over..." << endl;
}

void Game::Run()
{
	if (!bIsGameOver) {

		//create an input class to create input
		GameInput = new Input();

		ShaderPtr TextureShader = Graphics->CreateShader({
			L"Game/Shaders/TextureShader/TextureShader.svert",
			L"Game/Shaders/TextureShader/TextureShader.sfrag"
			});

		TexturePtr TConcrete = Graphics->CreateTexture("Game/Textures/GreySquare_S.jpg");
		TexturePtr TTech = Graphics->CreateTexture("Game/Textures/seamless-tech-texture.jpg");

		//create meshes
		Poly = Graphics->CreateSimpleMeshShape(GeometricShapes::Cube, TextureShader, {TConcrete});
		Poly2 = Graphics->CreateSimpleMeshShape(GeometricShapes::Cube, TextureShader, { TTech });


		Poly->Transform.Location = Vector3(0.0f, 0.0f, 0.0f);
		Poly2->Transform.Location = Vector3(0.0f, 0.0f, 0.0f);
	}

	while (!bIsGameOver) {
		//Make sure twe process what the user has done
		ProcessInput();
		
		//Apply the logic base on the inputs and the AI Logic
		Update();

		//Render the screen based on the 2 functions above
		Draw();
	}

	//Clean the game after it ends
	CloseGame();
}

void Game::Update()
{
	//set delta time first always
	static double LastFrameTime = 0.0;
	//set the current time since the game has passed
	double CurrentFrameTime = static_cast<double>(SDL_GetTicks64());
	// find the time difference between the last and current frame
	double NewDeltaTime = CurrentFrameTime - LastFrameTime;
	//set delta time as seconds
	DeltaTime = NewDeltaTime / 1000.0;
	//update the last frame time for the next update
	LastFrameTime = CurrentFrameTime;
	
	Poly->Transform.Rotation.x += 50.0f * GetFDeltaTime();
	Poly->Transform.Rotation.y += 50.0f * GetFDeltaTime();
	Poly->Transform.Rotation.z += 50.0f * GetFDeltaTime();

	Poly2->Transform.Rotation.x += -50.0f * GetFDeltaTime();
	Poly2->Transform.Rotation.y += -50.0f * GetFDeltaTime();
	Poly2->Transform.Rotation.z += -50.0f * GetFDeltaTime();

	Vector3 CameraInput = Vector3(0.0f);
	CDirections CamDirections = Graphics->EngineDefaultCam->GetDirections();

	//move camera forward
	if (GameInput->IsKeyDown(SDL_SCANCODE_W)) {
		CameraInput += CamDirections.Forward;
	}

	//move camera backward
	if (GameInput->IsKeyDown(SDL_SCANCODE_S)) {
		CameraInput += -CamDirections.Forward;
	}

	//move camera left
	if (GameInput->IsKeyDown(SDL_SCANCODE_A)) {
		CameraInput += -CamDirections.Right;
	}

	//move camera right
	if (GameInput->IsKeyDown(SDL_SCANCODE_D)) {
		CameraInput += CamDirections.Right;
	}

	//move camera up
	if (GameInput->IsKeyDown(SDL_SCANCODE_Q)) {
		CameraInput += -CamDirections.Up;
	}

	//move camera down
	if (GameInput->IsKeyDown(SDL_SCANCODE_E)) {
		CameraInput += CamDirections.Up;
	}

	float CameraSpeed = 1.0f;
	if (GameInput->IsKeyDown(SDL_SCANCODE_LSHIFT)) {
		CameraSpeed = 5.0f;
	}
	

	CameraInput *= CameraSpeed * GetFDeltaTime();

	Vector3 NewLocation = Graphics->EngineDefaultCam->GetTransform().Location += CameraInput;

	Graphics->EngineDefaultCam->Translate(NewLocation);

	if (GameInput->IsMouseButtonDown(MouseButtons::RIGHT)) {
		Graphics->EngineDefaultCam->RotatePitch(-GameInput->MouseYDelta * GetFDeltaTime() * 50.0f);
		Graphics->EngineDefaultCam->RotateYaw(GameInput->MouseXDelta * GetFDeltaTime() * 40.0f);
	}
}

void Game::ProcessInput()
{
	//Run the input detection for our game input
	GameInput->ProcessInput();
}

void Game::Draw()
{
	Graphics->Draw();
}

void Game::CloseGame()
{
	delete GameInput;
}
