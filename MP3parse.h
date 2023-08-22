//
// Created by Елизавета Кравченкова on 08.02.2023.
//
#pragma once

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <math.h>
#include <set>
#include <locale>
#include <codecvt>

std::u16string GetWC2(const std::string& str1);

std::u16string GetWC3(const std::string& str1);

// считывает 32 bit synchsafe integer
uint32_t ReadSynchInt(std::ifstream& file);

// получение бинарного представления числа
void GetBin(uint8_t x, bool bin_ch[]);

uint32_t GetUintNumber(bool bin_ch[]);

std::string utf16_to_utf8(const std::string& bad_utf16, uint8_t code);

std::string DecoderCode(const std::string& str1, uint8_t code);

std::string DecondUnsynchronisation(const std::string& www);

uint32_t GetNum256(const std::string& str);

void PrintTimeStampFormat(uint8_t byte);

void PrintTypeOfEvent(uint8_t byte);

void PrintReceviedAs(uint8_t byte);

void PrintInterpolationMethod(uint8_t byte);

void PrintContentType(uint8_t byte);

void PrintTypeOfChannel(uint8_t byte);

void ParseEQU2(const std::string& FreqVolum, std::vector<int16_t>& volume_adjustment, std::vector<uint16_t>& frequency);

bool ParseRVA2(const std::string& other, std::vector<uint8_t>& channel_type, std::vector<int16_t>& volume_adjustment_,
               std::vector<uint8_t>& bits_representing_, std::vector<uint32_t>& peak_volume_);

void PrintEventsETCO(const std::string& value);

void PrintEventsSYLT(const std::string& value);

// применяется для описания T...(искл TXXX) и URL, SEEK фреймов
class Frame {
public:
    Frame(const std::string& name, const std::string& value, bool valid = true)
            : name_(name), value_(value), valid_(valid) {
    }

    Frame(const std::string& name, const std::string& value, uint8_t encoding, bool valid = true)
            : name_(name), value_(DecoderCode(value, encoding)), valid_(valid) {

        for (uint32_t i = 0; i < value.size(); i++) {
            if (value_[i] == '\0') {
                value_[i] = ' ';
            }
        }
    }

    const std::string& GetName() {
        return name_;
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " " << value_ << std::endl;
    }

    void PrintData() {
        if (!valid_) {
            std::cout << " К сожалению, это все, что я смог понять: " << '[' << name_ << ']' << " " << value_
                      << std::endl;
        } else {
            PrintValidData();
        }
    }

    virtual ~Frame() = default;

protected:
    std::string value_;
    std::string name_;
    bool valid_ = true;
};

// применяется для описания TXXX and WXXX фреймов
class XXframe : public Frame {
public:
    XXframe(const std::string& name, const std::string& value, uint8_t encoding, const std::string& deskript,
            bool valid = true)
            : Frame(name, value, valid), deskript_encod_text(DecoderCode(deskript, encoding)) {}

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " " << deskript_encod_text << ": " << value_ << std::endl;
    }

protected:
    std::string deskript_encod_text;
};

// применяется для описания COMM and USLT фреймов
class LangXXFrame : public XXframe {
public:
    LangXXFrame(const std::string& name, const std::string& value, uint8_t encoding, const std::string& language,
                const std::string& deskript,
                bool valid = true)
            : XXframe(name, DecoderCode(value, encoding), encoding, deskript, valid), language_(language) {
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " Language:" << language_ << ", " << deskript_encod_text << ": " << value_
                  << std::endl;
    }

protected:
    std::string language_;
};

// применяется для описания USER фреймов
class LangTFrame : public Frame {
public:
    LangTFrame(const std::string& name, const std::string& value, uint8_t encoding, const std::string& language,
               bool valid = true)
            : Frame(name, value, encoding, valid), language_(language) {
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " Language: " << language_ << ',' << value_ << std::endl;
    }

protected:
    std::string language_;
};

// применяется для описания SYLT фреймов
class SYLTframe : public LangTFrame {
public:
    SYLTframe(const std::string& name, uint8_t encoding, const std::string& language, uint8_t time_stamp_format,
              uint8_t content_type, const std::string& value, const std::string& Lyric, bool valid = true)
            : LangTFrame(name, value, encoding, language, valid), time_stamp_format_(time_stamp_format),
              content_type_(content_type), liric_text(Lyric) {}

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " Language: " << language_;
        PrintTimeStampFormat(time_stamp_format_);
        PrintContentType(content_type_);
        std::cout << " Content descriptor: " << value_ << std::endl;
        PrintEventsSYLT(liric_text);
    }

private:
    uint8_t time_stamp_format_;
    uint8_t content_type_;
    std::string liric_text;
};

// применяется для описания OWNE фреймов
class OWNEframe : public XXframe {
public:
    OWNEframe(const std::string& name, uint8_t encoding, const std::string& value, const std::string& data,
              const std::string& deskript,
              bool valid = true)
            : XXframe(name, value, encoding, deskript, valid), date_(data) {}

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " " << deskript_encod_text << ':' << value_ << std::endl;
    }

