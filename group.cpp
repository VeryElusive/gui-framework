#include "group.hpp"
#include "form.hpp"
#include "items/dropdown.hpp"
#include "items/slider.hpp"
#include "items/button.hpp"
#include "items/colorpicker.hpp"
#include "items/textinput.hpp"
#include "items/checkbox.hpp"
#include "items/listbox.hpp"

#include "../render/base.hpp"
#include "../render/font/base.hpp"

#include "../input/mouse.hpp"
#include "../input/keys.hpp"

#include "libutil/common/memheap.h"

bool GUI::CGroup::Render(int& RenderPositionX, int& RenderPositionY,
                                const int FormSizeX, const int FormSizeY,
                                const int MaxHorizontalSlots, const int MaxVerticalSlots,
                                const float TimeSinceLastFrame, const float Alpha, 
                                const Color FormAccent, const float DPIScale, 
                                const bool AllowInput, CFocusItem *const FocusItem)
{
	const int SizeX         = (int)((FormSizeX - SIDE_BAR_SIZE - (MARGIN * (MaxHorizontalSlots + 1))) * SizePercentageX);
    const int SizeY         = (int)((FormSizeY - TOP_BAR_SIZE  - MARGIN *  (MaxVerticalSlots   + 1)) * SizePercentageY);
    
    const auto Hovered      = Input::IsMouseInBounds(RenderPositionX, RenderPositionY, SizeX, SizeY);
    const auto ScrollState  = Input::GetMouseWheelState();

    const auto Padding 	= (int)(PADDING * DPIScale);

    Render::Renderer->DrawSetColor(GROUP_COLOR_MENU);
    Render::Renderer->DrawFilledRect(RenderPositionX, RenderPositionY, SizeX, SizeY);

	Render::Renderer->DrawSetColor(FormAccent.ScaleAlpha(.3f));
    Render::Renderer->DrawOutlinedRect(RenderPositionX, RenderPositionY, SizeX, SizeY);

    const auto BackupRenderPositionY = RenderPositionY;

    int BackupX, BackupY, BackupW, BackupH;
    Render::Renderer->GetClippingRect(BackupX, BackupY, BackupW, BackupH);
    Render::Renderer->SetClippingRect(RenderPositionX + 1, RenderPositionY + 1, SizeX - 2, SizeY - 2);

    RenderPositionX += PADDING * 2;
    RenderPositionY += Padding * 2 + Scroll;

    for(int i = 0; i < Items.Count; i++)
    {
        const auto Item = *(CItem **)LibUtil_Vector_GetElement(&Items, i);
		if(!Item)
		{
			continue;
		}

        const auto ItemRet = Item->Render(
            RenderPositionX, RenderPositionY, 
            SizeX, SizeY,
            0.f,
            TimeSinceLastFrame, Alpha,
            FormAccent, DPIScale, 
            true,
            AllowInput, FocusItem
        );

        if(Item->Type == EItemType::LIST_BOX && ItemRet)
        {
            Scroll -= ScrollState * 12;
        }
    }

    Render::Renderer->DrawFilledGradientRect(RenderPositionX + 1, BackupRenderPositionY + SizeY - Padding * 2, 
                                            SizeX - 2, Padding * 4,
                                            FALSE,
                                            GROUP_COLOR_MENU.ScaleAlpha(0), 
                                            GROUP_COLOR_MENU);

    Render::Renderer->DrawFilledGradientRect(RenderPositionX + 1, BackupRenderPositionY, 
                                            SizeX - 2, Padding * 4,
                                            FALSE,
                                            GROUP_COLOR_MENU, 
                                            GROUP_COLOR_MENU.ScaleAlpha(0));

    const auto BackupScroll = Scroll;

    if(Hovered)
    {
        Scroll                  += ScrollState * 12;

        if(ScrollState)
        {
            FocusItem->Active    = FALSE;
            FocusItem->Animation = 0.f;
        }
    }

    const auto Outside = LibUtil_MaxI32((RenderPositionY + Padding) - Scroll - (BackupRenderPositionY + SizeY), 0);
    Scroll = LibUtil_LerpI32(Scroll, LibUtil_ClampI32(Scroll, -Outside, 0), ANIMATION_SPEED * 2);

    if(BackupScroll != Scroll && FocusItem)
    {
        for(int i = 0; i < Items.Count; i++)
        {
            if(FocusItem->Item == *(CItem **)LibUtil_Vector_GetElement(&Items, i))
            {
                FocusItem->PositionY += Scroll - BackupScroll;
                break;
            }
        }

    }

    if(Outside)
    {
        Render::Renderer->DrawSetColor(FormAccent);
        Render::Renderer->DrawFilledRect(RenderPositionX + SizeX - PADDING * 2 - 4, BackupRenderPositionY - LibUtil_MinI32(Scroll, 0), 4, (int)(SizeY * (1.f - (float)Outside / (float)SizeY)));
    }

    Render::Renderer->SetClippingRect(BackupX, BackupY, BackupW, BackupH);

    int TW, TH;
    Render::Font::Hack->GetTextSizeFast(TW, TH, LibUtil_WStringLength(Name), 0.7f * DPIScale);

    Render::Font::Hack->DrawText(
        RenderPositionX + PADDING, BackupRenderPositionY - (int)(TH / 2),
        Name, LibUtil_WStringLength(Name),
        Color(255,255,255).ScaleAlpha(Alpha).ToUInt(),
        Render::Font::FONT_SHADOW,
        0.7f * DPIScale
    );

    RenderPositionX += SizeX + MARGIN - PADDING * 2;
    RenderPositionY = BackupRenderPositionY;
    if(NewLine)
    {
        RenderPositionY += SizeY + MARGIN;
		return true;
    }

	return false;
}

