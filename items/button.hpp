#ifndef __CHEAT_GUI_ITEM_BUTTON__
#define __CHEAT_GUI_ITEM_BUTTON__

#include "../item.hpp"

#include "libutil/common/decl.h"
#include "libutil/containers/vector.h"

namespace Cheat
{
    namespace GUI
	{
		class CButton : public CItem
		{
		public:
			CButton(const wchar_t *Name, void *Callback) 
				: CItem(Name, Callback, NULL, EItemType::BUTTON), 
				ConfirmationCountdown(0.f) {}

			bool Render(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput);
			
			float ConfirmationCountdown;
		};

		bool DrawButton(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
						const wchar_t *Name, void *Callback, 
						float& ConfirmationCountdown, Cheat::Datatypes::Color& Color,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput);
	}
}

#endif