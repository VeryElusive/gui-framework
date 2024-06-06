#include "checkbox.hpp"
#include "../form.hpp"

#include "../../input/mouse.hpp"
#include "../../input/keys.hpp"

#include "../../render/base.hpp"
#include "../../render/font/base.hpp"

using namespace Cheat;

bool GUI::CCheckbox::Render(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowKeybinds,
						const bool AllowInput, CFocusItem *const FocusItem)
{
	return DrawCheckbox(
					RenderPositionX, RenderPositionY,
					GroupSizeX,
					Name, (bool*)Value, 
					AllowKeybinds,
					Color,
                    TimeSinceLastFrame, Alpha, 
					FormAccent, DPIScale, 
					AllowInput, 
					FocusItem, this
	);
}

void RenderCheckbox(Datatypes::Color& Color,
				const int RenderPositionX, const int RenderPositionY,
				const int GroupSizeMargin,
				const int SizeX, const int SizeY,
				const wchar_t *Name, const float Alpha,
				const float DPIScale);

void CheckboxHandleInput(bool *const Value,
					Datatypes::Color& Color,
					const int RenderPositionX, const int RenderPositionY,
					const int GroupSizeMargin,	
					const int GroupSizeX,	
					const int SizeX, const int SizeY,
					const float TimeSinceLastFrame, const float Alpha,
					Datatypes::Color FormAccent,
					const bool AllowInput, const bool AllowKeybinds,
					GUI::CFocusItem *const FocusItem, GUI::CCheckbox *const Checkbox);

bool GUI::DrawCheckbox(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
						const wchar_t *Name, bool *Value, 
						bool AllowKeybinds, 
						Cheat::Datatypes::Color& Color,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput, 
						CFocusItem *const FocusItem, CCheckbox * const Checkbox)
{
	const auto SizeX 			= (int)(ITEM_HEIGHT * DPIScale);
	const auto SizeY 			= (int)(ITEM_HEIGHT * DPIScale);
	const auto Padding 			= (int)(PADDING * DPIScale);
	const auto GroupSizeMargin 	= GroupSizeX - PADDING * 4;

	CheckboxHandleInput(
		Value, Color, 
		RenderPositionX, RenderPositionY, 
		GroupSizeMargin, GroupSizeX, 
		SizeX, SizeY, 
		TimeSinceLastFrame, Alpha, 
		FormAccent, 
		AllowInput, AllowKeybinds, 
		FocusItem, Checkbox
	);

	RenderCheckbox(
		Color, 
		RenderPositionX, RenderPositionY, 
		GroupSizeMargin, 
		SizeX, SizeY, 
		Name, 
		Alpha, 
		DPIScale
	);

	RenderPositionY += SizeY + Padding;

	return FALSE;
}

void RenderCheckbox(Datatypes::Color& Color,
				const int RenderPositionX, const int RenderPositionY,
				const int GroupSizeMargin,
				const int SizeX, const int SizeY,
				const wchar_t *Name, const float Alpha,
				const float DPIScale)
{
	Render::Renderer->DrawSetColor(Color);
    Render::Renderer->DrawFilledRect(RenderPositionX + GroupSizeMargin - SizeX, RenderPositionY, SizeX, SizeY);

	Render::Renderer->DrawSetColor(OUTLINE_LIGHT_COLOR_MENU);
    Render::Renderer->DrawOutlinedRect(RenderPositionX + GroupSizeMargin - SizeX, RenderPositionY, SizeX, SizeY);

	Render::Font::Hack->DrawText(
        RenderPositionX, RenderPositionY - 2,
        Name, LibUtil_WStringLength(Name),
        LABEL_COLOR_MENU.ScaleAlpha(Alpha).ToUInt(),
        Render::Font::FONT_SHADOW,
        0.7f * DPIScale
    );
}

void CheckboxHandleInput(bool *const Value,
					Datatypes::Color& Color,
					const int RenderPositionX, const int RenderPositionY,
					const int GroupSizeMargin,	
					const int GroupSizeX,	
					const int SizeX, const int SizeY,
					const float TimeSinceLastFrame, const float Alpha,
					Datatypes::Color FormAccent,
					const bool AllowInput, const bool AllowKeybinds,
					GUI::CFocusItem *const FocusItem, GUI::CCheckbox *const Checkbox)
{
	const auto MouseRightState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_RIGHT);
	const auto MouseLeftState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);
	const auto Hovered 			= Input::IsMouseInBounds(RenderPositionX + GroupSizeMargin - SizeX, RenderPositionY, SizeX, SizeY);

	if(AllowInput && Hovered)
	{
		if(MouseLeftState.Pressed)
		{
			*Value = !*Value;
		}

		if(AllowKeybinds && MouseRightState.Pressed && FocusItem)
		{
			int MousePosX, MousePosY;
			Input::GetMousePos(MousePosX, MousePosY);
			FocusItem->Update((GUI::CItem *)Checkbox, 
					RenderPositionX, 
					RenderPositionY, 
					GroupSizeX,
					MousePosX, MousePosY, 
					TRUE);
		}
	}

	Color = Color.Lerp(*Value ? 
		FormAccent.ScaleAlpha(Alpha) : AllowInput && Hovered ? 
		HOVERED_ITEM_COLOR_MENU : BACKGROUND_COLOR_MENU, ANIMATION_SPEED);
}