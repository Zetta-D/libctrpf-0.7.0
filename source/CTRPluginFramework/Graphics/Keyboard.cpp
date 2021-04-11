#include "CTRPluginFrameworkImpl/Menu/KeyboardImpl.hpp"
#include "CTRPluginFramework/Menu/Keyboard.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"

namespace CTRPluginFramework
{
    enum class Layout
    {
        Qwerty,
        Decimal,
        Hexadecimal
    };

    u8    ConvertToU8(std::string str, bool isHex)
    {
        if(!str.empty() || str.find('.') == std::string::npos)
            return  (u8)(std::strtoul(str.c_str(), nullptr, isHex ? 16 : 10));
        return 0;
    }

    u16    ConvertToU16(std::string str, bool isHex)
    {
        if(!str.empty() || str.find('.') == std::string::npos)
            return  (u16)(std::strtoul(str.c_str(), nullptr, isHex ? 16 : 10));
        return 0;
    }

    u32    ConvertToU32(std::string str, bool isHex)
    {
        if(!str.empty() || str.find('.') == std::string::npos)
            return  (u32)(std::strtoul(str.c_str(), nullptr, isHex ? 16 : 10));
        return 0;
    }

    u64     ConvertToU64(std::string str, bool isHex)
    {
        if(!str.empty() || str.find('.') == std::string::npos)
            return  (u64)(std::strtoul(str.c_str(), nullptr, isHex ? 16 : 10));
        return 0;
    }

    float   ConvertToU64(std::string str, bool isHex)
    {
        if(!str.empty() || str.find('.') == std::string::npos)
            return  (float)(std::strtof(str.c_str(), nullptr, isHex ? 16 : 10));
        return 0;
    }

    float   ConvertToUDouble(std::string str, bool isHex)
    {
        if(!str.empty() || str.find('.') != std::string::npos)
            return  (float)(std::strtof(str.c_str(), nullptr, isHex ? 16 : 10));
        return 0.0;
    }

    double  ConvertToUDouble(std::string str, bool isHex)
    {
        if(!str.empty() || str.find('.') != std::string::npos)
            return  (double)(std::strtod(str.c_str(), nullptr, isHex ? 16 : 10));
        return 0.0;
    }

    Keyboard::Keyboard(const std::string &text)
    {
        this->_keyboard = std::make_unique<KeyboardImpl>(text);
        _hexadecimal = true;
    }

    Keyboard::Keyboard(const std::string &text, const std::vector<std::string> &options)
    {
        this->_keyboard = std::make_unique<KeyboardImpl>(text);
        this->_keyboard->Populate(options);
        _hexadecimal = false;
        _isPopulated = !options.empty() ? true : false;
    }

    Keyboard::Keyboard(const std::vector<std::string> &options)
    {
        this->_keyboard = std::make_unique<KeyboardImpl>("");
        this->_keyboard->Populate(options);
        _hexadecimal = false;
        _isPopulated = !options.empty() ? true : false;
    }

    Keyboard::~Keyboard(void)
    {
        this->_keyboard->~KeyboardImpl();
    }

    void Keyboard::IsHexadecimal(bool isHex)
    {
        _hexadecimal = isHex;
    }

    void Keyboard::SetMaxLength(u32 maxValue) const
    {
        this->_keyboard->SetMaxInput(maxValue);
    }

    void Keyboard::SetCompareCallback(CompareCallback callback) const
    {
        this->_keyboard->SetCompareCallback(callback);
    }

    void Keyboard::Populate(const std::vector<std::string> &input)
    {
        this->_keyboard->Populate(input);
        _isPopulated = true;
    }

    int Keyboard::Open(void) const
    {
        return _isPopulated ? this->_keyboard->Run() : -1;
    }

    int Keyboard::Open(u64 &output) const
    {
        this->_keyboard->SetLayout(_hexadecimal ? Layout::Hexadecimal : Layout::Decimal)
        if (_hexadecimal)
            this->_keyboard->SetMaxInput(16);
        this->_keyboard->CanChangeLayout(true);
        this->_keyboard->SetConvertCallback(ConvertToU64);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToU64(this->_keyboard->GetInput(), this->_keyboard->IsHexadecimal());
        return res;
    }

    int Keyboard::Open(u64 &output, u64 start) const
    {
        bool isEmpty = this->_keyboard->GetInput().empty();
        this->_keyboard->SetLayout(_hexadecimal ? Layout::Hexadecimal : Layout::Decimal)
        if (_hexadecimal)
            this->_keyboard->SetMaxInput(16);
        this->_keyboard->CanChangeLayout(true);
        this->_keyboard->SetConvertCallback(ConvertToU64);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToU64(isEmpty ? Utils::Format(_hexadecimal ? "%X" : "%d", start) : this->_keyboard->GetInput()
                , this->_keyboard->IsHexadecimal());
        return res;
    }

    int Keyboard::Open(float &output) const
    {
        this->_keyboard->SetLayout(Layout::Decimal)
        this->_keyboard->SetConvertCallback(ConvertToFloat);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToFloat(this->_keyboard->GetInput(), this->_keyboard->IsHexadecimal());
        return res;
    }

    int Keyboard::Open(float &output, float start) const
    {
        bool isEmpty = this->_keyboard->GetInput().empty();
        this->_keyboard->SetLayout(Layout::Decimal)
        this->_keyboard->SetConvertCallback(ConvertToFloat);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToFloat(isEmpty ? Utils::ToString(start, 4) : this->_keyboard->GetInput(),
            this->_keyboard->IsHexadecimal());
        return res;
    }

