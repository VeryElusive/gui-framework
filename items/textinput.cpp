#include "textinput.hpp"
#include "../form.hpp"

#include "../../input/mouse.hpp"
#include "../../input/keys.hpp"

#include "../../render/base.hpp"
#include "../../render/font/base.hpp"

#include "../../sdk/source/interfaces/tier0/init.hpp"

using namespace Cheat;

bool GUI::CTextInput::Render(int RenderPositionX, int& RenderPositionY,
						const int GroupSizeX,
						const float Animation,
                        const float TimeSinceLastFrame, const float Alpha, 
						const Cheat::Datatypes::Color FormAccent, const float DPIScale, 
						const bool AllowInput, CFocusItem *const FocusItem)
{
	return DrawTextInput(RenderPositionX, RenderPositionY,
					GroupSizeX,
					Name, (char *)Value, BufferSize,
					Animation,
					CursorStart,
					CursorFade,
					CursorPosition,
					Color,
                    TimeSinceLastFrame, Alpha, 
					FormAccent, DPIScale, 
					AllowInput, FocusItem,
					this);
}

void AddCharacter(char Character, char *Value, int& TextLength, int& CursorPosition, float& CursorStart, bool& CursorFade);

void RenderTextInput(int RenderPositionX, int& RenderPositionY,
					const int SizeX, const int SizeY,
					const int TextLength,
					const int GroupSizeX,
					const wchar_t *Name, char *Value, const int BufferSize,
					const float Animation,
					float& CursorStart, 
					bool& CursorFade,
					int& CursorPosition, 
					Cheat::Datatypes::Color& Color,
                    const float TimeSinceLastFrame, const float Alpha, 
					const Cheat::Datatypes::Color FormAccent, const float DPIScale);

void TextInputHandleInput(int RenderPositionX, int& RenderPositionY,
					const int SizeX, const int SizeY,
					int& TextLength,
					const int GroupSizeX,
					const wchar_t *Name, char *Value, const int BufferSize,
					const float Animation,
					float& CursorStart, 
					bool& CursorFade,
					int& CursorPosition, 
					Cheat::Datatypes::Color& Color, 
					const float TimeSinceLastFrame, const float Alpha,
					const float DPIScale,
					const bool AllowInput, GUI::CFocusItem *const FocusItem,
					GUI::CTextInput * const TextInput);


bool GUI::DrawTextInput(int RenderPositionX, int& RenderPositionY,
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
						CTextInput *const TextInput)
{
	const auto SizeX 			= (int)(GroupSizeX - PADDING * 4);
	const auto SizeY 			= (int)(ITEM_HEIGHT * 2 * DPIScale);
	const auto Padding 			= (int)(PADDING * DPIScale);

	const auto EscapeState 		= Input::GetKeyState(Input::KeyCodes::KEY_ESCAPE);
	const auto MouseLeftState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);
	const auto Hovered 			= Input::IsMouseInBounds(RenderPositionX, RenderPositionY, SizeX, SizeY);
	
	int TextLength = LibUtil_StringNLength(Value, BufferSize);
	
	TextInputHandleInput(
		RenderPositionX, RenderPositionY,
		SizeX, SizeY,
		TextLength,
		GroupSizeX,
		Name, Value, BufferSize,
		Animation,
		CursorStart, 
		CursorFade,
		CursorPosition, 
		Color, 
		TimeSinceLastFrame, Alpha,
		DPIScale,
		AllowInput, FocusItem,
		TextInput
	);

	RenderTextInput(
		RenderPositionX, RenderPositionY,
		SizeX, SizeY,
		TextLength,
		GroupSizeX,
		Name, Value, BufferSize,
		Animation,
		CursorStart, 
		CursorFade,
		CursorPosition, 
		Color,
        TimeSinceLastFrame, Alpha, 
		FormAccent, DPIScale
	);

	RenderPositionY += SizeY + Padding;

	return FALSE;
}

