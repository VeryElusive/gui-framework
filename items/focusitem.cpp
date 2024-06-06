#include "../form.hpp"
#include "dropdown.hpp"
#include "checkbox.hpp"
#include "slider.hpp"

#include "../../input/mouse.hpp"
#include "../../input/keys.hpp"

#include "../../render/base.hpp"
#include "../../render/font/base.hpp"

using namespace Cheat;

void HandleDropDownAnimation(const bool Change, const bool Active, 
					const float TimeSinceLastFrame, 
					bool& Opened, float& Animation);

void RenderBindSelector(const int X, int& Y, 
					int& Key,
					const int GroupSizeX,
					const float DPIScale, 
					const float TimeSinceLastFrame,
					const float Alpha, const float Animation, 
					bool& Active, const bool AllowInput);

const char *GetKeyString(Input::KeyCodes Code);


void GUI::CFocusItem::Render(const float TimeSinceLastFrame, const float Alpha, 
                        const Color FormAccent, const float DPIScale)
{
	if(Animation <= 0.01f)
	{
		return;
	}
	
	if(Item != NULL)
	{
		auto PosY = PositionY;
		Item->Render(PositionX, PosY, 
				GroupSizeX, 0,
				KeybindSelector ? 0.f : Animation,
				TimeSinceLastFrame, Alpha, 
				FormAccent, DPIScale, 
				FALSE,
				FALSE, this);
	}

	if(!KeybindSelector)
	{
		return;
	}

	const int SizeX = (int)(300 * DPIScale);
	const int SizeY = (int)(120 * DPIScale);

	const auto RenderPosX = MousePositionX;
	GUI::RenderPopup(RenderPosX, MousePositionY, SizeX, SizeY, BACKGROUND_COLOR_MENU.ScaleAlpha(Animation), FormAccent.ScaleAlpha(Animation));
	
	const auto EscapeState 			= Input::GetKeyState(Input::KeyCodes::KEY_ESCAPE);
	const auto MouseLeftState 		= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);
	const auto Hovered 				= Input::IsMouseInBounds(RenderPosX, PositionY, SizeX, (int)(SizeY + (PADDING * DPIScale * 1.5f * 2)));//compensating for the mode selector dropdown extending out beyond
	
	if(FirstRun == FALSE && Hovered == FALSE 
		&& (MouseLeftState.Pressed == TRUE || EscapeState.Pressed == TRUE))
	{
		Active = FALSE;
	}

	if(KeybindSelector)
	{
		RenderKeybindSelector(SizeX, SizeY, TimeSinceLastFrame, Alpha, FormAccent, DPIScale);
	}

	if(FirstRun)
	{
		FirstRun = FALSE;
	}
}

