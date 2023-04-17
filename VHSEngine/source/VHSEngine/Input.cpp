#include "VHSEngine/Input.h"
#include "VHSEngine/Game.h"
#include <iostream>

using namespace std;

Input::Input()
{
	//assign the current keys by default
	KeyBoardState = SDL_GetKeyboardState(NULL);
	/*MouseXDelta = MouseYDelta = 0;*/
	ScrollDelta = 0;
	MouseX = MouseY = 0;
}

void Input::ProcessInput()
{
	//reset the delta each frame
	MouseXDelta = MouseYDelta = 0;

	SDL_Event PollEvent;

	//this is waiting for inputs to be pressed
	while (SDL_PollEvent(&PollEvent)) {
		//checking what input was pressed
		switch (PollEvent.type) {
		case SDL_MOUSEBUTTONDOWN:
			//PollEvent.button.button gets the index of the mouse button pressed
			SetMouseButtonState(PollEvent.button.button, false);
			break;
		case SDL_MOUSEBUTTONUP:
			SetMouseButtonState(PollEvent.button.button, true);
			break;
		case SDL_MOUSEMOTION:
			OnMouseMove(PollEvent.motion);
			break;
		case SDL_MOUSEWHEEL:
			OnMouseScroll(PollEvent.wheel);
			break;
		case SDL_KEYDOWN:
			KeyBoardState = SDL_GetKeyboardState(NULL);
			break;
		case SDL_KEYUP:
			KeyBoardState = SDL_GetKeyboardState(NULL);
			break;
		case SDL_QUIT: //on close button pressed
			Game::GetGameInstance().CloseApp();
			break;
		default:
			break;
		}
	}
}

bool Input::IsKeyDown(SDL_Scancode Key)
{
	if (KeyBoardState == nullptr) {
		
		cout << "No keyboard state detected." << endl;
		return false;
	}

	//return the result of the key
	return static_cast<bool>(KeyBoardState[Key]);
}

bool Input::IsMouseButtonDown(MouseButtons Button)
{
	return MouseButtonStates[Button];
}

void Input::SetMouseButtonState(Uint8 ButtonIndex, bool CurrentState)
{
	MouseButtonStates[ButtonIndex] = CurrentState;
}

void Input::OnMouseMove(SDL_MouseMotionEvent& MEvent)
{
	//the mouse postion
	MouseX = MEvent.x;
	MouseY = MEvent.y;

	//movement of the mouse based on the last positon
	//the relative movement
	MouseXDelta += MEvent.xrel;
	MouseYDelta += MEvent.yrel;
}

void Input::OnMouseScroll(SDL_MouseWheelEvent& MEvent)
{
	//increase the relative to how much was scrolled 
	ScrollDelta += MEvent.preciseY;
}
