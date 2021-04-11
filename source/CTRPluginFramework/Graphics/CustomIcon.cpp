#include "CTRPluginFramework/Graphics/CustomIcon.hpp"

namespace CTRPluginFramework
{
    CustomIcon::CustomIcon(Pixel* pixArray, int sizeX, int sizeY, bool isEnabled)
    {
        this->pixArray = pixArray;
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        this->isEnabled = isEnabled;
    }

    CustomIcon::CustomIcon()
    {
        this->pixArray = nullptr;
        this->sizeX = 0;
        this->sizeY = 0;
        this->isEnabled = true;
    }
}
