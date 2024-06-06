#include "colorpicker.hpp"
#include "../form.hpp"

#include "../../input/mouse.hpp"
#include "../../input/keys.hpp"

#include "../../render/base.hpp"
#include "../../render/font/base.hpp"

using namespace Cheat;

void RGBToHSV(const Color Col, float& H, float& S, float& V);
void HSVToRGB(float H, float S, const float V, Color& Col);
Color hsv2rgb(float hue, float sat, float val);

bool GUI::CColorPicker::Render(int RenderPositionX, int& RenderPositionY,
		const int GroupSizeX,
		const float FocusAnimation,
                        const float TimeSinceLastFrame, const float Alpha, 
		const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
		const bool AllowInput, CFocusItem *const FocusItem)
{
	return DrawColorPicker(RenderPositionX, RenderPositionY, 
		GroupSizeX,
		this->Name, (Datatypes::Color*)Value, 
		SelectAlpha,
		SelectingState,
		FocusAnimation,
		TimeSinceLastFrame, Alpha, 
		FormAccent, DPIScale, 
		AllowInput, 
		FocusItem, this);
}

void RenderColorpicker(Datatypes::Color *const Value,
					const int WidgetPositionX, const int WidgetPositionY,
					const int SizeX, const int SizeY,
					const int PopupPosX, const int PopupPosY,
					const int PopupSizeX, const int PopupSizeY,
					const int ColorBoardX, const int ColorBoardY,
					const int ColorBoardW, const int ColorBoardH,
					const int HueSelectorY, const int AlphaSelectorY,
					const float Hue, const float Sat, const float Val,
					const bool SelectAlpha,
					const float FocusAnimation, const float Alpha,
					const int Padding,
					const Datatypes::Color FormAccent, const float DPIScale);

void ColorpickerHandleInput(Datatypes::Color *const Value,
					const int RenderPositionX, const int RenderPositionY,
					const int GroupSizeX,
					const int WidgetPositionX, const int WidgetPositionY, 
					const int SizeX, const int SizeY,
					const int PopupPosX, const int PopupPosY, 
					const int PopupSizeX, const int PopupSizeY,
					const int ColorBoardX, const int ColorBoardY,
					const int ColorBoardW, const int ColorBoardH,
					float& Hue, float& Sat, float& Val,
					libutil_u8& SelectingState,
					const int HueSelectorY,
					const bool SelectAlpha,
					const int AlphaSelectorY,
					const int BarSizeY,
					GUI::CFocusItem *const FocusItem,
					const float FocusAnimation,
					GUI::CColorPicker * const ColorPicker);