void RenderTextInput(int RenderPositionX, int& RenderPositionY,
					const int SizeX, const int SizeY,
					const int TextLength,
					const int GroupSizeX,
					const wchar_t *Name, char *Value, const int BufferSize,
					const float Animation,
					float& CursorStart, 
					bool& CursorFade,
					int& CursorPosition, 
					Cheat::Datatypes::Color& Color,
                    const float TimeSinceLastFrame, const float Alpha, 
					const Cheat::Datatypes::Color FormAccent, const float DPIScale)
{
	Render::Renderer->DrawSetColor(GROUP_COLOR_MENU);
    Render::Renderer->DrawFilledRect(RenderPositionX, RenderPositionY, SizeX, SizeY);

	Render::Renderer->DrawSetColor(OUTLINE_LIGHT_COLOR_MENU);
    Render::Renderer->DrawOutlinedRect(RenderPositionX, RenderPositionY, SizeX, SizeY);

	if(Animation)
	{
		int CursorAddX, TMP;
		Render::Font::Hack->GetTextSize(CursorAddX, TMP, 
			Value, CursorPosition, 
			0.7f * DPIScale);

		int TW, TH;
		Render::Font::Hack->GetTextSize(TW, TH,
			Value, TextLength, 
			0.7f * DPIScale);


		if(CursorFade)
		{
			CursorStart -= TimeSinceLastFrame;

			if(CursorStart <= 0.f)
			{
				CursorStart = 0.f;
				CursorFade = FALSE;
			}
		}
		else
		{
			CursorStart += TimeSinceLastFrame;
			if(CursorStart >= 1.f)
			{
				CursorStart = 1.f;
				CursorFade = TRUE;
			}
		}

		Render::Renderer->DrawSetColor(SELECTED_ITEM_COLOR_MENU.ScaleAlpha(LibUtil_MinF(CursorStart, Animation)));
    	Render::Renderer->DrawFilledRect(RenderPositionX + SizeX / 2 - TW / 2 + CursorAddX, RenderPositionY, (int)(2 * DPIScale), SizeY);
	}
		
	Render::Font::Hack->DrawText(
        RenderPositionX + SizeX / 2, RenderPositionY + 2,
        Value, TextLength,
        Color.ScaleAlpha(Alpha).ToUInt(),
        Render::Font::FONT_SHADOW | Render::Font::FONT_CENTER,
        0.7f * DPIScale
    );
}

