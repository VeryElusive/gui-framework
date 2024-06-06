#include "form.hpp"
#include "group.hpp"

#include "../input/mouse.hpp"
#include "../input/keys.hpp"

#include "../render/base.hpp"
#include "../render/font/base.hpp"

#include "libutil/common/memlib.h"
#include "libutil/common/strlib.h"
#include "libutil/libutil/math/int.h"

using namespace Cheat;

int GUI::DPIScale = 2;

void GUI::CForm::Render(const float TimeSinceLastFrame)
{
	const auto key = Input::GetKeyState(Input::KeyCodes::KEY_INSERT); 
    if(key.Pressed && !key.Trapped)
	{
		Opened = !Opened;
	}

	if(!Opened)
	{
		FocusItem.Reset();
	}

	if(Alpha > 0.f && !Opened)
	{
		Alpha -= ANIMATION_SPEED;

		Alpha = LibUtil_MaxF(Alpha, 0.f);
	}
	else if(Alpha < 1.f && Opened)
	{
		Alpha += ANIMATION_SPEED;

		Alpha = LibUtil_MinF(Alpha, 1.f);
	}

	if(Alpha <= 0.f)
	{
		return;
	}

	HandleControls();

	GUI::RenderPopup(PositionX, PositionY, SizeX, SizeY, BACKGROUND_COLOR_MENU, Color);

	const auto OldSelectedTab = SelectedTab;

	RenderTabs(TimeSinceLastFrame, Alpha);
	if(SelectedTab == NULL)
	{
		return;
	}
	SelectedTab->RenderSubTabs(PositionX, PositionY, SizeX, SizeY, TimeSinceLastFrame, Alpha, Color);

    if(SelectedTab->SelectedSubTab == NULL)
    {
		return;
	}

	const auto OldSelectedSubTab = SelectedTab->SelectedSubTab;

	const auto DPIScale = GUI::GetDPIScale();

    SelectedTab->SelectedSubTab->RenderGroups(SizeX, SizeY, 
											  PositionX, PositionY,
											  TimeSinceLastFrame, Alpha, 
											  Color, DPIScale, 
											  FocusItem.Active == FALSE, &FocusItem);

	static float LastDPIScale;

	if(OldSelectedSubTab != SelectedTab->SelectedSubTab
		|| OldSelectedTab != SelectedTab
		|| LastDPIScale != DPIScale
		|| Dragging != 0)
	{
		FocusItem.Active = FALSE;
		FocusItem.Animation = 0.f;
	}

	LastDPIScale = DPIScale;

	if(FocusItem.Item != NULL || FocusItem.Animation > 0.f)
	{
		if(FocusItem.Active != FALSE)
		{
			FocusItem.Animation = LibUtil_InterpolateF(ANIMATION_SPEED, FocusItem.Animation, 1.f);
		}
		else
		{
			FocusItem.Animation = LibUtil_InterpolateF(ANIMATION_SPEED, FocusItem.Animation, 0.f);
		}

		Render::Renderer->DrawSetColor(BACKGROUND_COLOR_MENU.ScaleAlpha(FocusItem.Animation * 0.7f));
		Render::Renderer->DrawFilledRect(PositionX + SIDE_BAR_SIZE + 1, PositionY + TOP_BAR_SIZE + 1, SizeX - SIDE_BAR_SIZE - 2, SizeY - TOP_BAR_SIZE - 2);
		
		FocusItem.Render(TimeSinceLastFrame, Alpha, Color, DPIScale);
	}
}

void GUI::CForm::HandleControls()
{
	int MouseDeltaX, MouseDeltaY;
	Input::GetMouseDelta(MouseDeltaX, MouseDeltaY);

	const auto MouseLeftState 		= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);
	const auto TopBarHovered 		= Input::IsMouseInBounds(PositionX + MouseDeltaX, PositionY + MouseDeltaY, SizeX, MARGIN);
	const auto BottomCornerHovered 	= Input::IsMouseInBounds(PositionX + SizeX + MouseDeltaX - 20, PositionY + SizeY + MouseDeltaY - 20, 20, 20);
	
	if(MouseLeftState.Released)
	{
		Dragging = 0;
	}
	else if(MouseLeftState.Pressed)
	{
		if(TopBarHovered)
		{
			Dragging = 1;
		}
		else if(BottomCornerHovered && Resizeable)
		{
			Dragging = 2;
		}
	}

	if(Dragging == 1)
	{
		PositionX += MouseDeltaX;
		PositionY += MouseDeltaY;
	}
	else if(Dragging == 2)
	{
		int MouseX, MouseY;
		Input::GetMousePos(MouseX, MouseY);
		if(MouseX >= PositionX + SizeX + MouseDeltaX - 20
			&& MouseX <= PositionX + SizeX + MouseDeltaX)
		{
			SizeX += MouseDeltaX;
			SizeX = LibUtil_ClampI32(SizeX, MinSizeX, MaxSizeX);
		}
			
		if(MouseY >= PositionY + SizeY + MouseDeltaY - 20
			&& MouseY <= PositionY + SizeY + MouseDeltaY)
		{
			SizeY += MouseDeltaY;
			SizeY = LibUtil_ClampI32(SizeY, MinSizeY, MaxSizeY);
		}
	}
}