void GUI::CFocusItem::RenderKeybindSelector(const int SizeX, const int SizeY,
						const float TimeSinceLastFrame, const float Alpha, 
                        const Color FormAccent, const float DPIScale)
{
	const auto Padding 	 = (int)(PADDING * DPIScale);
	auto RenderPositionX = MousePositionX + PADDING * 2;
	auto RenderPositionY = MousePositionY + Padding;

	Render::Font::Hack->DrawText(
		RenderPositionX, RenderPositionY - 1,
		"Keybind Selector", LibUtil_StringLength("Keybind Selector"),
		(SELECTED_ITEM_COLOR_MENU).ScaleAlpha(Animation).ToUInt(),
		Render::Font::FONT_SHADOW,
		0.7f * DPIScale
	);

	RenderPositionY += Padding * 3;


	static Color ValueColor = DIM_ITEM_COLOR_MENU;
	if(Animation < 0.1f)
	{
		ValueColor = DIM_ITEM_COLOR_MENU;
	}

	ValueColor = ValueColor.ScaleAlpha(Animation);

	const auto BackupValue = Item->Value;
	const auto BackupColor = Item->Color;

	static Color Col = DIM_ITEM_COLOR_MENU;
	Item->Color = Col;

	switch(Item->Type)
	{
	case EItemType::CHECKBOX:
		Item->Value = &Item->Keybind->ChangeValue.B;
		break;
	case EItemType::SLIDER:
		Item->Value = &Item->Keybind->ChangeValue.F;
		break;
	case EItemType::DROPDOWN:
		Item->Value = &Item->Keybind->ChangeValue.I;
		break;
	default:
		break;
	}

	static float ValueAnimation 	= 0.f;
	static bool Opened				= FALSE;
	float BackupInterpolatedValue	= 0.f;
	bool BackupSelected				= FALSE;

	if(Item->Type == EItemType::SLIDER)
	{
		BackupInterpolatedValue = ((CSlider *)Item)->InterpolatedValue;
		BackupSelected 			= ((CSlider *)Item)->Selected;

		((CSlider *)Item)->InterpolatedValue = ValueAnimation;
		((CSlider *)Item)->Selected 		 = Opened;
	}

	auto PosY = PositionY;
	const auto Change = Item->Render(
		RenderPositionX, RenderPositionY,
		SizeX, SizeY,
		ValueAnimation,
		TimeSinceLastFrame, Animation,
		FormAccent, DPIScale,
		false,
		Active, NULL);

	if(Item->Type == EItemType::SLIDER)
	{
		Opened 			= ((CSlider *)Item)->Selected;
		ValueAnimation 	= ((CSlider *)Item)->InterpolatedValue;

		ValueAnimation = LibUtil_LerpF(ANIMATION_SPEED, ValueAnimation, *(float*)Item->Value);

		((CSlider *)Item)->InterpolatedValue = BackupInterpolatedValue;
		((CSlider *)Item)->Selected 		 = BackupSelected;
	}
	else if(Item->Type == EItemType::DROPDOWN || Item->Type == EItemType::MULTISELECT)
	{
		HandleDropDownAnimation(Change, Active, TimeSinceLastFrame, Opened, ValueAnimation);
	}

	if(!Active)
	{
		Opened = FALSE;
	}

	Col = Item->Color;

	Item->Value = BackupValue;
	Item->Color = BackupColor;

	RenderBindSelector(RenderPositionX, RenderPositionY,
					Item->Keybind->Key, 
					SizeX, 
					DPIScale, TimeSinceLastFrame,
					Alpha, Animation,
					Active, Active && !Change);
	
	static CBaseItem Modes[] 		= {L"Toggle", L"Hold"};
	static Color TypeDropDownColor 	= DIM_ITEM_COLOR_MENU;
	static float DropDownAnimation 	= 0.f;
	static bool DropDownOpened 		= false;

	const auto TypeDropDownChangeStates = GUI::DrawDropDown(
		RenderPositionX, RenderPositionY, 
		SizeX, LibUtil_MinF(DropDownAnimation, Animation), 
		L"Mode", (int*)&Item->Keybind->Mode, 
		2, Modes,
		FALSE,
		TypeDropDownColor,
		TimeSinceLastFrame, Animation, 
		FormAccent, DPIScale, 
		Active && !Change, FALSE, 
		NULL, NULL
	);

	HandleDropDownAnimation(TypeDropDownChangeStates, Active, TimeSinceLastFrame, DropDownOpened, DropDownAnimation);
}

