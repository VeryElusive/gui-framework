#ifndef __CHEAT_GUI_ITEM_CHECKBOX__
#define __CHEAT_GUI_ITEM_CHECKBOX__

#include "../item.hpp"

#include "libutil/common/decl.h"
#include "libutil/containers/vector.h"

namespace Cheat
{
    namespace GUI
	{
		class CCheckbox : public CItem
		{
		public:
			bool Render(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						bool AllowKeybinds,
						const bool AllowInput, CFocusItem *const FocusItem);
		};

		bool DrawCheckbox(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
						const wchar_t *Name, bool *Value, 
						bool AllowKeybinds, 
						Cheat::Datatypes::Color& Color,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput, 
						CFocusItem *const FocusItem, CCheckbox * const Checkbox);
	}
}

#endif