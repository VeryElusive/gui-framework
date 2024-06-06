#pragma once

#include "item.hpp"

#include "items/dropdown.hpp"

#include "libutil/common/decl.h"
#include "libutil/containers/vector.h"

namespace GUI
{
	class CGroup
	{
	public:
		CGroup(const wchar_t *name, const float HorizontalPercentage, const float VerticalPercentage, const bool NewLine)
		: 	SizePercentageX(HorizontalPercentage), SizePercentageY(VerticalPercentage),
            NewLine(NewLine)
		{
			LibUtil_Vector_Setup(&Items, sizeof(CBaseItem *));

			LibUtil_Memcpy(this->Name, name, sizeof(Name));
			Name[19] = '\0';
			Scroll = 0;
		}

		void Destroy();

		void AddCheckbox(const wchar_t *Name, bool *Value, GUI::CKeyBind *Keybind);
		void AddDropdown(const wchar_t *Name, int *Value, GUI::CKeyBind *Keybind, wchar_t **Elements, const int ElementsCount);
		void AddListbox(const wchar_t *Name, int *Value, wchar_t **Elements, const int ElementsCount, const float MaxHeightPercentage);
		void AddMultiSelect(const wchar_t *Name, CMultiSelectItem *Elements, const int ElementsCount);
		void AddSlider(const wchar_t *Name, float *Value, GUI::CKeyBind *Keybind, const float Min, const float Max);
		void AddButton(const wchar_t *Name, void *Callback);
		void AddColorPicker(Datatypes::Color *Callback, const bool AlphaSelector = TRUE);
		void AddTextInput(const wchar_t *Name, char *const Buffer, const int BufferSize);


		LIBUTIL_VECTOR 	Items;
	private:
		bool 			NewLine;

		float 			SizePercentageX;
		float 			SizePercentageY;

		short 			Scroll;

		wchar_t			Name[20];

	public:
		bool Render(int& RenderPositionX, int& RenderPositionY,
                    const int FormSizeX, const int FormSizeY,
                    const int MaxHorizontalSlots, const int MaxVerticalSlots,
                     const float TimeSinceLastFrame, const float Alpha, 
					const Color FormAccent, const float DPIScale, 
					const bool AllowInput, CFocusItem *const FocusItem);
		void Reset();
	};
}