bool GUI::DrawColorPicker(int RenderPositionX, int& RenderPositionY,
		const int GroupSizeX,
		const wchar_t *Name, Cheat::Datatypes::Color *Value,
		const bool SelectAlpha,
		libutil_u8& SelectingState,
		const float FocusAnimation,
                        const float TimeSinceLastFrame, const float Alpha, 
		const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
		const bool AllowInput, 
		CFocusItem *const FocusItem, CColorPicker * const ColorPicker)
{
	const auto WidgetSizeY 		= (int)(ITEM_HEIGHT * DPIScale);
	const auto WidgetSizeX 		= (int)(WidgetSizeY * 1.5f);
	const auto Padding 			= (int)(PADDING * DPIScale);
	const auto GroupSizeMargin 	= GroupSizeX - PADDING * 4;
	const auto CheckboxSize		= (int)(ITEM_HEIGHT * DPIScale);
	const auto WidgetPositionX	= RenderPositionX + GroupSizeMargin - WidgetSizeX - CheckboxSize - PADDING;
	const auto WidgetPositionY	= RenderPositionY - CheckboxSize - Padding;

	const auto BarSizeY			= (int)(WidgetSizeY * 1.5f);

	const auto PopupSizeX 		= (int)(200 * DPIScale);
	const auto PopupSizeY 		= PopupSizeX + (int)(((WidgetSizeY * 1.5f) + Padding) * (SelectAlpha ? 2 : 1));
	const auto PopupPosX 		= FocusItem->MousePositionX;
	const auto PopupPosY 		= FocusItem->MousePositionY;

	const auto ColorBoardX 		= PopupPosX + Padding;
	const auto ColorBoardY 		= PopupPosY + Padding;
	const auto ColorBoardW 		= PopupSizeX - Padding * 2;
	const auto ColorBoardH 		= PopupSizeX - Padding * 2;

	const auto HueSelectorY 	= ColorBoardY + ColorBoardH + Padding;
	const auto AlphaSelectorY 	= HueSelectorY + BarSizeY + Padding;

	float Hue, Sat, Val;
	RGBToHSV(*Value, Hue, Sat, Val);

	ColorpickerHandleInput(
		Value,
		RenderPositionX, RenderPositionY,
		GroupSizeX,
		WidgetPositionX, WidgetPositionY, 
		WidgetSizeX, WidgetSizeY,
		PopupPosX, PopupPosY, 
		PopupSizeX, PopupSizeY,
		ColorBoardX, ColorBoardY,
		ColorBoardW, ColorBoardH,
		Hue, Sat, Val,
		SelectingState,
		HueSelectorY,
		SelectAlpha,
		AlphaSelectorY,
		BarSizeY,
		FocusItem,
		FocusAnimation,
		ColorPicker
	);

	RenderColorpicker(
		Value, 
		WidgetPositionX, WidgetPositionY, 
		WidgetSizeX, WidgetSizeY, 
		PopupPosX, PopupPosY, 
		PopupSizeX, PopupSizeY, 
		ColorBoardX, ColorBoardY, 
		ColorBoardW, ColorBoardH, 
		HueSelectorY, AlphaSelectorY,
		Hue, Sat, Val,
		SelectAlpha, 
		FocusAnimation, Alpha, 
		Padding, 
		FormAccent, 
		DPIScale
	);

	return FALSE;
}

void RenderColorpicker(Datatypes::Color *const Value,
					const int WidgetPositionX, const int WidgetPositionY,
					const int SizeX, const int SizeY,
					const int PopupPosX, const int PopupPosY,
					const int PopupSizeX, const int PopupSizeY,
					const int ColorBoardX, const int ColorBoardY,
					const int ColorBoardW, const int ColorBoardH,
					const int HueSelectorY, const int AlphaSelectorY,
					const float Hue, const float Sat, const float Val,
					const bool SelectAlpha,
					const float FocusAnimation, const float Alpha,
					const int Padding,
					const Datatypes::Color FormAccent, const float DPIScale)
{
	Render::Renderer->DrawSetColor(Value->ScaleAlpha(Alpha));
    Render::Renderer->DrawFilledRect(WidgetPositionX, WidgetPositionY, SizeX, SizeY);

	Render::Renderer->DrawSetColor(OUTLINE_LIGHT_COLOR_MENU);
    Render::Renderer->DrawOutlinedRect(WidgetPositionX, WidgetPositionY, SizeX, SizeY);

	if(FocusAnimation < 0.01f)
	{
		return;
	}

	GUI::RenderPopup(
		PopupPosX, PopupPosY, 
		PopupSizeX, PopupSizeY, 
		BACKGROUND_COLOR_MENU.ScaleAlpha(FocusAnimation), FormAccent.ScaleAlpha(FocusAnimation)
	);

	Color HueCol(255, 255, 255);
	HSVToRGB(Hue, 1.f, 1.f, HueCol);

	Render::Renderer->DrawSetColor(HueCol.ScaleAlpha(FocusAnimation));
    Render::Renderer->DrawFilledRect(ColorBoardX, ColorBoardY, ColorBoardW, ColorBoardH);

	Render::Renderer->DrawFilledGradientRect(ColorBoardX, ColorBoardY, 
						ColorBoardW, ColorBoardH,
						TRUE,
						Cheat::Datatypes::Color(255,255,255, (int)(FocusAnimation * 255.f)), 
						Cheat::Datatypes::Color(255,255,255, 0));

	Render::Renderer->DrawFilledGradientRect(ColorBoardX, ColorBoardY, 
						ColorBoardW, ColorBoardH,
						FALSE,
						Cheat::Datatypes::Color(0, 0, 0, 0), 
						Cheat::Datatypes::Color(0, 0, 0, (int)(FocusAnimation * 255.f)));

	Render::Renderer->DrawSetColor(OUTLINE_LIGHT_COLOR_MENU.ScaleAlpha(FocusAnimation));
	Render::Renderer->DrawOutlinedRect(ColorBoardX + Sat * ColorBoardW, 
					ColorBoardY + (1.f - Val) * ColorBoardH,
					(int)(4 * DPIScale), (int)(4 * DPIScale));

	for(int i = 0; i < ColorBoardW; i++)
	{
		const auto hue = ((float)i / ColorBoardW) * 360.f;
		Color LineColor;
		HSVToRGB(hue, 1.f, 1.f, LineColor);

		const auto X = ColorBoardX + i;

		Render::Renderer->DrawSetColor(LineColor.ScaleAlpha(FocusAnimation));
		Render::Renderer->DrawLine(X, HueSelectorY, X, HueSelectorY + (int)(SizeY * 1.5f));
	}

	if(SelectAlpha)
	{
		int X, Y, W, H;
		Render::Renderer->GetClippingRect(X, Y, W, H);
		Render::Renderer->SetClippingRect(ColorBoardX, AlphaSelectorY, ColorBoardW, (int)(SizeY * 1.5f));
		for(int i = 0; i < 2; i++) 
		{
			const auto SquareSize = (int)((SizeY * 1.5f) / 2.f);
			const auto RowPosX = ColorBoardX;
			const auto RowPosY = AlphaSelectorY + SquareSize * i;

			for(int n = 0; n < ColorBoardW / SquareSize; ++n) 
			{
				auto SquareCol = Color(225, 225, 225);
				if((i + n) % 2 == 0) 
				{
					SquareCol = Color(255, 255, 255);
				}

				Render::Renderer->DrawSetColor(SquareCol.ScaleAlpha(FocusAnimation));
				Render::Renderer->DrawFilledRect(RowPosX + SquareSize * n, RowPosY, SquareSize, SquareSize);
			}
		}

		Render::Renderer->DrawFilledGradientRect(ColorBoardX, AlphaSelectorY, 
							ColorBoardW, (int)(SizeY * 1.5f),
							TRUE,
							Cheat::Datatypes::Color(255, 255, 255, 0), 
							HueCol.ScaleAlpha(FocusAnimation));

		Render::Renderer->SetClippingRect(X, Y, W, H);
	}
}

