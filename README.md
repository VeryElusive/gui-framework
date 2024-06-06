# An object oriented GUI framework, with heavy emphasis on modularity.
## Project linecount total: 4,116 lines.

Written using libutil, my friend's runtime library (thx @welikethestocks !!)

## Features:
 - Object oriented
 - Keybind selector for checkboxes, sliders and dropdowns
 - Scrolling on groups and listboxes
 - Menu movable by dragging
 - Menu resizing by dragging
 - DPI scaling

## Menu items:
 - Checkboxes
 - Sliders
 - Dropdowns
 - Multi selectors
 - Color pickers
 - Buttons
 - Listboxs
 - Text inputs


# USAGE:

### Creating a form:
```cpp

static Cheat::GUI::CForm Form(
            DEFAULT_POSITION_X, DEFAULT_POSITION_Y, 
            DEFAULT_SIZE_X, DEFAULT_SIZE_Y, 
            L"Form name", ACCENT_COLOR, 
            DEFAULT_OPENED,
            RESIZABLE, 
            MINIMUM_SIZE_X, MINIMUM_SIZE_Y,
            MAXIMUM_SIZE_X, MAXIMUM_SIZE_Y
);
```


### Creating a tab:
```cpp
  const GUI::CTab *Tab = Form.CreateNewTab(L"Tab name");
```

### Creating a subtab:
```cpp
  const GUI::CSubTab *Subtab = Tab->CreateNewSubTab(L"SubTab name");
```

### Creating a group:
```cpp
  const CGroup *Group = Subtab->CreateGroup(L"Group name", HORIZONTAL_SIZE_PERCENT, VERTICAL_SIZE_PERCENT);
```

### Creating a checkbox:
```cpp
  static bool CheckboxValue;
  static GUI::CKeyBind CheckboxKeybind;
  Group->AddCheckbox(L"Checkbox name", &CheckboxValue, &CheckboxKeybind);

  // no keybind selector.
  Group->AddCheckbox(L"Checkbox name", &CheckboxValue, NULL);
```

### Creating a slider:
```cpp
  static float SliderValue;
  static GUI::CKeyBind SliderKeybind;

  Group->AddSlider(L"Slider name", &SliderValue, &SliderKeybind, SLIDER_MIN_VALUE, SLIDER_MAX_VALUE);

  // no keybind selector.
  Group->AddSlider(L"Slider name", &SliderValue, NULL, SLIDER_MIN_VALUE, SLIDER_MAX_VALUE);
```

### Creating a dropdown:
```cpp
  static GUI::CKeyBind DropdownKeybind;
  wchar_t *DropdownItems[] = {(L"One"), (L"two"), (L"Three")};
  Group->AddDropdown(L"Dropdown name", &Dropdown, &DropdownKeybind, DropdownItems, ITEMS_COUNT);

  Group->AddDropdown(L"Dropdown name", &Dropdown, NULL, DropdownItems, ITEMS_COUNT);
```

### Creating a multi select:
```cpp
  static bool wan, too, tree;
  GUI::CMultiSelectItem MultiSelectItems[] = {{L"One 1", &wan}, {L"two 2", &too}, {L"Three 3", &tree}};

  Group->AddMultiSelect(L"Multi select name", MultiSelectItems, ITEMS_COUNT);
```

### Creating a colorpicker (must be used BELOW A CHECKBOX!):
```cpp
  // example checkbox
  Group->AddCheckbox(L"Checkbox name", &CheckboxValue, &CheckboxKeybind);

  static Datatypes::Color ColorpickerValue(255, 0, 0);
  group->AddColorPicker(&ColorpickerValue);
```

### Creating a button:
```cpp
  // must be a cdecl function
  void __cdecl TestCallback()
  {
    Tier0::Msg("Callback Hit!\n");
  }

  Group->AddButton(L"Button name", TestCallback); 
```

### Creating a list box:
```cpp
  static int ListboxValue;
  wchar_t *ListboxItems[] = {(L"One"), (L"two"), (L"Three")};

  Group->AddListbox(L"Listbox name", &ListboxVal, ListboxItems, ITEMS_COUNT, HORIZONTAL_SIZE_PERCENT);
```

### Creating a text input:
```cpp
  static char Buffer[32] = {'\0'};
  Group->AddTextInput(L"Text input name", Buffer, BUFFER_SIZE);
```
