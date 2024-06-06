#include "listbox.hpp"
#include "../form.hpp"

#include "../../input/mouse.hpp"
#include "../../input/keys.hpp"

#include "../../render/base.hpp"
#include "../../render/font/base.hpp"

using namespace Cheat;

bool GUI::CListbox::Render(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX, const int GroupSizeY,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput)
{
	return DrawListbox(Name, 
		RenderPositionX, RenderPositionY, 
		GroupSizeX, GroupSizeY, 
		(int *)Value, 
		Elements, ElementsCount, 
		Scroll, MaxHeightPercentage, 
		TimeSinceLastFrame, Alpha, 
		FormAccent, DPIScale, 
		AllowInput
	);
}

void RenderListbox(wchar_t *const Name,
					int RenderPositionX, int& RenderPositionY,
					const int SizeX, const int SizeY,
                    const float Alpha, 
					const Cheat::Datatypes::Color FormAccent, const float DPIScale);

void RenderListboxItem(wchar_t *const Name, 
					const int RenderPositionX, const int RenderPositionY,
					const Datatypes::Color Color,
					const float DPIScale, const float Alpha);

void ListboxHandleInput(const bool Hovered,
				int& Scroll,
				const int Outside,
				const float TimeSinceLastFrame);

void RenderListboxItem(wchar_t *const Name,
					const int RenderPositionX, const int RenderPositionY,
					const float DPIScale, const float Alpha);

void ListboxItemHandleInput(const bool Hovered,
						int *const Value, const int i,
						Datatypes::Color& Color,
						const float TimeSinceLastFrame, const float Alpha,
						const bool Clicked);

bool GUI::DrawListbox(wchar_t *const Name,
					int RenderPositionX, int& RenderPositionY,
					const int GroupSizeX, const int GroupSizeY,
					int *const Value,
					CBaseItem *const Elements, const int ElementsCount,
					int& Scroll,
					const float MaxHeightPercentage,
                    const float TimeSinceLastFrame, const float Alpha, 
					const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
					const bool AllowInput)
{
	const auto SizeX 			= (int)(GroupSizeX - PADDING * 4);
	const auto SizeY 			= (int)((GroupSizeY - PADDING * 4) * MaxHeightPercentage);
	const auto Padding 			= (int)(PADDING * DPIScale);
	const auto ElementGap		= (int)(ITEM_HEIGHT + 4 * DPIScale);
	const auto MouseLeftState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);

	RenderListbox(
		Name,
		RenderPositionX, RenderPositionY,
		SizeX, SizeY,
		Alpha, 
		FormAccent,
		DPIScale
	);

	int BackupX, BackupY, BackupW, BackupH;
    Render::Renderer->GetClippingRect(BackupX, BackupY, BackupW, BackupH);
    Render::Renderer->SetClippingRect(RenderPositionX + 1, LibUtil_MaxI32(BackupY, RenderPositionY + 1), SizeX - 2, SizeY - 2);

	const auto RenderX = RenderPositionX + PADDING;
	for(int i = 0; i < ElementsCount; i++)
	{
		CBaseItem *Element = &Elements[i];
		if(Element == NULL)
		{
			continue;
		}

		const auto PositionY 	= RenderPositionY + Scroll + Padding + ElementGap * i;
		const auto Hovered 		= Input::IsMouseInBounds(RenderX, PositionY, SizeX - PADDING * 2, ElementGap);
		
		ListboxItemHandleInput(Hovered, 
			Value, i, 
			Element->Color, 
			TimeSinceLastFrame, Alpha, 
			MouseLeftState.Pressed
		);

		RenderListboxItem(
			Element->Name, 
			RenderX, PositionY, 
			Element->Color,
			DPIScale, Alpha
		);
	}

	const auto Hovered = Input::IsMouseInBounds(RenderPositionX, RenderPositionY, SizeX, SizeY);
	const auto Outside = Scroll + Padding + ElementGap * (ElementsCount - 1);

	ListboxHandleInput(Hovered,
		Scroll, Outside, TimeSinceLastFrame);

	Render::Renderer->SetClippingRect(BackupX, BackupY, BackupW, BackupH);

	RenderPositionY += SizeY + Padding;

	return Hovered;
}

void RenderListbox(wchar_t *const Name,
					int RenderPositionX, int& RenderPositionY,
					const int SizeX, const int SizeY,
                    const float Alpha, 
					const Cheat::Datatypes::Color FormAccent, const float DPIScale)
{
	Render::Renderer->DrawSetColor(GROUP_COLOR_MENU);
    Render::Renderer->DrawFilledRect(RenderPositionX, RenderPositionY, SizeX, SizeY);

	Render::Renderer->DrawSetColor(OUTLINE_LIGHT_COLOR_MENU);
    Render::Renderer->DrawOutlinedRect(RenderPositionX, RenderPositionY, SizeX, SizeY);

	int TW, TH;
    Render::Font::Hack->GetTextSizeFast(TW, TH, LibUtil_WStringLength(Name), 0.7f * DPIScale);

	Render::Font::Hack->DrawText(
        RenderPositionX + PADDING, RenderPositionY - (int)(TH / 2),
        Name, LibUtil_WStringLength(Name),
        Color(255,255,255).ScaleAlpha(Alpha).ToUInt(),
        Render::Font::FONT_SHADOW,
        0.7f * DPIScale
    );
}

void RenderListboxItem(wchar_t *const Name, 
					const int RenderPositionX, const int RenderPositionY,
					const Datatypes::Color Color,
					const float DPIScale, const float Alpha)
{
	Render::Font::Hack->DrawText(
        RenderPositionX, RenderPositionY,
        Name, LibUtil_WStringLength(Name),
        Color.ScaleAlpha(Alpha).ToUInt(),
        Render::Font::FONT_SHADOW,
        0.7f * DPIScale
    );
}

void ListboxItemHandleInput(const bool Hovered,
						int *const Value, const int i,
						Datatypes::Color& Color,
						const float TimeSinceLastFrame, const float Alpha,
						const bool Clicked)
{
	if(Hovered && Clicked)
	{
		*Value = i;
	}

	if(*Value == i)
	{
		Color = Color.Lerp(SELECTED_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}
	else if(Hovered)
	{
		Color = Color.Lerp(HOVERED_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}
	else
	{
		Color = Color.Lerp(DIM_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}
}

void ListboxHandleInput(const bool Hovered,
				int& Scroll,
				const int Outside,
				const float TimeSinceLastFrame)
{
	if(Hovered)
	{
		const auto ScrollState  = Input::GetMouseWheelState();
		Scroll += ScrollState * 12;
	}
	Scroll = LibUtil_LerpI32(Scroll, LibUtil_ClampI32(Scroll, -Outside, 0), ANIMATION_SPEED * 2);
}