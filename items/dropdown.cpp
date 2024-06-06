#include "dropdown.hpp"
#include "../form.hpp"

#include "../../input/mouse.hpp"
#include "../../input/keys.hpp"

#include "../../render/base.hpp"
#include "../../render/font/base.hpp"

using namespace Cheat;

bool GUI::CDropdown::Render(int RenderPositionX, int& RenderPositionY,
						int GroupSizeX, const float FocusAnimation,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowKeybinds,
						const bool AllowInput, CFocusItem *const FocusItem)
{
	return DrawDropDown(
		RenderPositionX, RenderPositionY, 
		GroupSizeX, FocusAnimation,
		this->Name, (int*)this->Value, 
		ElementsCount, Elements, 
		FALSE,
		Color, 
		TimeSinceLastFrame, Alpha, 
		FormAccent, DPIScale, 
		AllowInput, AllowKeybinds,
		FocusItem, this
	);
}

void GUI::CDropdown::Destroy()
{
	LibUtil_Heap_Free(Elements);
}

bool GUI::CMultiSelect::Render(int RenderPositionX, int& RenderPositionY,
						int GroupSizeX, const float FocusAnimation,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowKeybinds,
						const bool AllowInput, CFocusItem *const FocusItem)
{
	return DrawDropDown(
		RenderPositionX, RenderPositionY, 
		GroupSizeX, FocusAnimation,
		this->Name, NULL, 
		ElementsCount, Elements, 
		TRUE,
		Color, 
		TimeSinceLastFrame, Alpha, 
		FormAccent, DPIScale, 
		AllowInput, AllowKeybinds,
		FocusItem, this
	);
}

void GUI::CMultiSelect::Destroy()
{
	LibUtil_Heap_Free(Elements);
}

bool DropDownHandleInput(int RenderPositionX, int& RenderPositionY,
				const int RenderX, const int GroupSizeX,
				const int SizeX, const int SizeY,
				const float Animation,
				const wchar_t *Name, int *Value, 
				const int ElementsCount, GUI::CBaseItem *const Elements, 
				wchar_t *PresentedName, int& PresentedNameLength, 
				const int DropDownSize,
				const bool MultiSelect,
				Datatypes::Color& Col, 
                const float TimeSinceLastFrame, const float Alpha, 
				const Datatypes::Color FormAccent, const float DPIScale, 
				const bool AllowInput, const bool AllowKeybinds,
				GUI::CFocusItem *const FocusItem, GUI::CItem *const Dropdown);

void RenderDropDown(int RenderPositionX, int& RenderPositionY,
				const int RenderX,
				const int SizeX, const int SizeY, const int DropdownSize,
				int GroupSizeX, 
				const float Animation,
				const wchar_t *Name, int *Value, 
				const wchar_t *PresentedName, const int PresentedNameLength,
				const int ElementsCount, GUI::CBaseItem *const Elements, 
				const bool MultiSelect,
				Datatypes::Color& Col, 
                const float TimeSinceLastFrame, const float Alpha, 
				const Datatypes::Color FormAccent, const float DPIScale, 
				const bool AllowInput, const bool AllowKeybinds,
				GUI::CFocusItem *const FocusItem, GUI::CItem *const Dropdown);

void RenderDropdownElement(const int RenderX, const int RenderY,
						const int SizeX,
						const int ElementGap,
						const int i, 
						const float DPIScale, const float Alpha,
						GUI::CBaseItem * const Element);

void DropdownElementHandleInput(const int i, 
						int *Value,
						GUI::CBaseItem * const Element,
						const int ElementGap, const int DropdownSize,
						const bool Hovered, const bool Clicking,
						const bool MultiSelect, 
						const Datatypes::Color FormAccent,
						const float TimeSinceLastFrame, const float Alpha);