void ColorpickerHandleInput(Datatypes::Color *const Value,
		const int RenderPositionX, const int RenderPositionY,
		const int GroupSizeX,
		const int WidgetPositionX, const int WidgetPositionY, 
		const int SizeX, const int SizeY,
		const int PopupPosX, const int PopupPosY, 
		const int PopupSizeX, const int PopupSizeY,
		const int ColorBoardX, const int ColorBoardY,
		const int ColorBoardW, const int ColorBoardH,
		float& Hue, float& Sat, float& Val,
		libutil_u8& SelectingState,
		const int HueSelectorY,
		const bool SelectAlpha,
		const int AlphaSelectorY,
		const int BarSizeY,
		GUI::CFocusItem *const FocusItem,
		const float FocusAnimation,
		GUI::CColorPicker * const ColorPicker)
{
	const auto MouseLeftState 		= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);
	const auto EscapeState 		= Input::GetKeyState(Input::KeyCodes::KEY_ESCAPE);

	const auto ColorBoardHovered  	= Input::IsMouseInBounds(ColorBoardX, ColorBoardY,
						ColorBoardW, ColorBoardH);

	const auto HueSelectorHovered 	= Input::IsMouseInBounds(ColorBoardX, HueSelectorY, 
						ColorBoardW, BarSizeY);

	const auto AlphaSelectorHovered = Input::IsMouseInBounds(ColorBoardX, AlphaSelectorY, 
						ColorBoardW, BarSizeY);

	if(FocusAnimation < 0.1f)
	{
		const auto Hovered 			= Input::IsMouseInBounds(WidgetPositionX, WidgetPositionY, SizeX, SizeY);
		if(Hovered && MouseLeftState.Pressed)
		{
			int MousePosX, MousePosY;
			Input::GetMousePos(MousePosX, MousePosY);

			FocusItem->Update((GUI::CItem *)ColorPicker, 
				RenderPositionX, 
				RenderPositionY, 
				GroupSizeX,
				MousePosX, MousePosY, 
				false
			);
		}

		return;
	}

	const auto Hovered = Input::IsMouseInBounds(PopupPosX, PopupPosY, PopupSizeX, PopupSizeY);
	if(!Hovered)
	{
		if((EscapeState.Pressed || MouseLeftState.Pressed) && FocusItem)
		{
			FocusItem->Active = FALSE;
		}
	}

	if(MouseLeftState.Pressed && !MouseLeftState.Trapped)
	{
		if(ColorBoardHovered)
		{
			SelectingState = 1;
		}
		else if(HueSelectorHovered)
		{
			SelectingState = 2;
		}
		else if(SelectAlpha)
		{
			if(AlphaSelectorHovered)
			{
				SelectingState = 3;
			}
		}
	}
	else if(SelectingState && MouseLeftState.Released)
	{
		SelectingState = 0;
	}

	if(SelectingState == 1)
	{
		int MousePosX, MousePosY;
		Input::GetMousePos(MousePosX, MousePosY);

		const auto CursorDeltaX = MousePosX - ColorBoardX;
		const auto CursorDeltaY = MousePosY - ColorBoardY;
			
		Sat = LibUtil_ClampF(	   (float)CursorDeltaX / (float)ColorBoardW, 0.01f, 0.99f);
		Val = LibUtil_ClampF(1.f - (float)CursorDeltaY / (float)ColorBoardH, 0.01f, 0.99f);
	}
	else if(SelectingState == 2)
	{
		int MousePosX, MousePosY;
		Input::GetMousePos(MousePosX, MousePosY);

		const auto CursorDeltaX = MousePosX - ColorBoardX;

		Hue = LibUtil_ClampF((float)CursorDeltaX / (float)ColorBoardW, 0.f, 1.f) * 359.f;
	}
	else if(SelectingState == 3)
	{
		int MousePosX, MousePosY;
		Input::GetMousePos(MousePosX, MousePosY);

		const auto CursorDeltaX = MousePosX - ColorBoardX;

		Value->a = LibUtil_ClampF((float)CursorDeltaX / (float)ColorBoardW, 0.f, 1.f) * 255.f;
	}

	if(SelectingState)
	{
		HSVToRGB(Hue, Sat, Val, *Value);
	}
}

