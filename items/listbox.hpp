#ifndef __CHEAT_GUI_ITEM_LISTBOX__
#define __CHEAT_GUI_ITEM_LISTBOX__

#include "../item.hpp"

#include "libutil/common/decl.h"
#include "libutil/containers/vector.h"
#include "libutil/common/memlib.h"
#include "libutil/common/memheap.h"

namespace Cheat
{
    namespace GUI
	{
		class CListbox : public CItem
		{
		public:
			CListbox(const wchar_t *Name, 
				int *const Value, wchar_t ** E, 
				const int EC, 
				const float MaxHeightPercentage) 
				: CItem(Name, Value, NULL, EItemType::LIST_BOX), 
					ElementsCount(EC), MaxHeightPercentage(MaxHeightPercentage), Scroll(0.f)
				{
					Elements 		= (CBaseItem *)LibUtil_Heap_Allocate(EC * sizeof(CBaseItem));
					LibUtil_Memset(Elements, 0, EC * sizeof(CBaseItem));
					for(int i = 0; i < EC; i++)
					{
						Elements[i].Init(E[i]);
					}
				}

			bool Render(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX, const int GroupSizeY,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput);
			
			CBaseItem 	*Elements;
			int 		ElementsCount;

			float 		MaxHeightPercentage;
			int 		Scroll;
		};

		bool DrawListbox(wchar_t *const Name,
						int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX, const int GroupSizeY,
						int *const Value,
						CBaseItem *const Elements, const int ElementsCount,
						int& Scroll,
						const float MaxHeightPercentage,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput);
	}
}

#endif