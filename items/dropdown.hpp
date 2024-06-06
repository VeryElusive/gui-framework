#ifndef __CHEAT_GUI_ITEM_DROPDOWN__
#define __CHEAT_GUI_ITEM_DROPDOWN__

#include "../item.hpp"

#include "libutil/common/decl.h"
#include "libutil/common/memlib.h"
#include "libutil/common/memheap.h"
#include "libutil/containers/vector.h"

namespace Cheat
{
    namespace GUI
	{
		class CMultiSelectItem : public CBaseItem
		{
		public:
			CMultiSelectItem() : CBaseItem() {Value = 0;};
			CMultiSelectItem(const wchar_t *Name, bool *Value) 
			: CBaseItem(Name, Cheat::Datatypes::Color(DIM_ITEM_COLOR)), Value(Value) {}

			void Init(const wchar_t *Name, bool *Value)
			{
				CBaseItem::Init(Name, Cheat::Datatypes::Color(DIM_ITEM_COLOR));
				this->Value = Value;
			}

			bool *Value;
		};

		class CMultiSelect : public CItem
		{
		public:
			CMultiSelect(const wchar_t *name, CMultiSelectItem *E, int EC) 
			: CItem(name, Value, NULL, EItemType::MULTISELECT)
			{
				ElementsCount 	= EC;
				
				Elements 		= (CMultiSelectItem *)LibUtil_Heap_Allocate(EC * sizeof(CMultiSelectItem));
				LibUtil_Memset(Elements, 0, EC * sizeof(CMultiSelectItem));
				for(int i = 0; i < EC; i++)
				{
					Elements[i].Init(E[i].Name, E[i].Value);
				}
			}

			void Destroy();

			bool Render(int RenderPositionX, int& RenderPositionY,
						int GroupSizeX, const float FocusAnimation,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						bool AllowKeybinds,
						const bool AllowInput, CFocusItem *const FocusItem);

			CMultiSelectItem	*Elements;
			int					ElementsCount;
		};

		class CDropdown : public CItem
		{
		public:
			CDropdown(const wchar_t *name, int *Value, CKeyBind *Keybind, wchar_t **E, int EC) 
			: CItem(name, Value, Keybind, EItemType::DROPDOWN)
			{
				ElementsCount 	= EC;

				Elements 		= (CBaseItem *)LibUtil_Heap_Allocate(EC * sizeof(CBaseItem));
				LibUtil_Memset(Elements, 0, EC * sizeof(CBaseItem));
				for(int i = 0; i < EC; i++)
				{
					Elements[i].Init(E[i]);
				}
			}

			void Destroy();

			bool Render(int RenderPositionX, int& RenderPositionY,
						int GroupSizeX, const float FocusAnimation,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						bool AllowKeybinds,
						const bool AllowInput, CFocusItem *const FocusItem);

			CBaseItem			*Elements;
			int					ElementsCount;
		};
		
		bool DrawDropDown(int RenderPositionX, int& RenderPositionY,
						int GroupSizeX, const float Animation,
						const wchar_t *Name, int *Value, 
						const int ElementsCount, CBaseItem *const Elements, 
						const bool MultiSelect,
						Cheat::Datatypes::Color& Col, 
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput, const bool AllowKeybinds,
						CFocusItem *const FocusItem, CItem *const Dropdown);
	}
}

#endif