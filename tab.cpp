#include "tab.hpp"
#include "specifications.hpp"

#include "../input/mouse.hpp"
#include "../input/keys.hpp"

#include "../render/base.hpp"
#include "../render/font/base.hpp"

#include "libutil/common/strlib.h"
#include "libutil/libutil/math/int.h"

void GUI::CTab::RenderSubTabs(const int FormPositionX, const int FormPositionY, 
	const int FormSizeX, const int FormSizeY,
	const float TimeSinceLastFrame, float Alpha, Datatypes::Color FormColor)
{
	if(!SubTabs.Count)
	{
		return;
	}

	if(SelectedSubTab == NULL)
	{
		SelectedSubTab = (CSubTab *)LibUtil_Vector_GetElement(&SubTabs, 0);
	}

	int TW, TH;
    Render::Font::Hack->GetTextSizeFast(TW, TH, LibUtil_WStringLength(SelectedSubTab->Name), 1.f);

	const auto MaxWidth 		= FormSizeX - MARGIN * 2 - TW - 150;
	const auto DistributedSize 	= MaxWidth / (SubTabs.Count);
	const auto MouseLeftState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);

	for(int i = 0; i < SubTabs.Count; i++)
	{
		const auto SubTab = (CSubTab *)LibUtil_Vector_GetElement(&SubTabs, i);
		if(SubTab == NULL)
		{
			continue;
		}

		const auto PosX = 150 + FormPositionX + MARGIN + DistributedSize * i;
		const auto PosY = FormPositionY + MARGIN;

		if(SelectedSubTab == SubTab)
		{
			SubTab->Color = SubTab->Color.Lerp(FormColor, ANIMATION_SPEED);
		}
		else if(Input::IsMouseInBounds(PosX, PosY, TW, TH))
		{
			if(MouseLeftState.Pressed)
			{
				SelectedSubTab = SubTab;
			}
			
			SubTab->Color = SubTab->Color.Lerp(SELECTED_ITEM_COLOR_MENU, ANIMATION_SPEED);
		}
		else
		{
			SubTab->Color = SubTab->Color.Lerp(HOVERED_ITEM_COLOR_MENU, ANIMATION_SPEED);
		}

        Render::Font::Hack->DrawText(
            PosX,
            PosY,
            SubTab->Name,
            LibUtil_WStringLength(SubTab->Name),
            SubTab->Color.ScaleAlpha(Alpha).ToUInt(),
            Render::Font::FONT_SHADOW,
            1.f
        );
	}
}

void GUI::CSubTab::RenderGroups(const int FormSizeX, const int FormSizeY,
                          		const int FormPositionX, const int FormPositionY,
                          		const float TimeSinceLastFrame, const float Alpha,
								Datatypes::Color FormAccent, const float DPIScale, 
								bool AllowInput, CFocusItem *const FocusItem)
{
	if(Groups.Count == 0 || GroupSlots.Count == 0)
	{
		return;
	}

	const auto StartX = FormPositionX + SIDE_BAR_SIZE + MARGIN;

    int RenderPositionX = StartX;
    int RenderPositionY = FormPositionY + TOP_BAR_SIZE + MARGIN;

	int LineNumber = 0;
	int GroupSlotsX = *(int *)LibUtil_Vector_GetElement(&GroupSlots, LineNumber);
	for(int i = 0; i < Groups.Count; i++)
	{
		const auto Group = (CGroup *)LibUtil_Vector_GetElement(&Groups, i);
		if(!Group)
		{
			continue;
		}

		if(Group->Render(RenderPositionX, RenderPositionY,
					FormSizeX, FormSizeY,
					GroupSlotsX, GroupSlots.Count,
					TimeSinceLastFrame, Alpha, 
					FormAccent, DPIScale, 
					AllowInput, FocusItem))
		{
			RenderPositionX = StartX;
			LineNumber += 1;
			if(LineNumber < GroupSlots.Count)
			{
				GroupSlotsX = *(int *)LibUtil_Vector_GetElement(&GroupSlots, LineNumber);
			}
		}
	}
}

GUI::CSubTab *GUI::CTab::CreateNewSubTab(const wchar_t *Name)
{
	CSubTab NewSubTab(Name, Datatypes::Color(DIM_ITEM_COLOR, 255));
	LibUtil_Vector_PushBack(&SubTabs, &NewSubTab);

	return (CSubTab *)LibUtil_Vector_GetElement(&SubTabs, SubTabs.Count - 1);
}

GUI::CGroup *GUI::CSubTab::CreateGroup(const wchar_t *Name, 
	const float HorizontalPercentage, const float VerticalPercentage, const bool NewLine)
{
	CGroup NewGroup(Name, HorizontalPercentage, VerticalPercentage, NewLine);
	LibUtil_Vector_PushBack(&Groups, &NewGroup);

	if(!GroupSlots.Count)
	{
		int TMP = 0;
		LibUtil_Vector_PushBack(&GroupSlots, &TMP);
	}

	auto HorizontalSlots = (int *)LibUtil_Vector_GetElement(&GroupSlots, GroupSlots.Count - 1);
	*HorizontalSlots += 1;

	if(NewLine)
	{
		int TMP = 0;
		LibUtil_Vector_PushBack(&GroupSlots, &TMP);
	}

	return (CGroup *)LibUtil_Vector_GetElement(&Groups, Groups.Count - 1);
}

void GUI::CSubTab::Destroy()
{
	for(int i = 0; i < Groups.Count; i++)
	{
		const auto Group = (CGroup *)LibUtil_Vector_GetElement(&Groups, i);
		if(!Group)
		{
			continue;
		}

		Group->Destroy();
	}

	LibUtil_Vector_Destroy(&Groups);
	LibUtil_Vector_Destroy(&GroupSlots);
}

void GUI::CTab::Destroy()
{
	for(int i = 0; i < SubTabs.Count; i++)
	{
		const auto SubTab = (CSubTab *)LibUtil_Vector_GetElement(&SubTabs, i);
		if(!SubTab)
		{
			continue;
		}

		SubTab->Destroy();
	}

	LibUtil_Vector_Destroy(&SubTabs);
}