    int Keyboard::Open(double &output) const
    {
        this->_keyboard->SetLayout(Layout::Decimal)
        this->_keyboard->SetConvertCallback(ConvertToDouble);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToDouble(this->_keyboard->GetInput(), this->_keyboard->IsHexadecimal());
        return res;
    }

    int Keyboard::Open(double &output, double start) const
    {
        bool isEmpty = this->_keyboard->GetInput().empty();
        this->_keyboard->SetLayout(Layout::Decimal)
        this->_keyboard->SetConvertCallback(ConvertToDouble);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToDouble(isEmpty ? Utils::Format("%.4lf", start) : this->_keyboard->GetInput(), 
                this->_keyboard->IsHexadecimal());
        return res;
    }

    int Keyboard::Open(std::string &output) const
    {
        this->_keyboard->SetLayout(Layout::Qwerty)
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = this->_keyboard->GetInput();
        return res;
    }

    int Keyboard::Open(std::string &output, const std::string &start) const
    {
        bool isEmpty = this->_keyboard->GetInput().empty();
        this->_keyboard->SetLayout(Layout::Qwerty)
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = isEmpty ? start : this->_keyboard->GetInput();
        return res;
    }

    std::string &Keyboard::GetInput(void) const
    {
        return this->_keyboard->GetInput();
    }

    std::string &Keyboard::GetMessage(void) const
    {
        return this->_keyboard->GetMessage();
    }

    void Keyboard::SetError(std::string error) const
    {
        this->_keyboard->GetMessage(error);
    }

    void Keyboard::CanAbort(bool canAbort) const
    {
        this->_keyboard->GetMessage(canAbort);
    }

    void Keyboard::Close(void) const
    {
        this->_keyboard->Close();
    }

    void Keyboard::OnInputChange(OnInputChangeCallback callback) const
    {
        this->_keyboard->OnInputChange(callback);
    }

    int Keyboard::Open(u32 &output) const
    {
        this->_keyboard->SetLayout(_hexadecimal ? Layout::Hexadecimal : Layout::Decimal)
        if (_hexadecimal)
            this->_keyboard->SetMaxInput(8);
        this->_keyboard->CanChangeLayout(true);
        this->_keyboard->SetConvertCallback(ConvertToU32);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToU32(this->_keyboard->GetInput(), this->_keyboard->IsHexadecimal());
        return res;
    }

    int Keyboard::Open(u32 &output, u32 start) const
    {
        bool isEmpty = this->_keyboard->GetInput().empty();
        this->_keyboard->SetLayout(_hexadecimal ? Layout::Hexadecimal : Layout::Decimal)
        if (_hexadecimal)
            this->_keyboard->SetMaxInput(8);
        this->_keyboard->CanChangeLayout(true);
        this->_keyboard->SetConvertCallback(ConvertToU32);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToU32(isEmpty ? Utils::Format(_hexadecimal ? "%X" : "%d", start) : this->_keyboard->GetInput()
                , this->_keyboard->IsHexadecimal());
        return res;
    }

    int Keyboard::Open(u16 &output) const
    {
        this->_keyboard->SetLayout(_hexadecimal ? Layout::Hexadecimal : Layout::Decimal)
        if (_hexadecimal)
            this->_keyboard->SetMaxInput(4);
        this->_keyboard->CanChangeLayout(true);
        this->_keyboard->SetConvertCallback(ConvertToU16);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToU16(this->_keyboard->GetInput(), this->_keyboard->IsHexadecimal());
        return res;
    }

    int Keyboard::Open(u16 &output, u16 start) const
    {
        bool isEmpty = this->_keyboard->GetInput().empty();
        this->_keyboard->SetLayout(_hexadecimal ? Layout::Hexadecimal : Layout::Decimal)
        if (_hexadecimal)
            this->_keyboard->SetMaxInput(4);
        this->_keyboard->CanChangeLayout(true);
        this->_keyboard->SetConvertCallback(ConvertToU16);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToU16(isEmpty ? Utils::Format(_hexadecimal ? "%X" : "%d", start) : this->_keyboard->GetInput()
                , this->_keyboard->IsHexadecimal());
        return res;
    }

    int Keyboard::Open(u8 &output) const
    {
        this->_keyboard->SetLayout(_hexadecimal ? Layout::Hexadecimal : Layout::Decimal)
        if (_hexadecimal)
            this->_keyboard->SetMaxInput(2);
        this->_keyboard->CanChangeLayout(true);
        this->_keyboard->SetConvertCallback(ConvertToU8);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToU8(this->_keyboard->GetInput(), this->_keyboard->IsHexadecimal());
        return res;
    }

    int Keyboard::Open(u8 &output, u8 start) const
    {
        bool isEmpty = this->_keyboard->GetInput().empty();
        this->_keyboard->SetLayout(_hexadecimal ? Layout::Hexadecimal : Layout::Decimal)
        if (_hexadecimal)
            this->_keyboard->SetMaxInput(2);
        this->_keyboard->CanChangeLayout(true);
        this->_keyboard->SetConvertCallback(ConvertToU8);
        int res = this->_keyboard->Run();
        if ( res != -1 )
            output = ConvertToU8(isEmpty ? Utils::Format(_hexadecimal ? "%X" : "%d", start) : this->_keyboard->GetInput()
                , this->_keyboard->IsHexadecimal());
        return res;
    }
}