void TextInputHandleInput(int RenderPositionX, int& RenderPositionY,
					const int SizeX, const int SizeY,
					int& TextLength,
					const int GroupSizeX,
					const wchar_t *Name, char *Value, const int BufferSize,
					const float Animation,
					float& CursorStart, 
					bool& CursorFade,
					int& CursorPosition, 
					Cheat::Datatypes::Color& Color, 
					const float TimeSinceLastFrame, const float Alpha,
					const float DPIScale,
					const bool AllowInput, GUI::CFocusItem *const FocusItem,
					GUI::CTextInput * const TextInput)
{
	const auto EscapeState 		= Input::GetKeyState(Input::KeyCodes::KEY_ESCAPE);
	const auto MouseLeftState 	= Input::GetKeyState(Input::KeyCodes::MOUSE_LEFT);
	const auto Hovered 			= Input::IsMouseInBounds(RenderPositionX, RenderPositionY, SizeX, SizeY);

	if(AllowInput == TRUE)
	{
		if(MouseLeftState.Pressed && Hovered)
		{
			if(FocusItem)
			{
				FocusItem->Update(TextInput, 
						RenderPositionX, 
						RenderPositionY, 
						GroupSizeX,
						0, 0, 
						false);
			}
		}
	}

	if(Animation)
	{
		Color = Color.Lerp(SELECTED_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}
	else if(Hovered)
	{
		Color = Color.Lerp(HOVERED_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}
	else 
	{
		Color = Color.Lerp(DIM_ITEM_COLOR_MENU, ANIMATION_SPEED);
	}

	if(Animation)
	{
		if((!Hovered && MouseLeftState.Pressed) || EscapeState.Pressed && FocusItem)
		{
			FocusItem->Active = FALSE;
		}

		for(int i = Input::KeyCodes::KEY_A; i <= Input::KeyCodes::KEY_0; i++)
		{
			if(TextLength >= BufferSize)
			{
				break;
			}

			if(Input::GetKeyState((Input::KeyCodes)i).Pressed)
			{
				if(i <= Input::KeyCodes::KEY_Z)
				{
					const auto Shifting = Input::GetKeyState(Input::KeyCodes::KEY_LSHIFT).Down || Input::GetKeyState(Input::KeyCodes::KEY_RSHIFT).Down;
					
					char Character = 'a' + i;
					if((Input::GetKeyState(Input::KeyCodes::KEY_CAPSLOCK).Toggled && !Shifting)
						|| (!Input::GetKeyState(Input::KeyCodes::KEY_CAPSLOCK).Toggled && Shifting))
					{
						Character = LibUtil_StringToUpper(Character);
					}

					AddCharacter(Character, 
						Value, TextLength, 
						CursorPosition, CursorStart, CursorFade);
				}
				else if(i >= Input::KeyCodes::KEY_1
					&& i <= Input::KeyCodes::KEY_9)
				{
					const auto Shifting = Input::GetKeyState(Input::KeyCodes::KEY_LSHIFT).Down || Input::GetKeyState(Input::KeyCodes::KEY_RSHIFT).Down;
		
					char Symbol = '1' + (i - Input::KeyCodes::KEY_1);
					if(Shifting)
					{
						switch(i)
						{
							case Input::KeyCodes::KEY_1:
								Symbol = '!';
								break;
							case Input::KeyCodes::KEY_2:
								Symbol = '@';
								break;
							case Input::KeyCodes::KEY_3:
								Symbol = '#';
								break;
							case Input::KeyCodes::KEY_4:
								Symbol = '$';
								break;
							case Input::KeyCodes::KEY_5:
								Symbol = '%';
								break;
							case Input::KeyCodes::KEY_6:
								Symbol = '^';
								break;
							case Input::KeyCodes::KEY_7:
								Symbol = '&';
								break;
							case Input::KeyCodes::KEY_8:
								Symbol = '*';
								break;
							case Input::KeyCodes::KEY_9:
								Symbol = '(';
								break;
							case Input::KeyCodes::KEY_0:
								Symbol = ')';
								break;
						}
					}
					AddCharacter(Symbol, 
						Value, TextLength, 
						CursorPosition, CursorStart, CursorFade);
				}
				else if(i == Input::KeyCodes::KEY_0)
				{
					const auto Shifting = Input::GetKeyState(Input::KeyCodes::KEY_LSHIFT).Down || Input::GetKeyState(Input::KeyCodes::KEY_RSHIFT).Down;
		
					AddCharacter(Shifting ? ')' : '0', 
						Value, TextLength, 
						CursorPosition, CursorStart, CursorFade);
				}
			}
		}

		if(Input::GetKeyState(Input::KeyCodes::KEY_SPACE).Pressed
			&& TextLength < BufferSize)
		{
			AddCharacter(' ', 
						Value, TextLength, 
						CursorPosition, CursorStart, CursorFade);
		}

		if(Input::GetKeyState(Input::KeyCodes::KEY_PERIOD).Pressed
			&& TextLength < BufferSize)
		{
			AddCharacter('.', 
						Value, TextLength, 
						CursorPosition, CursorStart, CursorFade);
		}

		if(Input::GetKeyState(Input::KeyCodes::KEY_COMMA).Pressed
			&& CursorPosition < BufferSize)
		{
			AddCharacter(',', 
						Value, TextLength, 
						CursorPosition, CursorStart, CursorFade);
		}

		if(Input::GetKeyState(Input::KeyCodes::KEY_BACKSPACE).Pressed
			&& CursorPosition > 0)
		{
			for(int j = CursorPosition; j < TextLength; j++)
			{
				Value[j - 1] = Value[j];
			}
			TextLength--;
			Value[TextLength] = '\0';
			CursorPosition--;
			CursorStart = 1.f;
			CursorFade 	= TRUE;
		}

		if(Input::GetKeyState(Input::KeyCodes::KEY_ARROW_LEFT).Pressed
			&& CursorPosition > 0)
		{
			CursorPosition--;
			CursorStart = 1.f;
			CursorFade 	= TRUE;
		}

		if(Input::GetKeyState(Input::KeyCodes::KEY_ARROW_RIGHT).Pressed
			&& CursorPosition < TextLength)
		{
			CursorPosition++;
			CursorStart = 1.f;
			CursorFade 	= TRUE;
		}

		if(Input::GetKeyState(Input::KeyCodes::KEY_DELETE).Pressed
			&& CursorPosition < TextLength)
		{
			for(int j = CursorPosition + 1; j < TextLength; j++)
			{
				Value[j - 1] = Value[j];
			}
			TextLength--;
			Value[TextLength] = '\0';

			CursorStart = 1.f;
			CursorFade 	= TRUE;
		}
	}
}

void AddCharacter(char Character, char *Value, int& TextLength, int& CursorPosition, float& CursorStart, bool& CursorFade)
{
	if(CursorPosition < TextLength)
	{
		for(int j = TextLength; j > CursorPosition; j--)
		{
			Value[j] = Value[j - 1];
		}
	}

	Value[CursorPosition] = Character;
	TextLength++;
	CursorPosition++;
	CursorStart = 1.f;
	CursorFade 	= TRUE;
}