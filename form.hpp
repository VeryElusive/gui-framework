#pragma once

#include "tab.hpp"

#include "libutil/common/decl.h"
#include "libutil/containers/vector.h"

namespace GUI
{
	float GetDPIScale();
	void RenderPopup(const int X, const int Y, const int W, const int H, 
					const Datatypes::Color PopupBackground, const Datatypes::Color PopupAccent);

	extern
	int					DPIScale;

	class CForm;

	class CFocusItem
	{
	public:
		CItem			*Item;
		float			Animation;
		bool			FirstRun;
		bool			Active;
		bool			KeybindSelector;

		int 			GroupSizeX;

		int 			MousePositionX;
		int 			MousePositionY;

		int 			PositionX;
		int 			PositionY;

		void Update(CItem *const Item, 
					const int PositionX, const int PositionY, 
					const int MousePositionX, const int MousePositionY,
					const int GroupSizeX, 
					const bool KeybindSelector);

		void Reset();

		void Render(const float TimeSinceLastFrame, const float Alpha, 
                    const Color FormAccent, const float DPIScale);

		void RenderKeybindSelector(const int SizeX, const int SizeY,
					const float TimeSinceLastFrame, const float Alpha, 
                    const Datatypes::Color FormAccent, const float DPIScale);
	};

	class CForm : public CBaseItem
	{
	public:
		bool			Opened;

		void Render(const float TimeSinceLastFrame);
		void RenderTabs(const float TimeSinceLastFrame, const float Alpha);

		CTab *CreateNewTab(const wchar_t *Name);
		CForm(const int X, const int Y, 
			  const int W, const int H, 
			  const wchar_t *Name, 
			  Datatypes::Color Col, 
			  const bool Opened = false, 
			  const bool Resizable = false, 
			  const int MinSizeX = 0, const int MinSizeY = 0,
			  const int MaxSizeX = 10000, const int MaxSizeY = 10000);
			
		void Destroy();
	private:
		CFocusItem	FocusItem;

		float			Alpha;

		libutil_u8 		Dragging;

		int 			PositionX;
		int 			PositionY;

		int 			SizeX;
		int 			SizeY;

		bool			Resizeable;

		int				MaxSizeX;
		int				MaxSizeY;

		int				MinSizeX;
		int				MinSizeY;

		CTab			*SelectedTab;
		LIBUTIL_VECTOR 	Tabs;
			
		void HandleControls();
	};
}