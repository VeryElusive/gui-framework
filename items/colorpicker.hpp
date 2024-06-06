#ifndef __CHEAT_GUI_ITEM_COLORPICKER__
#define __CHEAT_GUI_ITEM_COLORPICKER__

#include "../item.hpp"

#include "libutil/common/decl.h"
#include "libutil/containers/vector.h"

namespace Cheat
{
    namespace GUI
	{
		class CColorPicker : public CItem
		{
		public:
			CColorPicker(Cheat::Datatypes::Color *Value, const bool SelectAlpha) 
				: CItem(L"", Value, NULL, EItemType::COLOR_PICKER), 
				SelectAlpha(SelectAlpha), SelectingState(0) {}

			bool Render(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
						const float FocusAnimation,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput, CFocusItem *const FocusItem);
			
			bool 		SelectAlpha;

			libutil_u8  SelectingState;
		};

		bool DrawColorPicker(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
						const wchar_t *Name, Cheat::Datatypes::Color *Value,
						const bool SelectAlpha,
						libutil_u8& SelectingState,
						const float FocusAnimation,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput, 
						CFocusItem *const FocusItem,
						CColorPicker * const ColorPicker);
	}
}

#endif