protected:
    std::string date_;
};

// применяется для описания COMR фреймов
class COMRframe : public OWNEframe {
public:
    COMRframe(const std::string& name, uint8_t encoding, const std::string& price, const std::string& date,
              const std::string& url,
              uint8_t recevied_us, const std::string& value, const std::string& deskript,
              const std::string& picture_descr,
              const std::string& logo,
              bool be_logo = false, bool valid = true)
            : OWNEframe(name, encoding, value, date, deskript, valid), url_(url), price_string(price),
              recevied_us_(recevied_us), picture_descr_(picture_descr), logo_(logo), be_logo_(be_logo) {}

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " " << price_string << ':' << date_ << ' ' << url_;
        PrintReceviedAs(recevied_us_);
        std::cout << value_ << ' ' << deskript_encod_text;
        if (be_logo_) {
            std::ofstream fout;
            fout.open("COMR.mp3parse", std::ios::binary);
            if (!fout.is_open()) {
                std::cout << ' ' << picture_descr_ << " Файл картинки не удалось создать " << std::endl;
            } else {
                fout.write(logo_.c_str(), logo_.size());
            }
            std::cout << ' ' << picture_descr_ << " Картинка хранится в файле COMR.mp3parse" << std::endl;
        } else {
            std::cout << std::endl;
        }
    }

protected:
    std::string price_string;
    std::string url_;
    uint8_t recevied_us_;
    std::string picture_descr_;
    std::string logo_;
    bool be_logo_;
};

// применяется для описаниz PCNT фреймов
class PCNTframe : public Frame {
public:
    PCNTframe(const std::string& name, const std::string& value, bool valid = true)
            : Frame(name, value, valid) {
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " Play counter: " << GetNum256(value_);
    }
};

// применяется для описаниz POPM фреймов
class POPMframe : public PCNTframe {
public:
    POPMframe(const std::string& name, const std::string& value, const std::string& email, uint8_t rating,
              bool valid = true)
            : PCNTframe(name, value, valid), email_(email), rating_(rating) {
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " Email to user: " << email_ << " rating: " << static_cast<int>(rating_);
        for (uint32_t i = 0; i < value_.size(); i++) {
            if (i == 0) {
                std::cout << " Counter:";  // так как это поле может быть пустое
            }
            std::cout << ' ' << value_[i];
        }
        std::cout << std::endl;
    }

protected:
    std::string email_;
    uint8_t rating_;
};

// применяется для описаниz RVA2 фреймов
class RVA2frame : public Frame {
public:
    RVA2frame(const std::string& name, const std::string& identification, const std::string& other, bool valid = true)
            : Frame(name, identification,
                    ParseRVA2(other, channel_type, volume_adjustment_, bits_representing_, peak_volume_)) {}

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " Identification: " << value_;
        for (uint32_t i = 0; i < channel_type.size(); i++) {
            PrintTypeOfChannel(channel_type[i]);
            std::cout << " Volume adjustment: " << volume_adjustment_[i] << " Bits representing peak"
                      << static_cast<uint8_t>(bits_representing_[i]);
            std::cout << " Peak volume: " << peak_volume_[i] << std::endl;
        }
    }

private:
    std::vector<uint8_t> channel_type;
    std::vector<int16_t> volume_adjustment_;
    std::vector<uint8_t> bits_representing_;
    std::vector<uint32_t> peak_volume_;
};

// применяется для описаниz POSS, SYTC фреймов
class Byteframe : public PCNTframe {
public:
    Byteframe(const std::string& name, const std::string& value, uint8_t byte, const std::string& forPrint,
              bool valid = true)
            : PCNTframe(name, value, valid), byte_(byte), ForPrint_(forPrint) {
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']';
        PrintTimeStampFormat(byte_);
        std::cout << ForPrint_ << value_;
    }

protected:
    uint8_t byte_;
    std::string ForPrint_;
};

// применяется для описания SIGN фреймов
class SIGNframe : public PCNTframe {
public:
    SIGNframe(const std::string& name, const std::string& value, uint8_t byte, bool valid = true)
            : PCNTframe(name, value, valid), byte_(byte) {
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']';
        std::cout << " Group symbol : " << byte_ << " Signature: " << value_ << std::endl;
    }

protected:
    uint8_t byte_;
};

// применяется для описаниz ENCR, GRID фреймов
class ByteIDframe : public Byteframe {
public:
    ByteIDframe(const std::string& name, const std::string& value, uint8_t byte, const std::string& id,
                const std::string& forPrint1, const std::string& forPrint2,
                bool valid = true)
            : Byteframe(name, value, byte, forPrint1, valid), id_(id), forPrint2_(forPrint2) {
    }

    virtual void PrintValidData() {
        std::cout << forPrint_ << byte_ << forPrint2_ << value_ << std::endl;
    }

private:
    std::string id_;
    std::string forPrint_;
    std::string forPrint2_;
};