void GUI::CForm::RenderTabs(const float TimeSinceLastFrame, const float Alpha)
{
	if(!Tabs.Count)
	{
		return;
	}

	if(SelectedTab == NULL)
	{
		SelectedTab = (CTab *)LibUtil_Vector_GetElement(&Tabs, 0);
	}

	int TW, TH;
    Render::Font::Hack->GetTextSizeFast(TW, TH, LibUtil_WStringLength(SelectedTab->Name), 1.f);

	const auto MaxHeight 		= SizeY - MARGIN * 2 - TH;
	const auto DistributedSize 	= MaxHeight / (Tabs.Count - 1);
	const auto MouseLeftState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);

	for(int i = 0; i < Tabs.Count; i++)
	{
		const auto Tab = (CTab *)LibUtil_Vector_GetElement(&Tabs, i);
		if(Tab == NULL)
		{
			continue;
		}

		const auto PosX = PositionX + MARGIN;
		const auto PosY = PositionY + MARGIN + DistributedSize * i;

		if(SelectedTab == Tab)
		{
			Tab->Color = Tab->Color.Lerp(Color, ANIMATION_SPEED);
		}
		else if(Input::IsMouseInBounds(PosX, PosY, TW, TH))
		{
			if(MouseLeftState.Pressed)
			{
				SelectedTab = Tab;
			}
			
			Tab->Color = Tab->Color.Lerp(SELECTED_ITEM_COLOR_MENU, ANIMATION_SPEED);
		}
		else
		{
			Tab->Color = Tab->Color.Lerp(HOVERED_ITEM_COLOR_MENU, ANIMATION_SPEED);
		}

        Render::Font::Hack->DrawText(
            PosX,
            PosY,
            Tab->Name,
            LibUtil_WStringLength(Tab->Name),
            Tab->Color.ScaleAlpha(Alpha).ToUInt(),
            Render::Font::FONT_SHADOW,
            1.f
        );
	}
}

GUI::CTab *GUI::CForm::CreateNewTab(const wchar_t *Name)
{
	CTab NewTab(Name, Cheat::Datatypes::Color(DIM_ITEM_COLOR));
	LibUtil_Vector_PushBack(&Tabs, &NewTab);

	return (CTab *)LibUtil_Vector_GetElement(&Tabs, Tabs.Count - 1);
}

GUI::CForm::CForm(const int X, const int Y, 
				  const int W, const int H, 
				  const wchar_t *Name, 
				  Cheat::Datatypes::Color Col, 
				  const bool Open, 
				  const bool AllowResizing, 
				  const int MinW, const int MinH,
				  const int MaxW, const int MaxH)
: CBaseItem(Name, Col)
{
	SizeX 			= W;
	SizeY 			= H;
	PositionX		= X;
	PositionY		= Y;

	Resizeable		= AllowResizing;

	MinSizeX		= MinW;
	MaxSizeX		= MaxW;

	MinSizeY		= MinH;
	MaxSizeY		= MaxH;

	Opened 	= Open;

	LibUtil_Vector_Setup(&Tabs, sizeof(CTab));
}

void GUI::CForm::Destroy()
{
	for(int i = 0; i < Tabs.Count; i++)
	{
		const auto Tab = (CTab *)LibUtil_Vector_GetElement(&Tabs, i);
		if(!Tab)
		{
			continue;
		}

		Tab->Destroy();
	}

	LibUtil_Vector_Destroy(&Tabs);
}

float GUI::GetDPIScale()
{
	switch(DPIScale)
	{
	case 0:
		return 0.5f;
	case 1:
		return 0.75f;
	case 2:
		return 1.f;
	case 3:
		return 1.25f;
	case 4:
		return 1.5f;
	case 5:
		return 1.75f;
	case 6:
		return 2.f;
	default:
		return 1.f;
	}
}

void GUI::RenderPopup(const int X, const int Y, const int W, const int H, 
					  const Color PopupBackground, const Color PopupAccent)
{
	Render::Renderer->DrawSetColor(PopupBackground);
	Render::Renderer->DrawFilledRect(X, Y, W, H);

	Render::Renderer->DrawSetColor(PopupAccent);
	Render::Renderer->DrawOutlinedRect(X, Y, W, H);
}