void RGBToHSV(const Color Col, float& H, float& S, float& V) 
{
	const float Red 	= (float)Col.r / 255.f;
	const float Green 	= (float)Col.g / 255.f;
	const float Blue 	= (float)Col.b / 255.f;
	float x, f, i;

	x = LibUtil_MinF(LibUtil_MinF(Red, Green), Blue);
	V = LibUtil_MaxF(LibUtil_MaxF(Red, Green), Blue);
	if(x == V) 
	{
		H = 0;
		S = 0;
	}
	else {
		f = (Red == x) ? Green - Blue : ((Green == x) ? Blue - Red : Red - Green);
		i = (Red == x) ? 3 : ((Green == x) ? 5 : 1);
		H = LibUtil_ModF((i - f / (V - x)) * 60, 360);
		S = ((V - x) / V);
	}
}

void HSVToRGB(float H, float S, const float V, Color& Col) 
{
    int i;
    float f, p, q, t;

    H /= 60;
    i = (int)H;
    f = H - i;
    p = V * (1 - S);
    q = V * (1 - S * f);
    t = V * (1 - S * (1 - f));

    switch (i) 
	{
        case 0:
            Col.r = (int)(V * 255);
            Col.g = (int)(t * 255);
            Col.b = (int)(p * 255);
            break;
        case 1:
            Col.r = (int)(q * 255);
            Col.g = (int)(V * 255);
            Col.b = (int)(p * 255);
            break;
        case 2:
            Col.r = (int)(p * 255);
            Col.g = (int)(V * 255);
            Col.b = (int)(t * 255);
            break;
        case 3:
            Col.r = (int)(p * 255);
            Col.g = (int)(q * 255);
            Col.b = (int)(V * 255);
            break;
        case 4:
            Col.r = (int)(t * 255);
            Col.g = (int)(p * 255);
            Col.b = (int)(V * 255);
            break;
        default:
            Col.r = (int)(V * 255);
            Col.g = (int)(p * 255);
            Col.b = (int)(q * 255);
            break;
    }
}