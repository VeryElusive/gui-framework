#include "slider.hpp"
#include "../form.hpp"

#include "../../Input/mouse.hpp"
#include "../../Input/keys.hpp"

#include "../../render/base.hpp"
#include "../../render/font/base.hpp"

using namespace Cheat;

bool GUI::CSlider::Render(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Datatypes::Color FormAccent, const float DPIScale, 
						bool AllowKeybinds,
						const bool AllowInput, CFocusItem *const FocusItem)
{
	return GUI::DrawSlider(RenderPositionX, RenderPositionY,
							GroupSizeX, Name, (float*)Value,
							InterpolatedValue, MinValue, MaxValue,
							Selected,
							Color, 
							TimeSinceLastFrame, Alpha, 
							FormAccent, DPIScale, 
							AllowInput, AllowKeybinds,
							FocusItem, this);
}

float MapNumber(float Input, float InputMin, float InputMax, float OutputMin, float OutputMax) 
{
	return (Input - InputMin) / (InputMax - InputMin) * (OutputMax - OutputMin) + OutputMin;
}

void RenderSlider(int RenderPositionX, int& RenderPositionY,
				const int RenderX,
				const int SizeX, const int SizeY,
				const wchar_t *Name, 
				float& InterpolatedValue, const float MinValue, const float MaxValue,
				Datatypes::Color& Color,
                const float TimeSinceLastFrame, const float Alpha, 
				const Datatypes::Color FormAccent, const float DPIScale);

void SliderHandleInput(int RenderPositionX, int& RenderPositionY,
					const int RenderX,
					const int SizeX, const int SizeY,
					const int GroupSizeX,
					float *Value, 
					float& InterpolatedValue, const float MinValue, const float MaxValue,
					bool& Selected, 
					Datatypes::Color& Color,
                    const float TimeSinceLastFrame, const float Alpha, 
					const Datatypes::Color FormAccent, const float DPIScale, 
					const bool AllowInput, bool AllowKeybinds,
					GUI::CFocusItem *const FocusItem, GUI::CSlider *Slider);

bool GUI::DrawSlider(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
						const wchar_t *Name, float *Value, 
						float& InterpolatedValue, const float MinValue, const float MaxValue,
						bool& Selected, 
						Datatypes::Color& Color,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput, bool AllowKeybinds,
						CFocusItem *const FocusItem, CSlider *Slider)
{
	const auto SizeX 			= (GroupSizeX - PADDING * 4) / 3;
	const auto SizeY 			= (int)(ITEM_HEIGHT * 1.5f * DPIScale);
	const auto Padding 			= (int)(PADDING * DPIScale);
	const auto ElementGap 		= ((int)Padding * 1.5f);
	const auto GroupSizeMargin 	= GroupSizeX - PADDING * 4;
	const auto RenderX			= RenderPositionX + GroupSizeMargin - SizeX;

	SliderHandleInput(
		RenderPositionX, RenderPositionY,
		RenderX,
		SizeX, SizeY,
		GroupSizeX,
		Value, 
		InterpolatedValue, MinValue, MaxValue,
		Selected, 
		Color,
        TimeSinceLastFrame, Alpha, 
		FormAccent, DPIScale, 
		AllowInput, AllowKeybinds,
		FocusItem, Slider
	);

	RenderSlider(
		RenderPositionX, RenderPositionY, 
		RenderX, 
		SizeX, SizeY, 
		Name, 
		InterpolatedValue, MinValue, MaxValue, 
		Color, 
		TimeSinceLastFrame, Alpha, 
		FormAccent, DPIScale
	);

	RenderPositionY += SizeY + Padding;

	return FALSE;
}

void RenderSlider(int RenderPositionX, int& RenderPositionY,
				const int RenderX,
				const int SizeX,const int SizeY,
				const wchar_t *Name, 
				float& InterpolatedValue, const float MinValue, const float MaxValue,
				Datatypes::Color& Color,
                const float TimeSinceLastFrame, const float Alpha, 
				const Datatypes::Color FormAccent, const float DPIScale)
{
	const auto BarPercent = MapNumber(InterpolatedValue, MinValue, MaxValue, 0.f, 1.f);

	Render::Font::Hack->DrawText(
		RenderPositionX, RenderPositionY - 1,
		Name, LibUtil_WStringLength(Name),
		(Datatypes::Color(255,255,255).ScaleAlpha(Alpha)).ToUInt(),
		Render::Font::FONT_SHADOW,
		0.7f * DPIScale
	);

	Render::Renderer->DrawSetColor(Color);
    Render::Renderer->DrawFilledRect(RenderX, RenderPositionY, SizeX * BarPercent, SizeY);

	Render::Renderer->DrawSetColor(HOVERED_ITEM_COLOR_MENU);
    Render::Renderer->DrawOutlinedRect(RenderX, RenderPositionY, SizeX, SizeY);
}

void SliderHandleInput(int RenderPositionX, int& RenderPositionY,
					const int RenderX,
					const int SizeX, const int SizeY,
					const int GroupSizeX,
					float *Value, 
					float& InterpolatedValue, const float MinValue, const float MaxValue,
					bool& Selected, 
					Datatypes::Color& Color,
                    const float TimeSinceLastFrame, const float Alpha, 
					const Datatypes::Color FormAccent, const float DPIScale, 
					const bool AllowInput, bool AllowKeybinds,
					GUI::CFocusItem *const FocusItem, GUI::CSlider *Slider)
{
	const auto MouseLeftState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);
	const auto MouseRightState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_RIGHT);
	const auto Hovered 			= Input::IsMouseInBounds(RenderX, RenderPositionY, SizeX, SizeY);

	int MousePosX, MousePosY;
	Input::GetMousePos(MousePosX, MousePosY);

	if(Hovered && AllowInput && MouseLeftState.Pressed)
	{
		if(FocusItem)
		{
			FocusItem->Update((GUI::CItem *)Slider, 
						RenderPositionX, 
						RenderPositionY, 
						GroupSizeX,
						MousePosX, MousePosY, 
						false);
		}

		Selected = TRUE;
	}

	if(Selected)
	{
		Color = Color.Lerp(FormAccent, ANIMATION_SPEED);
		if(MouseLeftState.Released)
		{
			if(FocusItem)
			{
				FocusItem->Active = FALSE;
			}

			Selected = FALSE;
		}

		const auto Offset = LibUtil_ClampI32(MousePosX - RenderX, 0, SizeX);
		*Value = MapNumber((float)Offset, 0.f, (float)SizeX, (float)MinValue, (float)MaxValue);
	}
	else if(FocusItem && FocusItem->Item == Slider && FocusItem->Active == TRUE)
	{
		Color = Color.Lerp(FormAccent, ANIMATION_SPEED);
	}
	else if(Hovered && AllowInput)
	{
		Color = Color.Lerp(SELECTED_ITEM_COLOR_MENU, ANIMATION_SPEED);

		if(MouseRightState.Pressed && FocusItem)
		{
			FocusItem->Update((GUI::CItem *)Slider, 
					RenderPositionX, 
					RenderPositionY, 
					GroupSizeX,
					MousePosX, MousePosY, 
					true);
		}
	}
	else
	{
		Color = Color.Lerp(DIM_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}

	InterpolatedValue = LibUtil_LerpF(ANIMATION_SPEED, InterpolatedValue, *Value);
}