// применяется для описаниz UFID и PRIV фреймов
class TwoTextframe : public Frame {
public:
    TwoTextframe(const std::string& name, const std::string& value, const std::string& id, const std::string& forPrint,
                 bool valid = true)
            : Frame(name, value, valid), id_(id), forPrint_(forPrint) {
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " Owner identifier: " << id_;
        std::cout << forPrint_ << value_ << std::endl;
    }

protected:
    std::string id_;
    std::string forPrint_;
};

// применяется для описаниz LINK фреймов
class LinkFrame : public Frame {
public:
    LinkFrame(const std::string& name, const std::string& url, const std::string& id, uint32_t intID, bool valid = true)
            : Frame(name, url, valid), id_(id), int_id_(intID) {
        for (uint32_t i = 0; i < id_.size(); i++) {
            if (id_[i] == '\0') {
                id_[i] = ' ';
            }
        }
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " Frame identifier: " << int_id_ << " url: " << value_
                  << " ID and additional data: " << id_ << std::endl;
    }

protected:
    uint32_t int_id_;
    std::string id_;
};

// применяется для описаниz ETCO фреймов
class ETCOframe : public Byteframe {
public:
    ETCOframe(const std::string& name, const std::string& value, uint8_t time_stamp, bool valid = true)
            : Byteframe(name, value, time_stamp, " Events: ", valid) {
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']';
        PrintTimeStampFormat(byte_);
        std::cout << std::endl << ForPrint_ << std::endl;
        PrintEventsETCO(value_);
    }
};

// применяется для описаниz EQU2 фреймов
class EQU2frame : public Byteframe {
public:
    EQU2frame(const std::string& name, const std::string& value, uint8_t interpolation_method,
              const std::string& FreqVolum, bool valid = true)
            : Byteframe(name, value, interpolation_method, " Identification: ", valid) {
        ParseEQU2(FreqVolum, volume_adjustment, frequency);
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']';
        PrintInterpolationMethod(byte_);
        std::cout << ForPrint_ << value_ << " (Frequency,Volume adjustment): ";
        for (uint32_t i = 0; i < volume_adjustment.size(); i++) {
            std::cout << '(' << frequency[i] << ',' << volume_adjustment[i] << ") ";
        }
    }

protected:
    std::vector<int16_t> volume_adjustment;
    std::vector<uint16_t> frequency;
};

// применяется для описаниz RBUF фреймов
class RBUFframe : public Frame {
public:
    RBUFframe(const std::string& name, const std::string& buff_size, bool flag, uint32_t offset, bool valid = true)
            : Frame(name, buff_size, valid), flag_(flag), offset_(offset) {
    }

    virtual void PrintValidData() {
        std::cout << '[' << name_ << ']' << " Buffer size: " << GetNum256(value_) << " Embedded info flag: " << flag_
                  << " Offset to next tag: " << offset_;
    }

protected:
    bool flag_;
    uint32_t offset_;
};

class MP3parse {
public:
    MP3parse(const std::string& path_to_mp3)
            : path_to_mp3_(path_to_mp3) {
    }

    void Parsing();

    void PrintAllDataParse() {
        for (uint32_t i = 0; i < big_data.size(); i++) {
            big_data[i]->PrintData();
        }
        for (uint32_t i = 0; i < only_frame.size(); i++) {
            only_frame[i]->PrintData();
        }
    }

    void PrintSomeDataParse(std::vector<char*>& need_frame) {
        for (uint32_t i = 0; i < big_data.size(); i++) {
            for (uint32_t j = 0; j < need_frame.size(); j++) {
                if (strcmp(need_frame[j], (big_data[i]->GetName()).c_str()) == 0) {
                    big_data[i]->PrintData();
                }
            }
        }
        for (uint32_t i = 0; i < only_frame.size(); i++) {
            for (uint32_t j = 0; j < need_frame.size(); j++) {
                if (strcmp(need_frame[j], (only_frame[i]->GetName()).c_str()) == 0) {
                    only_frame[i]->PrintData();
                }
            }
        }
    }

    ~MP3parse() {
        for (uint32_t i = 0; i < big_data.size(); i++) {
            delete big_data[i];
        }
        for (uint32_t i = 0; i < only_frame.size(); i++) {
            delete only_frame[i];
        }
    }

private:
    std::string path_to_mp3_;
    std::vector<Frame*> big_data;
    std::vector<Frame*> only_frame; // уникальные ETCO SYTC PCNT RBUF POSS OWNE SEEK
    const std::set<std::string> BadFrames{"AENC", "APIC", "ASPI", "GEOB", "MCDI", "MLLT", "RVRB", "TFLT", "TMED"};

    uint32_t ParseOneTag(std::ifstream& file);

    bool FindPlaceOfTag(std::ifstream& file);

    void UpdateFrame(Frame* frame, const std::string& name, bool UpdateTag);
};