bool GUI::DrawDropDown(int RenderPositionX, int& RenderPositionY,
						int GroupSizeX, const float Animation,
						const wchar_t *Name, int *Value, 
						const int ElementsCount, CBaseItem *const Elements, 
						const bool MultiSelect,
						Cheat::Datatypes::Color& Col, 
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput, const bool AllowKeybinds,
						CFocusItem *const FocusItem, CItem *const Dropdown)
{
	const auto SizeX 			= (GroupSizeX - PADDING * 4) / 3;
	const auto SizeY 			= (int)(ITEM_HEIGHT * 1.5f * DPIScale);
	const auto Padding 			= (int)(PADDING * DPIScale);
	const auto ElementGap 		= (int)(Padding * 1.5f);
	const auto GroupSizeMargin 	= GroupSizeX - PADDING * 4;
	const auto RenderX			= RenderPositionX + GroupSizeMargin - SizeX;
	const auto DropdownSize 	= ElementsCount * ElementGap * Animation;

	wchar_t PresentedName[32] = {'\0'};
	int PresentedNameLength;

	const auto Ret = DropDownHandleInput(
		RenderPositionX, RenderPositionY, 
		RenderX, GroupSizeX, 
		SizeX, SizeY, 
		Animation, 
		Name, 
		Value, ElementsCount, Elements, 
		PresentedName, PresentedNameLength, 
		DropdownSize, 
		MultiSelect, 
		Col, 
		TimeSinceLastFrame, Alpha, 
		FormAccent, DPIScale, 
		AllowInput, AllowKeybinds, 
		FocusItem, Dropdown
	);

	RenderDropDown(
		RenderPositionX, RenderPositionY, 
		RenderX, SizeX, SizeY, 
		DropdownSize, 
		GroupSizeX, 
		Animation, 
		Name, Value, 
		PresentedName, PresentedNameLength, 
		ElementsCount, Elements, 
		MultiSelect, 
		Col, 
		TimeSinceLastFrame, Alpha, 
		FormAccent, DPIScale, 
		AllowInput, AllowKeybinds, 
		FocusItem, Dropdown
	);

	if(Animation <= 0.01f)
	{
		RenderPositionY += SizeY + Padding;
		return Ret;
	}

	const auto RenderPosY = RenderPositionY + SizeY;

	Render::Renderer->SetClippingRect(RenderX, RenderPosY, SizeX, DropdownSize);

	const auto MouseLeftState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);
	for(int i = 0; i < ElementsCount; i++)
	{
		CBaseItem *Element;
		if(MultiSelect == TRUE)
		{
			Element = (CBaseItem *)&(((CMultiSelectItem *)Elements)[i]);
		}
		else
		{
			Element = &Elements[i];
		}

		const int PosY 		= RenderPosY + i * ElementGap;
		if(i * ElementGap > DropdownSize)
		{
			break;
		}

		const auto Hovered 		= Input::IsMouseInBounds(RenderX, PosY, SizeX, Padding);
		DropdownElementHandleInput(
			i, Value, Element,
			ElementGap, DropdownSize,
			Hovered, MouseLeftState.Pressed && !MouseLeftState.Trapped,
			MultiSelect, FormAccent,
			TimeSinceLastFrame, Alpha
		);

		RenderDropdownElement(
			RenderX + 1, PosY,
			SizeX - 2, ElementGap,
			i, DPIScale, Alpha, 
			Element
		);
	}

	Render::Renderer->SetClippingRect(RenderX, RenderPosY + DropdownSize, 99999, 99999);

	RenderPositionY += SizeY + Padding;
	return Ret;
}

