#include "CTRPluginFramework/Graphics/MessageBox.hpp"
#include "CTRPluginFrameworkImpl/Graphics/MessageBoxImpl.hpp"

namespace   CTRPluginFramework
{
    MessageBox::~MessageBox(void)
    {
        //delete _messageBox;
    }

    MessageBox::MessageBox(const std::string &title, const std::string &message, DialogType dialogType, ClearScreen clear);
    {
        this->_messageBox = std::make_unique<MessageBoxImpl>(title, message, dialogType, clear);
    }

    MessageBox::MessageBox(const std::string &message, DialogType dialogType, ClearScreen clear);
    {
        this->_messageBox = std::make_unique<MessageBoxImpl>("", message, dialogType, clear);
    }

    MessageBox& MessageBox::SetClear(ClearScreen screen)
    {
        this->_messageBox->screen = screen;
        return *this;
    }

    bool MessageBox::operator()(void) const
    {
        return (this->_messageBox)();
    }
}