void GUI::CGroup::Destroy()
{
    for(int i = 0; i < Items.Count; i++)
    {
        const auto Item = *(CItem **)LibUtil_Vector_GetElement(&Items, i);
		if(!Item)
		{
			continue;
		}

        switch(Item->Type)
        {
        case EItemType::DROPDOWN:
            ((CDropdown *)Item)->Destroy();
            break;
        case EItemType::MULTISELECT:
            ((CMultiSelect *)Item)->Destroy();
            break;
        default:
            break;
        }
        
        LibUtil_Heap_Free(Item);
    }

    LibUtil_Vector_Destroy(&Items);
}

void GUI::CGroup::AddCheckbox(const wchar_t *Name, bool *Value, GUI::CKeyBind *Keybind)
{
    const auto Allocation = (CItem *)LibUtil_Heap_Allocate(sizeof(CItem));
    LibUtil_Memset(Allocation, 0, sizeof(CItem));
    *Allocation = CItem(Name, Value, Keybind, EItemType::CHECKBOX);
    LibUtil_Vector_PushBack(&Items, &Allocation);
}

void GUI::CGroup::AddColorPicker(Datatypes::Color *Value, const bool AlphaSelector)
{
    const auto Allocation = (CColorPicker *)LibUtil_Heap_Allocate(sizeof(CColorPicker));
    LibUtil_Memset(Allocation, 0, sizeof(CColorPicker));
    *Allocation = CColorPicker(Value, AlphaSelector);
    LibUtil_Vector_PushBack(&Items, &Allocation);
}

void GUI::CGroup::AddSlider(const wchar_t *Name, 
                                float *Value, GUI::CKeyBind *Keybind, 
                                const float Min, const float Max)
{
    const auto Allocation = (CSlider *)LibUtil_Heap_Allocate(sizeof(CSlider));
    LibUtil_Memset(Allocation, 0, sizeof(CSlider));
    *Allocation = CSlider(Name, Value, Keybind, Min, Max);
    LibUtil_Vector_PushBack(&Items, &Allocation);
}

void GUI::CGroup::AddDropdown(const wchar_t *Name, 
                                int *Value, GUI::CKeyBind *Keybind, 
                                wchar_t **Elements, const int ElementsCount)
{
    const auto Allocation = (CDropdown *)LibUtil_Heap_Allocate(sizeof(CDropdown));
    LibUtil_Memset(Allocation, 0, sizeof(CDropdown));
    *Allocation = CDropdown(Name, Value, Keybind, Elements, ElementsCount);
    LibUtil_Vector_PushBack(&Items, &Allocation);
}

void GUI::CGroup::AddMultiSelect(const wchar_t *Name,    
                                    CMultiSelectItem *Elements, const int ElementsCount)
{
    const auto Allocation = (CMultiSelect *)LibUtil_Heap_Allocate(sizeof(CMultiSelect));
    LibUtil_Memset(Allocation, 0, sizeof(CMultiSelect));
    *Allocation = CMultiSelect(Name, Elements, ElementsCount);
    LibUtil_Vector_PushBack(&Items, &Allocation);
}

void GUI::CGroup::AddButton(const wchar_t *Name, void *Callback)
{
    const auto Allocation = (CButton *)LibUtil_Heap_Allocate(sizeof(CButton));
    LibUtil_Memset(Allocation, 0, sizeof(CButton));
    *Allocation = CButton(Name, Callback);
    LibUtil_Vector_PushBack(&Items, &Allocation);
}

void GUI::CGroup::AddTextInput(const wchar_t *Name, 
                                    char *const Buffer, const int BufferSize)
{
    const auto Allocation = (CTextInput *)LibUtil_Heap_Allocate(sizeof(CTextInput));
    LibUtil_Memset(Allocation, 0, sizeof(CTextInput));
    *Allocation = CTextInput(Name, Buffer, BufferSize);
    LibUtil_Vector_PushBack(&Items, &Allocation);
}

void GUI::CGroup::AddListbox(const wchar_t *Name, 
                                int *Value, 
                                wchar_t **Elements, const int ElementsCount, 
                                const float MaxHeightPercentage)
{
    const auto Allocation = (CListbox *)LibUtil_Heap_Allocate(sizeof(CListbox));
    LibUtil_Memset(Allocation, 0, sizeof(CListbox));
    *Allocation = CListbox(Name, Value, Elements, ElementsCount, MaxHeightPercentage);
    LibUtil_Vector_PushBack(&Items, &Allocation);
}