void RenderDropDown(int RenderPositionX, int& RenderPositionY,
				const int RenderX,
				const int SizeX, const int SizeY, const int DropdownSize,
				int GroupSizeX, 
				const float Animation,
				const wchar_t *Name, int *Value, 
				const wchar_t *PresentedName, const int PresentedNameLength,
				const int ElementsCount, GUI::CBaseItem *const Elements, 
				const bool MultiSelect,
				Datatypes::Color& Col, 
                const float TimeSinceLastFrame, const float Alpha, 
				const Datatypes::Color FormAccent, const float DPIScale, 
				const bool AllowInput, const bool AllowKeybinds,
				GUI::CFocusItem *const FocusItem, GUI::CItem *const Dropdown)
{
	Render::Font::Hack->DrawText(
		RenderPositionX, RenderPositionY - 1,
		Name, LibUtil_WStringLength(Name),
		(Datatypes::Color(255,255,255).ScaleAlpha(Alpha)).ToUInt(),
		Render::Font::FONT_SHADOW,
		0.7f * DPIScale
	);

	Render::Font::Hack->DrawText(
		RenderX + (int)(SizeX / 2), RenderPositionY + 1,
		PresentedName, PresentedNameLength,
		(LABEL_COLOR_MENU.ScaleAlpha(Alpha)).ToUInt(),
		Render::Font::FONT_SHADOW | Render::Font::FONT_CENTER,
		0.7f * DPIScale
	);

	Render::Renderer->DrawSetColor(Col);
    Render::Renderer->DrawOutlinedRect(RenderX, RenderPositionY, SizeX, SizeY);

	if(Animation <= 0.01f)
	{
		return;
	}

	Render::Renderer->DrawSetColor(GROUP_COLOR_MENU);
	Render::Renderer->DrawFilledRect(RenderX, RenderPositionY + SizeY, SizeX, DropdownSize);

	Render::Renderer->DrawSetColor(OUTLINE_LIGHT_COLOR_MENU);
	Render::Renderer->DrawOutlinedRect(RenderX, RenderPositionY + SizeY, SizeX, DropdownSize);
}

void RenderDropdownElement(const int RenderX, const int RenderY,
						const int SizeX,
						const int ElementGap,
						const int i, 
						const float DPIScale, const float Alpha,
						GUI::CBaseItem * const Element)
{
	if(i % 2)
	{
		Render::Renderer->DrawSetColor(Cheat::Datatypes::Color(30, 30, 30, (int)(255.f * Alpha)));
		Render::Renderer->DrawFilledRect(RenderX, RenderY, SizeX, ElementGap);
	}

	Render::Font::Hack->DrawText(
		RenderX, RenderY - 1,
		Element->Name, LibUtil_WStringLength(Element->Name),
		Element->Color.ScaleAlpha(Alpha).ToUInt(),
		Render::Font::FONT_SHADOW,
		0.5f * DPIScale
	);
}

