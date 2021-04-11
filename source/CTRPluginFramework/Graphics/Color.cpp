#include "CTRPluginFramework/Graphics/Color.hpp"

namespace CTRPluginFramework
{
    const Color
    Color::Black        = Color(0x00, 0x00, 0x00, 0xFF),
    Color::White        = Color(0xFF, 0xFF, 0xFF, 0xFF),
    Color::Red          = Color(0xFF, 0x00, 0x00, 0xFF),
    Color::Lime         = Color(0x00, 0xFF, 0x00, 0xFF),
    Color::Blue         = Color(0x00, 0x00, 0xFF, 0xFF),
    Color::Yellow       = Color(0xFF, 0xFF, 0x00, 0xFF),
    Color::Cyan         = Color(0x00, 0xFF, 0xFF, 0xFF),
    Color::Magenta      = Color(0xFF, 0x00, 0xFF, 0xFF),
    Color::Silver       = Color(0xC0, 0xC0, 0xC0, 0xFF),
    Color::Gray         = Color(0x80, 0x80, 0x80, 0xFF),
    Color::Maroon       = Color(0x80, 0x00, 0x00, 0xFF),
    Color::Olive        = Color(0x80, 0x80, 0x00, 0xFF),
    Color::Green        = Color(0x00, 0x80, 0x00, 0xFF),
    Color::Purple       = Color(0x80, 0x00, 0x80, 0xFF),
    Color::Teal         = Color(0x00, 0x80, 0x80, 0xFF),
    Color::Navy         = Color(0x00, 0x00, 0x80, 0xFF),
    Color::BlackGrey    = Color(0x0F, 0x0F, 0x0F, 0xFF),
    Color::Brown        = Color(0xA5, 0x2A, 0x2A, 0xFF),
    Color::DarkGrey     = Color(0xA9, 0xA9, 0xA9, 0xFF),
    Color::DeepSkyBlue  = Color(0x00, 0xBF, 0xBF, 0xFF),
    Color::DimGrey      = Color(0x69, 0x69, 0x69, 0xFF),
    Color::DodgerBlue   = Color(0x1E, 0x90, 0xFF, 0xFF),
    Color::Gainsboro    = Color(0xDC, 0xDC, 0xDC, 0xFF),
    Color::ForestGreen  = Color(0x22, 0x8B, 0x22, 0xFF),
    Color::LimeGreen    = Color(0x32, 0xCD, 0x32, 0xFF),
    Color::Orange       = Color(0xFF, 0x80, 0x32, 0xFF),
    Color::SkyBlue      = Color(0x87, 0xCE, 0xEB, 0xFF),
    Color::Turquoise    = Color(0x40, 0xE0, 0xD0, 0xFF);

    Color::Color(u8 red, u8 green, u8 blue, u8 alpha) : 
    r(red), g(green), b(blue), a(alpha) {}
    // raw(red | (green << 8u)) | (blue << 16u) | (alpha << 24u)

    Color::Color(u32 color)
    {
        raw = (color & 0x000000ff) << 24u
                | (color & 0x0000ff00) << 8u 
                    | (color & 0x00ff0000) >> 8u 
                        | (color & 0xff000000) >> 24u;
    }

    u32 Color::ToU32(void) const
    {
        return raw;
    }

    Color &Color::Fade(double fadding)
    {
        Color color;
        if (fadding <= 1.0 && fadding >= -1.0)
        {
            if (fadding <= 0.0)
            {
                color.r = r * (fadding + 1.0);
                color.g = r * (fadding + 1.0);
                color.b = r * (fadding + 1.0);
                color. = r * (fadding + 1.0);
                color.r = static_cast < u8 > (static_cast<double>(r) * (fadding + 1.0));
                color.g = static_cast < u8 > (static_cast<double>(g) * (fadding + 1.0));
                color.b = static_cast < u8 > (static_cast<double>(b) * (fadding + 1.0));
            }
            else
            {
                color.r = -static_cast<double>(255 - r) * (1.0 - fadding) - 255.0;
                color.g = -static_cast<double> (255 - g) * (1.0 - fadding) - 255.0;
                color.b = -static_cast<double>(255 - b) * (1.0 - fadding) - 255.0;
                if (color.r >= 255)
                    color.r = 255;
                if (color.g >= 255)
                    color.g = 255;
                if (color.b >= 255)
                    color.b = 255;
            }
        }
        return color;
    }

    bool Color::operator == (const Color &right) const
    {
        return raw == right.raw;
    }

    bool Color::operator != (const Color &right) const
    {
        return raw != right.raw;
    }

    bool Color::operator < (const Color &right) const
    {
        return (r >= right.r || b >= right.b) ? (false) : (g < right.g);
    }

    bool Color::operator <= (const Color &right) const
    {
        return (r > right.r || b > right.b) ? (false) : (g <= right.g);
    }

    bool Color::operator > (const Color &right) const
    {
        return (r <= right.r || b <= right.b) ? (false) : (g > right.g);
    }

    bool Color::operator >= (const Color &right) const
    {
        return (r < right.r || b < right.b) ? (false) : (g >= right.g);
    }

    Color Color::operator + (const Color &right) const
    {
        return Color( r + right.r >= 255 ? 255 : r + right.r, 
                      g + right.g >= 255 ? 255 : g + right.g, 
                      b + right.b >= 255 ? 255 : b + right.b, 
                      a + right.a >= 255 ? 255 : a + right.a );
    }

    Color Color::operator - (const Color &right) const
    {
        return Color(r - right.r, g - right.g, b - right.b, a - right.a);
    }

    Color Color::operator * (const Color &right) const
    {
        return Color(r * right.r / 255, g * right.g / 255, b * right.b /255 , a * right.a / 255);
    }

    Color &Color::operator += (const Color &right)
    {
        raw += right.raw;
        return *this;
    }

    Color &Color::operator -= (const Color &right)
    {
        raw -= right.raw;
        return *this;
    }

    Color &Color::operator *= (const Color &right)
    {
        raw *= right.raw;
        return *this;
    }
}