void RenderBindSelector(const int X, int& Y, 
					int& Key,
					const int GroupSizeX,
					const float DPIScale, 
					const float TimeSinceLastFrame,
					const float Alpha, const float Animation, 
					bool& Active, const bool AllowInput)
{
	bool Ret					= false;
	const auto SizeX 			= (GroupSizeX - PADDING * 4) / 3;
	const auto SizeY 			= (int)(ITEM_HEIGHT * 1.5f * DPIScale);
	const auto Padding 			= (int)(PADDING * DPIScale);

	static bool Selected;
	static Color Col(DIM_ITEM_COLOR_MENU);
	const auto Hovered 				= Input::IsMouseInBounds(X, Y, GroupSizeX, SizeY);
	const auto MouseLeftState 		= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);
	const auto EscapeState 			= Input::GetKeyState(Input::KeyCodes::KEY_ESCAPE);

	if(Selected)
	{
		Col = Col.Lerp(DIM_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}
	else if(Hovered)
	{
		Col = Col.Lerp(SELECTED_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}
	else 
	{
		Col = Col.Lerp(HOVERED_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}

	const auto BackupSelected = Selected;
	
	if(Hovered && AllowInput && MouseLeftState.Pressed && !MouseLeftState.Trapped)
	{
		Selected = TRUE;
	}

	if(!Active)
	{
		Selected = FALSE;
	}

	if(Selected && EscapeState.Pressed)
	{
		Selected = FALSE;
		Active 	 = TRUE;
		Key = -1;
	}

	if(Selected)
	{
		for(int i = Input::KeyCodes::KEY_FIRST; i < Input::KeyCodes::KEY_COUNT + Input::KeyCodes::MOUSE_COUNT; i++)
		{
			if(Input::GetKeyState((Input::KeyCodes)i).Pressed
				&& (i != Input::KeyCodes::MOUSE_LEFT
				|| BackupSelected == Selected))
			{
				Key = i;
				Selected = false;
				break;
			}
		}
	}

	Render::Font::Hack->DrawText(
		X, Y - 1,
		"Key", LibUtil_StringLength("Key"),
		(LABEL_COLOR_MENU).ScaleAlpha(Animation).ToUInt(),
		Render::Font::FONT_SHADOW,
		0.7f * DPIScale
	);

	Render::Renderer->DrawSetColor(OUTLINE_LIGHT_COLOR_MENU.ScaleAlpha(Animation));
	Render::Renderer->DrawOutlinedRect(X + GroupSizeX - PADDING * 4 - SizeX, Y, SizeX, SizeY);

	const auto KeyString = Selected ? "..." : GetKeyString(Input::KeyCodes(Key));
	Render::Font::Hack->DrawText(
		X + GroupSizeX - PADDING * 4 - SizeX / 2, Y,
		KeyString, LibUtil_StringLength(KeyString),
		(Col).ScaleAlpha(Animation).ToUInt(),
		Render::Font::FONT_SHADOW | Render::Font::FONT_CENTER,
		0.7f * DPIScale
	);

	Y += SizeY + Padding;
}

void GUI::CFocusItem::Update(CItem *const Item, 
						const int PositionX, const int PositionY, 
						const int GroupSizeX, 
						const int MousePositionX, const int MousePositionY,
						const bool KeybindSelector)
{
	this->Item 				= Item;
	this->MousePositionX 	= MousePositionX;
	this->MousePositionY 	= MousePositionY;
	this->PositionX 		= PositionX;
	this->PositionY 		= PositionY;
	this->GroupSizeX 		= GroupSizeX;
	this->KeybindSelector 	= KeybindSelector;
	this->Animation 		= 0.f;
	this->FirstRun 			= true;
	this->Active 			= true;
}

void GUI::CFocusItem::Reset()
{
	this->Item 				= 0;
	this->MousePositionX 	= 0;
	this->MousePositionY 	= 0;
	this->PositionX 		= 0;
	this->PositionY 		= 0;
	this->GroupSizeX 		= 0;
	this->KeybindSelector 	= 0;
	this->Animation 		= 0;
	this->FirstRun 			= 0;
	this->Active 			= 0;
}

void HandleDropDownAnimation(const bool Change, const bool Active, 
					const float TimeSinceLastFrame, 
					bool& Opened, float& Animation)
{
	if(Change && (Animation <= 0.1f || Animation >= 0.9f))
	{
		Opened = !Opened;
	}

	if(!Active)
	{
		Opened = FALSE;
	}

	if(Opened == TRUE)
	{
		Animation = LibUtil_InterpolateF(ANIMATION_SPEED, Animation, 1.f);
	}
	else
	{
		Animation = LibUtil_InterpolateF(ANIMATION_SPEED, Animation, 0.f);
	}
}

const char *GetKeyString(Input::KeyCodes Code)
{
	switch(Code)
	{
		case Input::KeyCodes::KEY_A:
			return "A";
		case Input::KeyCodes::KEY_B:
			return "B";
		case Input::KeyCodes::KEY_C:
			return "C";
		case Input::KeyCodes::KEY_D:
			return "D";
		case Input::KeyCodes::KEY_E:
			return "E";
		case Input::KeyCodes::KEY_F:
			return "F";
		case Input::KeyCodes::KEY_G:
			return "G";
		case Input::KeyCodes::KEY_H:
			return "H";
		case Input::KeyCodes::KEY_I:
			return "I";
		case Input::KeyCodes::KEY_J:
			return "J";
		case Input::KeyCodes::KEY_K:
			return "K";
		case Input::KeyCodes::KEY_L:
			return "L";
		case Input::KeyCodes::KEY_M:
			return "M";
		case Input::KeyCodes::KEY_N:
			return "N";
		case Input::KeyCodes::KEY_O:
			return "O";
		case Input::KeyCodes::KEY_P:
			return "P";
		case Input::KeyCodes::KEY_Q:
			return "Q";
		case Input::KeyCodes::KEY_R:
			return "R";
		case Input::KeyCodes::KEY_S:
			return "S";
		case Input::KeyCodes::KEY_T:
			return "T";
		case Input::KeyCodes::KEY_U:
			return "U";
		case Input::KeyCodes::KEY_V:
			return "V";
		case Input::KeyCodes::KEY_W:
			return "W";
		case Input::KeyCodes::KEY_X:
			return "X";
		case Input::KeyCodes::KEY_Y:
			return "Y";
		case Input::KeyCodes::KEY_Z:
			return "Z";
		case Input::KeyCodes::KEY_0:
			return "0";
		case Input::KeyCodes::KEY_1:
			return "1";
		case Input::KeyCodes::KEY_2:
			return "2";
		case Input::KeyCodes::KEY_3:
			return "3";
		case Input::KeyCodes::KEY_4:
			return "4";
		case Input::KeyCodes::KEY_5:
			return "5";
		case Input::KeyCodes::KEY_6:
			return "6";
		case Input::KeyCodes::KEY_7:
			return "7";
		case Input::KeyCodes::KEY_8:
			return "8";
		case Input::KeyCodes::KEY_9:
			return "9";
		case Input::KeyCodes::KEY_ENTER:
			return "ENTER";
		case Input::KeyCodes::KEY_MINUS:
			return "MINUS";
		case Input::KeyCodes::KEY_PLUS:
			return "PLUS";
		case Input::KeyCodes::KEY_PERIOD:
			return "PERIOD";
		case Input::KeyCodes::KEY_COMMA:
			return "COMMA";
		case Input::KeyCodes::KEY_LSHIFT:
			return "LSHIFT";
		case Input::KeyCodes::KEY_RSHIFT:
			return "RSHIFT";
		case Input::KeyCodes::KEY_LCTRL:
			return "LCTRL";
		case Input::KeyCodes::KEY_RCTRL:
			return "RCTRL";
		case Input::KeyCodes::KEY_ALT:
			return "ALT";
		case Input::KeyCodes::KEY_ALTGR:
			return "ALTGR";
		case Input::KeyCodes::KEY_CAPSLOCK:
			return "CAPS";
		case Input::KeyCodes::KEY_TAB:
			return "TAB";
		case Input::KeyCodes::KEY_SPACE:
			return "SPACE";
		case Input::KeyCodes::KEY_ARROW_LEFT:
			return "LEFT";
		case Input::KeyCodes::KEY_ARROW_RIGHT:
			return "RIGHT";
		case Input::KeyCodes::KEY_ARROW_UP:
			return "UP";
		case Input::KeyCodes::KEY_ARROW_DOWN:
			return "DOWN";
		case Input::KeyCodes::KEY_NUMPAD_0:
			return "N0";
		case Input::KeyCodes::KEY_NUMPAD_1:
			return "N1";
		case Input::KeyCodes::KEY_NUMPAD_2:
			return "N2";
		case Input::KeyCodes::KEY_NUMPAD_3:
			return "N3";
		case Input::KeyCodes::KEY_NUMPAD_4:
			return "N4";
		case Input::KeyCodes::KEY_NUMPAD_5:
			return "N5";
		case Input::KeyCodes::KEY_NUMPAD_6:
			return "N6";
		case Input::KeyCodes::KEY_NUMPAD_7:
			return "N7";
		case Input::KeyCodes::KEY_NUMPAD_8:
			return "N8";
		case Input::KeyCodes::KEY_NUMPAD_9:
			return "N9";
		case Input::KeyCodes::KEY_F1:
			return "F1";
		case Input::KeyCodes::KEY_F2:
			return "F2";
		case Input::KeyCodes::KEY_F3:
			return "F3";
		case Input::KeyCodes::KEY_F4:
			return "F4";
		case Input::KeyCodes::KEY_F5:
			return "F5";
		case Input::KeyCodes::KEY_F6:
			return "F6";
		case Input::KeyCodes::KEY_F7:
			return "F7";
		case Input::KeyCodes::KEY_F8:
			return "F8";
		case Input::KeyCodes::KEY_F9:
			return "F9";
		case Input::KeyCodes::KEY_F10:
			return "F10";
		case Input::KeyCodes::KEY_F11:
			return "F11";
		case Input::KeyCodes::KEY_F12:
			return "F12";
		case Input::KeyCodes::KEY_INSERT:
			return "INSERT";
		case Input::KeyCodes::KEY_DELETE:
			return "DELETE";
		case Input::KeyCodes::KEY_PGUP:
			return "PGUP";
		case Input::KeyCodes::KEY_PGDOWN:
			return "PGDOWN";
		case Input::KeyCodes::KEY_BACKSPACE:
			return "BACK";
		case Input::KeyCodes::KEY_ESCAPE:
			return "ESCAPE";
		case Input::KeyCodes::MOUSE_LEFT:
			return "M1";
		case Input::KeyCodes::MOUSE_RIGHT:
			return "M2";
		case Input::KeyCodes::MOUSE_WHEEL:
			return "M3";
		case Input::KeyCodes::MOUSE_BUTTON4:
			return "M4";
		case Input::KeyCodes::MOUSE_BUTTON5:
			return "M5";
		default:
			return "-";
	}
}


			