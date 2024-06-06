#ifndef __CHEAT_GUI_ITEM_SLIDER__
#define __CHEAT_GUI_ITEM_SLIDER__

#include "../item.hpp"

#include "libutil/common/decl.h"
#include "libutil/containers/vector.h"

namespace Cheat
{
    namespace GUI
	{
		class CSlider : public CItem
		{
		public:
			CSlider(const wchar_t *name, float *Value, GUI::CKeyBind *Keybind, const float Min, const float Max) 
			: CItem(name, Value, Keybind, EItemType::SLIDER)
			{
				MinValue = Min;
				MaxValue = Max;
				InterpolatedValue = 0.f;
				Selected = FALSE;
			}

			bool Render(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						bool AllowKeybinds,
						const bool AllowInput, CFocusItem *const FocusItem);

			float MinValue;
			float MaxValue;
			float InterpolatedValue;
			bool  Selected;
		};

		bool DrawSlider(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
						const wchar_t *Name, float *Value, 
						float& InterpolatedValue, const float MinValue, const float MaxValue,
						bool& Selected, 
						Cheat::Datatypes::Color& Color,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput, bool AllowKeybinds,
						CFocusItem *const FocusItem, CSlider *Slider);
	}
}

#endif