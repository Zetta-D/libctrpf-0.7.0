#include "CTRPluginFramework/System/FwkSettings.hpp"

namespace   CTRPluginFramework
{
    FwkSettings& FwkSettings::Get(void)
    {
        return Preferences::Settings;
    }

    void    FwkSettings::SetThemeDefault(void)
    {
        FwkSettings &settings = FwkSettings::Get();

        settings.MainTextColor                          = Color::White;
        settings.WindowTitleColor                       = Color::White;
        settings.BackgroundBorderColor                  = Color::White;
        settings.MenuSelectedItemColor                  = Color::White;
        settings.MenuUnselectedItemColor                = Color::Silver;
        settings.BackgroundMainColor                    = Color::Black;
        settings.BackgroundSecondaryColor               = Color::BlackGrey;
        settings.CursorFadeValue                        = 0.2f;
        settings.Keyboard.Background                    = Color::Black;
        settings.Keyboard.KeyBackground                 = Color::Black;
        settings.Keyboard.KeyBackgroundPressed          = Color::Silver;
        settings.Keyboard.KeyText                       = Color::White;
        settings.Keyboard.KeyTextPressed                = Color::White;
        settings.Keyboard.KeyTextDisabled               = Color::DimGrey;
        settings.Keyboard.Cursor                        = Color::White;
        settings.Keyboard.Input                         = Color::White;
        settings.CustomKeyboard.BackgroundMain          = Color::Black;
        settings.CustomKeyboard.BackgroundSecondary     = Color::BlackGrey;
        settings.CustomKeyboard.BackgroundBorder        = Color::White;
        settings.CustomKeyboard.KeyBackground           = Color(55, 55, 55, 255);
        settings.CustomKeyboard.KeyBackgroundPressed    = Color::Gainsboro;
        settings.CustomKeyboard.KeyText                 = Color::White;
        settings.CustomKeyboard.KeyTextPressed          = Color::Black;
        settings.CustomKeyboard.ScrollBarBackground     = Color::Silver;
        settings.CustomKeyboard.ScrollBarThumb          = Color::DimGrey;
    }

    Result    FwkSettings::SetTopScreenBackground(void *bmpData)
    {
        BMPImage *img = new BMPImage(bmpData);
        if(!img->IsLoaded())
        {
            img->~BMPImage();
            delete img;
            return -1;
        }
        Preferences::topBackgroundImage = img;
        Window::UpdateBackgrounds();
        return 0;
    }

    Result    FwkSettings::SetTopScreenBackground(void *bmpData)
    {
        BMPImage *img = new BMPImage(bmpData);
        if(!img->IsLoaded())
        {
            img->~BMPImage();
            delete img;
            return -1;
        }
        Preferences::bottomBackgroundImage = img;
        Window::UpdateBackgrounds();
        return 0;
    }
}
