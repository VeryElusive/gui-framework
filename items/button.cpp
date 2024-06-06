#include "button.hpp"
#include "../form.hpp"

#include "../../input/mouse.hpp"
#include "../../input/keys.hpp"

#include "../../render/base.hpp"
#include "../../render/font/base.hpp"

using namespace Cheat;

bool GUI::CButton::Render(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput)
{
	return DrawButton(RenderPositionX, RenderPositionY,
					GroupSizeX,
					Name, Value, 
					ConfirmationCountdown,
					Color,
                    TimeSinceLastFrame, Alpha, 
					FormAccent, DPIScale, 
					AllowInput);
}

void RenderButton(const wchar_t *Name,
				const Datatypes::Color Color, 
				const int RenderPositionX, const int RenderPositionY,
				const int SizeX, const int SizeY,
				const float ConfirmationCountdown,
				const float Alpha, const float DPIScale);

void ButtonHandleInput(Datatypes::Color& Color, void *const Callback, 
				const int RenderPositionX, const int RenderPositionY,
				const int SizeX, const int SizeY,
				float& ConfirmationCountdown,
				const float TimeSinceLastFrame, const float Alpha, 
				const float DPIScale,
				const bool AllowInput);

bool GUI::DrawButton(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
						const wchar_t *Name, void *Callback, 
						float& ConfirmationCountdown, 
						Cheat::Datatypes::Color& Color,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput)
{
	const auto SizeX 			= (int)(GroupSizeX - PADDING * 4);
	const auto SizeY 			= (int)(ITEM_HEIGHT * 2 * DPIScale);
	const auto Padding 			= (int)(PADDING * DPIScale);

	ButtonHandleInput(
		Color, Callback, 
		RenderPositionX, RenderPositionY, 
		SizeX, SizeY, 
		ConfirmationCountdown, 
		TimeSinceLastFrame, 
		Alpha, DPIScale, 
		AllowInput
	);

	RenderButton(
		Name,
		Color, 
		RenderPositionX, RenderPositionY,
		SizeX, SizeY,
		ConfirmationCountdown,
		Alpha, DPIScale
	);

	RenderPositionY += SizeY + Padding;

	return FALSE;
}

void RenderButton(const wchar_t *Name,
				const Datatypes::Color Color, 
				const int RenderPositionX, const int RenderPositionY,
				const int SizeX, const int SizeY,
				const float ConfirmationCountdown,
				const float Alpha, const float DPIScale)
{
	Render::Renderer->DrawSetColor(Color);
    Render::Renderer->DrawFilledRect(RenderPositionX, RenderPositionY, SizeX, SizeY);

	Render::Renderer->DrawSetColor(OUTLINE_LIGHT_COLOR_MENU);
    Render::Renderer->DrawOutlinedRect(RenderPositionX, RenderPositionY, SizeX, SizeY);

	const auto Text = ConfirmationCountdown > 0.f ? L"Are you sure?" : Name;

	Render::Font::Hack->DrawText(
        RenderPositionX + SizeX / 2, RenderPositionY + 2,
        Text, LibUtil_WStringLength(Text),
        LABEL_COLOR_MENU.ScaleAlpha(Alpha).ToUInt(),
        Render::Font::FONT_SHADOW | Render::Font::FONT_CENTER,
        0.7f * DPIScale
    );
}

void ButtonHandleInput(Datatypes::Color& Color, void *const Callback, 
				const int RenderPositionX, const int RenderPositionY,
				const int SizeX, const int SizeY,
				float& ConfirmationCountdown,
				const float TimeSinceLastFrame, const float Alpha, 
				const float DPIScale,
				const bool AllowInput)
{
	const auto MouseLeftState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);
	const auto Hovered 			= Input::IsMouseInBounds(RenderPositionX, RenderPositionY, SizeX, SizeY);

	if(ConfirmationCountdown > 0.f)
	{
		ConfirmationCountdown -= TimeSinceLastFrame;
	}

	if(AllowInput == TRUE)
	{
		if(MouseLeftState.Down && Hovered)
		{
			Color = Color.Lerp(OUTLINE_DARK_COLOR_MENU, ANIMATION_SPEED);
		}
		else if(Hovered)
		{
			Color = Color.Lerp(DIM_ITEM_COLOR_MENU, ANIMATION_SPEED);
		}
		else 
		{
			Color = Color.Lerp(GROUP_COLOR_MENU, ANIMATION_SPEED);
		}

		if(MouseLeftState.Released && Hovered)
		{
			if(ConfirmationCountdown > 0.f)
			{
				ConfirmationCountdown = 0.f;
				((void(LIBUTIL_CDECL *)())Callback)();
			}
			else
			{
				ConfirmationCountdown = 3.f;
			}
		}
	}
}