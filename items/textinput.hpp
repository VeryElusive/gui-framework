#ifndef __CHEAT_GUI_ITEM_TEXTINPUT__
#define __CHEAT_GUI_ITEM_TEXTINPUT__

#include "../item.hpp"

#include "libutil/common/decl.h"
#include "libutil/containers/vector.h"

namespace Cheat
{
    namespace GUI
	{
		class CTextInput : public CItem
		{
		public:
			CTextInput(const wchar_t *Name, char *const Buf, int MaxBuf) 
				: CItem(Name, Buf, NULL, EItemType::TEXT_INPUT), 
				BufferSize(MaxBuf), CursorFade(FALSE), CursorStart(0.f), CursorPosition(0) {}

			bool Render(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
						const float Animation,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput, CFocusItem *const FocusItem);
			
			int 	BufferSize;

			float 	CursorStart;
			int 	CursorPosition;
			bool 	CursorFade;
		};

		bool DrawTextInput(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
						const wchar_t *Name, char *Value, const int BufferSize,
						const float Animation,
						float& CursorStart, 
						bool& CursorFade, 
						int& CursorPosition, 
						Cheat::Datatypes::Color& Color,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput, CFocusItem *const FocusItem,
						CTextInput *const TextInput);
	}
}

#endif