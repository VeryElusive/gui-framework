#include "item.hpp"
#include "form.hpp"

#include "items/button.hpp"
#include "items/checkbox.hpp"
#include "items/dropdown.hpp"
#include "items/slider.hpp"
#include "items/colorpicker.hpp"
#include "items/textinput.hpp"
#include "items/listbox.hpp"

bool GUI::CItem::Render(const int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX, const int GroupSizeY,
                        const float FocusAnimation,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Datatypes::Color FormAccent, const float DPIScale, 
                        bool AllowKeybinds,
						const bool AllowInput, CFocusItem *const FocusItem)
{
#if 0
    if(FocusItem && FocusItem->Item == this)
    {
        return;
    }
#endif

    if(Keybind == NULL)
    {
        AllowKeybinds = FALSE;
    }

	switch(Type)
    {
        case EItemType::CHECKBOX:
            return ((CCheckbox *)this)->Render(
                RenderPositionX, RenderPositionY, 
                GroupSizeX,
                TimeSinceLastFrame, Alpha,
                FormAccent, DPIScale, 
                AllowKeybinds,
                AllowInput, FocusItem
            );
        case EItemType::DROPDOWN:
            return ((CDropdown *)this)->Render(
                RenderPositionX, RenderPositionY, 
                GroupSizeX, FocusAnimation,
                TimeSinceLastFrame, Alpha,
                FormAccent, DPIScale, 
                AllowKeybinds,
                AllowInput, FocusItem
            );
        case EItemType::MULTISELECT:
            return ((CMultiSelect *)this)->Render(
                RenderPositionX, RenderPositionY, 
                GroupSizeX, FocusAnimation,
                TimeSinceLastFrame, Alpha,
                FormAccent, DPIScale, 
                AllowKeybinds,
                AllowInput, FocusItem
            );
        case EItemType::SLIDER:
            return ((CSlider *)this)->Render(
                RenderPositionX, RenderPositionY, 
                GroupSizeX,
                TimeSinceLastFrame, Alpha,
                FormAccent, DPIScale, 
                AllowKeybinds,
                AllowInput, FocusItem
            );
        case EItemType::BUTTON:
            return ((CButton *)this)->Render(
                RenderPositionX, RenderPositionY, 
                GroupSizeX,
                TimeSinceLastFrame, Alpha,
                FormAccent, DPIScale, 
                AllowInput
            );
        case EItemType::COLOR_PICKER:
            return ((CColorPicker *)this)->Render(
                RenderPositionX, RenderPositionY, 
                GroupSizeX,
                FocusAnimation,
                TimeSinceLastFrame, Alpha,
                FormAccent, DPIScale, 
                AllowInput, FocusItem
            );
        case EItemType::TEXT_INPUT:
            return ((CTextInput *)this)->Render(
                RenderPositionX, RenderPositionY, 
                GroupSizeX,
                FocusAnimation,
                TimeSinceLastFrame, Alpha,
                FormAccent, DPIScale, 
                AllowInput, FocusItem
            );

        case EItemType::LIST_BOX:
            return ((CListbox *)this)->Render(
                RenderPositionX, RenderPositionY,
				GroupSizeX, GroupSizeY,
                TimeSinceLastFrame, Alpha, 
				FormAccent, DPIScale, 
				AllowInput
            );
        default:
            return FALSE;
    }
}