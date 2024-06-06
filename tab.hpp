#pragma once

#include "group.hpp"

#include "libutil/common/decl.h"
#include "libutil/containers/vector.h"


namespace GUI
{
	class CSubTab : public CBaseItem
	{
	public:
		CSubTab(const wchar_t *Name, Datatypes::Color Color) : CBaseItem(Name, Color)
		{
			LibUtil_Vector_Setup(&Groups, sizeof(CGroup));
			LibUtil_Vector_Setup(&GroupSlots, sizeof(int));
		}

		CGroup *CreateGroup(const wchar_t *Name, 
				const float HorizontalPercentage, const float VerticalPercentage, 
				const bool NewLine = false);

        void RenderGroups(const int FormSizeX, const int FormSizeY,
                    const int FormPositionX, const int FormPositionY,
                    const float TimeSinceLastFrame, const float Alpha, 
					Datatypes::Color FormAccent, const float DPIScale, 
					bool AllowInput, CFocusItem *const FocusItem);

		void Destroy();

		LIBUTIL_VECTOR 	GroupSlots;

		LIBUTIL_VECTOR 	Groups;
	};

	class CTab : public CBaseItem
	{
	public:
		CTab(const wchar_t *Name, Datatypes::Color Color) : CBaseItem(Name, Color)
			{
			SelectedSubTab = 0;
			LibUtil_Vector_Setup(&SubTabs, sizeof(CSubTab));
		}

		GUI::CSubTab *CreateNewSubTab(const wchar_t *Name);
		LIBUTIL_VECTOR 	SubTabs;
        CSubTab *SelectedSubTab;

		void Destroy();
		void RenderSubTabs(const int FormPositionX, const int FormPositionY, 
					const int FormSizeX, const int FormSizeY, 
					const float TimeSinceLastFrame, const float Alpha, 
					Datatypes::Color FormColor);
	};
}