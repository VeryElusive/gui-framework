#pragma once

#include "specifications.hpp"

#include "../sdk/datatypes/color.hpp"

#include "libutil/common/memlib.h"
#include "libutil/common/strlib.h"
#include "libutil/common/decl.h"
#include "libutil/containers/vector.h"

namespace GUI
{
	class CFocusItem;

	enum EItemType
	{
		CHECKBOX,
		SLIDER,
		DROPDOWN,
		MULTISELECT,
		COLOR_PICKER,
		BUTTON,
		LIST_BOX,
		TEXT_INPUT
	};

	enum EKeyMode 
	{
		TOGGLE,
		HOLD
	};

	union ItemValue_t
	{
		bool 	B;
		int 	I = 0;
		float	F;
	};

	class CKeyBind
	{
	public:
		CKeyBind() {};
		int 			Key				= -1;
		EKeyMode 		Mode 			= EKeyMode::HOLD;

		ItemValue_t		ChangeValue;
	};

	class CBaseItem
	{
	public:
		CBaseItem() : Color(255,255,255) {};
		CBaseItem(const wchar_t *name, Color Color = Datatypes::Color(255,255,255))
		{
			Init(name, Color);
		}

		void Init(const wchar_t *name, Color Color = Datatypes::Color(255,255,255))
		{
			this->Color = Color;

			const auto Size = LibUtil_WStringNLength(name, 20);
			LibUtil_Memcpy(this->Name, name, LibUtil_MinI32(Size * sizeof(wchar_t), sizeof(Name)));

			Name[19] = '\0';
		}

		Color			Color;
		wchar_t			Name[20] = {'\0'};
	};

	class CItem : public CBaseItem
	{
	public:
		CItem(const wchar_t *name, void * Value, CKeyBind * Keybind, EItemType Type) 
		: CBaseItem(name, Datatypes::Color(255,255,255)), 
		  Value(Value), Type(Type), Keybind(Keybind) {}

		EItemType 		Type;

		void 	   		*Value;

		CKeyBind		*Keybind;

		void Reset();
		bool Render(const int RenderPositionX, int& RenderPositionY,
					const int GroupSizeX, const int GroupSizeY,
					const float FocusAnimation,
                    const float TimeSinceLastFrame, const float Alpha, 
					const Datatypes::Color FormAccent, const float DPIScale, 
					bool AllowKeybinds,
					const bool AllowInput, CFocusItem *const FocusItem);

	private:
		void OpenKeybindSelector();
		CKeyBind *CreateNewKeybind(int Key, EKeyMode Mode);
	};
}