bool DropDownHandleInput(int RenderPositionX, int& RenderPositionY,
				const int RenderX, const int GroupSizeX,
				const int SizeX, const int SizeY,
				const float Animation,
				const wchar_t *Name, int *Value, 
				const int ElementsCount, GUI::CBaseItem *const Elements, 
				wchar_t *PresentedName, int& PresentedNameLength, 
				const int DropdownSize,
				const bool MultiSelect,
				Datatypes::Color& Col, 
                const float TimeSinceLastFrame, const float Alpha, 
				const Datatypes::Color FormAccent, const float DPIScale, 
				const bool AllowInput, const bool AllowKeybinds,
				GUI::CFocusItem *const FocusItem, GUI::CItem *const Dropdown)
{
	bool Ret = FALSE;
	const auto MouseLeftState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);
	const auto MouseRightState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_RIGHT);
	const auto Hovered 			= Input::IsMouseInBounds(RenderX, RenderPositionY, SizeX, SizeY + DropdownSize);

	if(Animation > 0.1f)
	{
		if(MouseLeftState.Pressed && !MouseLeftState.Trapped)
		{
			if(!Input::IsMouseInBounds(RenderX, RenderPositionY + SizeY, SizeX, DropdownSize)
				|| MultiSelect == FALSE)
			{
				if(FocusItem)
				{
					FocusItem->Active = FALSE;
				}

				Ret = TRUE;
			}
		}
	}
	else if(Hovered && AllowInput)
	{
		if(MouseLeftState.Pressed && !MouseLeftState.Trapped)
		{
			if(FocusItem)
			{
				FocusItem->Update(
					Dropdown, 
					RenderPositionX, 
					RenderPositionY, 
					GroupSizeX,
					0, 0, 
					FALSE
				);
			}
				
			Ret = TRUE;
		}
		else if(AllowKeybinds && MouseRightState.Pressed && FocusItem)
		{
			int MousePosX, MousePosY;
			Input::GetMousePos(MousePosX, MousePosY);

			FocusItem->Update(
				Dropdown, 
				RenderPositionX, 
				RenderPositionY, 
				GroupSizeX,
				MousePosX, MousePosY, 
				TRUE
			);
		}
	}
	
	if(Animation)
	{
		Col = Col.Lerp(SELECTED_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}
	else if(Hovered && AllowInput)
	{
		Col = Col.Lerp(SELECTED_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}
	else 
	{
		Col = Col.Lerp(HOVERED_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}

	if(MultiSelect == FALSE)
	{
		if(*Value < ElementsCount)
		{
			LibUtil_Memcpy((void *)PresentedName, Elements[*Value].Name, 32 * sizeof(wchar_t));
		}
	}
	else
	{
		libutil_size NameCharacters = 0;
		for(int i = 0; i < ElementsCount; i++)
		{
			GUI::CMultiSelectItem *Element = &(((GUI::CMultiSelectItem *)Elements)[i]);
			if(*Element->Value == FALSE)
			{
				continue;
			}

			if(NameCharacters != 0)
			{
				PresentedName[NameCharacters] = ',';
				PresentedName[NameCharacters + 1] = ' ';
				NameCharacters += 2;
			}

			auto NameLength = LibUtil_WStringLength(Element->Name);
			const auto MaxSlotsRemaining = 32 - NameCharacters;

			LibUtil_Memcpy(
				&PresentedName[NameCharacters], 
				Element->Name, 
				LibUtil_MinF(MaxSlotsRemaining, NameLength) * sizeof(wchar_t)
			);

			if(MaxSlotsRemaining <= NameLength)
			{
				break;
			}

			NameCharacters += NameLength;
		}
	}

	PresentedNameLength = LibUtil_WStringLength(PresentedName);
	
	int TW, TH;
	Render::Font::Hack->GetTextSize(TW, TH, 
			PresentedName, PresentedNameLength, 
			0.7f * DPIScale);

	while(TW > SizeX - 20)
	{
		PresentedName[PresentedNameLength - 1] = '\0';
		PresentedName[PresentedNameLength - 2] = '.';
		PresentedName[PresentedNameLength - 3] = '.';
		PresentedName[PresentedNameLength - 4] = '.';
		PresentedNameLength -= 1;

		Render::Font::Hack->GetTextSize(TW, TH, 
			PresentedName, PresentedNameLength, 
			0.7f * DPIScale);
	}

	return Ret;
}

void DropdownElementHandleInput(const int i, 
						int *Value,
						GUI::CBaseItem * const Element,
						const int ElementGap, const int DropdownSize,
						const bool Hovered, const bool Clicking,
						const bool MultiSelect, 
						const Datatypes::Color FormAccent,
						const float TimeSinceLastFrame, const float Alpha)
{
	if(Hovered && Clicking)
	{
		if(MultiSelect == TRUE)
		{
			*((GUI::CMultiSelectItem *)Element)->Value = !*((GUI::CMultiSelectItem *)Element)->Value;
		}
		else
		{
			*Value = i;
		}
	}

	if((MultiSelect == TRUE
		&& *((GUI::CMultiSelectItem *)Element)->Value == TRUE)
			|| (MultiSelect == FALSE 
			&& i == *Value))
	{
		Element->Color = Element->Color.Lerp(FormAccent, ANIMATION_SPEED);
	}
	else if(Hovered)
	{
		Element->Color = Element->Color.Lerp(HOVERED_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}
	else 
	{
		Element->Color = Element->Color.Lerp(DIM_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}
}