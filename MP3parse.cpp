//
// Created by Елизавета Кравченкова on 08.02.2023.
//
#include "MP3parse.h"

std::u16string GetWC2(const std::string& str1) {
    std::u16string wideString;
    for (uint32_t i = 0; i < str1.size(); i++) {
        uint16_t q = 0;
        q = (uint16_t) str1[i] * 256 + (uint16_t) str1[i + 1];
        i++;
        char16_t qc = q;
        wideString.push_back(qc);
    }
    return wideString;
}

std::u16string GetWC3(const std::string& str1) {
    std::u16string wideString;
    for (uint32_t i = 0; i < str1.size(); i++) {
        uint16_t q = 0;
        q = (uint16_t) str1[i] + (uint16_t) str1[i + 1] * 256;
        i++;
        char16_t qc = q;
        wideString.push_back(qc);
    }

    return wideString;
}

std::string utf16_to_utf8(const std::string& bad_utf16, uint8_t code) {
    std::u16string u16str;

    if (code == 1) {
        u16str = GetWC3(bad_utf16);
    } else {
        u16str = GetWC2(bad_utf16);
    }

    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    std::string utf8 = convert.to_bytes(u16str);
    return utf8;

}

std::string DecondUnsynchronisation(const std::string& www) {
    std::string ans;
    for (uint64_t i = 0; i < www.size(); i++) {
        if ((uint8_t) www[i] == 0xFF) {
            ans.push_back(www[i]);
            i++;
            if (www[i] != 0) {
                ans.push_back(www[i]);
            }
        } else {
            ans.push_back(www[i]);
        }
    }
    return ans;
}

std::string DecoderCode(const std::string& str1, uint8_t code) {
    if (code == 3 or code == 0) {
        return str1;
    }
    return utf16_to_utf8(str1, code);
}

void PrintTimeStampFormat(uint8_t byte) {
    switch (byte) {
        case 1:
            std::cout << " time_stamp_format: Absolute time, 32 bit sized, using MPEG frames as unit";
            break;
        case 2:
            std::cout << " time_stamp_format: Absolute time, 32 bit sized, using milliseconds as unit";
            break;
        default:
            std::cout << "Невалидная информация";
    }
};

void PrintTypeOfEvent(uint8_t byte) {
    if (byte >= 0x17 and byte <= 0xDF) {
        std::cout << " Type of event: reserved for future use";
        return;
    }
    if (byte >= 0xE0 and byte <= 0xEF) {
        std::cout << " Type of event: not predefined synch 0-F";
        return;
    }
    if (byte >= 0xF0 and byte <= 0xFC) {
        std::cout << " Type of event: reserved for future use";
        return;
    }
    switch (byte) {
        case 0x00:
            std::cout << " Type of event: padding (has no meaning)";
            break;
        case 0x01:
            std::cout << " Type of event: end of initial silence";
            break;
        case 0x02:
            std::cout << " Type of event: intro start";
            break;
        case 0x03:
            std::cout << " Type of event: main part start";
            break;
        case 0x04:
            std::cout << " Type of event: outro start";
            break;
        case 0x05:
            std::cout << " Type of event: outro end";
            break;
        case 0x06:
            std::cout << " Type of event: verse start";
            break;
        case 0x07:
            std::cout << " Type of event: refrain start";
            break;
        case 0x08:
            std::cout << " Type of event: interlude start";
            break;
        case 0x09:
            std::cout << " Type of event: theme start";
            break;
        case 0x0A:
            std::cout << " Type of event: variation start";
            break;
        case 0x0B:
            std::cout << " Type of event: key change";
            break;
        case 0x0C:
            std::cout << " Type of event: time change";
            break;
        case 0x0D:
            std::cout << " Type of event: momentary unwanted noise (Snap, Crackle & Pop)";
            break;
        case 0x0E:
            std::cout << " Type of event: sustained noise";
            break;
        case 0x0F:
            std::cout << " Type of event: sustained noise end";
            break;
        case 0x10:
            std::cout << " Type of event: intro end";
            break;
        case 0x11:
            std::cout << " Type of event: main part end";
            break;
        case 0x12:
            std::cout << " Type of event: verse end";
            break;
        case 0x13:
            std::cout << " Type of event: refrain end";
            break;
        case 0x14:
            std::cout << " Type of event: theme end";
            break;
        case 0x15:
            std::cout << " Type of event: profanity";
            break;
        case 0x16:
            std::cout << " Type of event: profanity end";
            break;
        case 0xFD:
            std::cout << " Type of event: audio end (start of silence)";
            break;
        case 0xFE:
            std::cout << " Type of event: audio file ends";
            break;
        case 0xFF:
            std::cout
                    << " Type of event: one more byte of events follows (all the following bytes with the value $FF have the same function)";
            break;
        default:
            std::cout << "Невалидная информация";

    }
};

void PrintReceviedAs(uint8_t byte) {
    switch (byte) {
        case 0:
            std::cout << " received as: other ";
            break;
        case 1:
            std::cout << " received as: Standard CD album with other songs ";
            break;
        case 2:
            std::cout << " received as: Compressed audio on CD ";
            break;
        case 3:
            std::cout << " received as: File over the Internet ";
            break;
        case 4:
            std::cout << " received as: Stream over the Internet ";
            break;
        case 5:
            std::cout << " received as: As note sheets ";
            break;
        case 6:
            std::cout << " received as: As note sheets in a book with other sheets ";
            break;
        case 7:
            std::cout << " received as: Music on other media ";
            break;
        case 8:
            std::cout << " received as: Non-musical merchandise ";
            break;
        default:
            std::cout << "Невалидная информация";
    }
}

void PrintInterpolationMethod(uint8_t byte) {
    switch (byte) {
        case 0x00:
            std::cout
                    << " Interpolation method: Band. No interpolation is made. A jump from one adjustment level to another occurs in the middle between two adjustment points.";
            break;
        case 0x01:
            std::cout << " Linear.Interpolation between adjustment points is linear.";
            break;
        default:
            std::cout << "Невалидная информация";
    }
}

void PrintContentType(uint8_t byte) {
    switch (byte) {
        case 0x00:
            std::cout << " Content type: is other";
            break;
        case 0x01:
            std::cout << " Content type: is lyrics";
            break;
        case 0x02:
            std::cout << " Content type: is text transcription";
            break;
        case 0x03:
            std::cout << " Content type: is movement/part name (e.g. \"Adagio\")";
            break;
        case 0x04:
            std::cout << " Content type: is events (e.g. \"Don Quijote enters the stage\")";
            break;
        case 0x05:
            std::cout << " Content type: is chord (e.g. \"Bb F Fsus\")";
            break;
        case 0x06:
            std::cout << " Content type: is trivia/'pop up' information";
            break;
        case 0x07:
            std::cout << " Content type: is URLs to webpages";
            break;
        case 0x08:
            std::cout << " Content type: is URLs to images";
            break;
        default:
            std::cout << "Невалидная информация";
    }
}

void PrintTypeOfChannel(uint8_t byte) {
    switch (byte) {
        case 0x00:
            std::cout << " Type of channel: Other";
            break;
        case 0x01:
            std::cout << " Type of channel: Master volume";
            break;
        case 0x02:
            std::cout << " Type of channel: Front right";
            break;
        case 0x03:
            std::cout << " Type of channel: Front left";
            break;
        case 0x04:
            std::cout << " Type of channel: Back right";
            break;
        case 0x05:
            std::cout << " Type of channel: Back left";
            break;
        case 0x06:
            std::cout << " Type of channel: Front centre";
            break;
        case 0x07:
            std::cout << " Type of channel: Back centre";
            break;
        case 0x08:
            std::cout << " Type of channel: Subwoofer";
            break;
        default:
            std::cout << "Невалидная информация";
    }
}

void PrintEventsETCO(const std::string& value) {
    for (uint32_t i = 0; i < value.size(); i++) {
        PrintTypeOfEvent(value[i]);
        i++;
        uint32_t ans = 0;
        for (uint8_t k = 0; k < 4; k++) {
            ans = ans * 256 + value[k + i];
        }
        i = i + 3;
        std::cout << " Time stamp: " << ans << std::endl;
    }
}

void PrintEventsSYLT(const std::string& value) {
    uint8_t null_byte = 0x0;
    uint32_t i = 0;
    bool start = true;
    while (i < value.size()) {
        while (value[i] != null_byte) {
            if (start) {
                std::cout << " Text: ";
                start = false;
            }
            std::cout << value[i];
            i++;
        }
        i++;
        uint32_t time = 0;
        for (uint32_t k = 0; k < 4; k++) {
            time = time * 256 + static_cast<uint8_t>(value[i + k]);
        }
        i = i + 4;
        std::cout << " Time: " << time << std::endl;
        start = true;
    }
}

// превращаем строку байтов в uint число
uint32_t GetNum256(const std::string& str) {
    uint32_t ans = 0;
    for (uint32_t i = 0; i < str.size(); i++) {
        ans = ans * pow(2, 8) + static_cast<uint8_t>(str[i]);
    }
    return ans;
}

void
ParseEQU2(const std::string& FreqVolum, std::vector<int16_t>& volume_adjustment, std::vector<uint16_t>& frequency) {
    uint32_t half_counter = 0;
    uint32_t str_size = FreqVolum.size();
    volume_adjustment.resize(str_size / 4);
    frequency.resize(str_size / 4);
    for (uint32_t i = 0; i < str_size; i = i + 1) {
        if (half_counter % 2 == 0) {
            frequency[half_counter] = static_cast<uint16_t>(FreqVolum[i]) * pow(2, 8);
            i++;
            frequency[half_counter] += static_cast<uint16_t>(FreqVolum[i]);
        } else {
            volume_adjustment[half_counter] = static_cast<int16_t>(FreqVolum[i]) * pow(2, 8);
            i++;
            volume_adjustment[half_counter] += static_cast<int16_t>(FreqVolum[i]);
        }
        half_counter++;
    }
}

bool ParseRVA2(const std::string& other, std::vector<uint8_t>& channel_type, std::vector<int16_t>& volume_adjustment_,
               std::vector<uint8_t>& bits_representing_, std::vector<uint32_t>& peak_volume_) {
    for (uint32_t i = 0; i < other.size(); i++) {
        channel_type.push_back(other[i]);
        if (i + 3 >= other.size()) {
            return false;
        }
        volume_adjustment_.push_back(
                static_cast<int16_t>((static_cast<int>(other[i + 1]) * pow(2, 8) + other[i + 2]) / 512));
        bits_representing_.push_back(other[i + 3]);
        if (other[i + 3] == 0) {
            peak_volume_.push_back(0);
        } else {
            peak_volume_.push_back(GetNum256(other.substr(i + 4, other[i + 3])));
        }
        i = i + 3 + other[i + 3];
    }
    return true;
}

bool MP3parse::FindPlaceOfTag(std::ifstream& file) {
    uint8_t q;
    while (file.good()) {
        uint64_t position = file.tellg();
        q = file.get();
        if (q == 'I') {
            std::string str(2, ' ');
            file.read(str.data(), 2);
            if (str == "D3") {
                q = file.get();
                // версия
                if (q == 4) {
                    uint8_t w;
                    w = file.get();
                    if (w == 0) {
                        // байт флагов
                        w = file.get();
                        // проверка что в конце 0000
                        if (w % 16 == 0) {
                            bool ok = true;
                            for (int i = 0; i < 4; i++) {
                                q = file.get();
                                // проверка что в SynchInt в начале 0
                                if (q >= 128) {
                                    ok = false;
                                    break;
                                }
                            }
                            if (ok) {
                                // возвращаемся в начало тега
                                file.seekg(position, std::ios_base::beg);
                                return true;
                            }

                        }

                    }

                }

            }
        }
    }
    return false;

}

// получение бинарного представления числа
void GetBin(uint8_t x, bool bin_ch[]) {
    for (int i = 7; i >= 0; i--) {
        bin_ch[i] = x % 2;
        x = x / 2;
    }
}

uint32_t GetUintNumber(bool bin_ch[]) {
    uint32_t ans = 0;
    int q = 6;
    for (int i = 1; i < 8; i++) {
        ans = ans + pow(2, q) * bin_ch[i];
        q--;
    }
    return ans;
}

uint32_t DecodeSynchInt(bool bin_ch1[], bool bin_ch2[], bool bin_ch3[], bool bin_ch4[]) {
    return GetUintNumber(bin_ch1) * pow(2, 21) + GetUintNumber(bin_ch2) * pow(2, 14) +
           GetUintNumber(bin_ch3) * pow(2, 7) +
           GetUintNumber(bin_ch4);
}

// считывает 32 bit synchsafe integer
uint32_t ReadSynchInt(std::ifstream& file) {
    uint8_t first_bit_of_size = file.get();
    bool first_bit_of_size_bin[8];
    GetBin(first_bit_of_size, first_bit_of_size_bin);

    uint8_t second_bit_of_size = file.get();
    bool second_bit_of_size_bin[8];
    GetBin(second_bit_of_size, second_bit_of_size_bin);

    uint8_t third_bit_of_size = file.get();
    bool third_bit_of_size_bin[8];
    GetBin(third_bit_of_size, third_bit_of_size_bin);

    uint8_t fourth_bit_of_size = file.get();
    bool fourth_bit_of_size_bin[8];
    GetBin(fourth_bit_of_size, fourth_bit_of_size_bin);

    return DecodeSynchInt(first_bit_of_size_bin, second_bit_of_size_bin, third_bit_of_size_bin, fourth_bit_of_size_bin);
}

// для  фрейма
void MP3parse::UpdateFrame(Frame* frame, const std::string& name, bool UpdateTag) {
    bool find = false;
    if (UpdateTag) {
        for (uint32_t i = 0; i < only_frame.size(); i++) {
            if (only_frame[i]->GetName() == name) {
                only_frame[i] = frame; // если это тег-обновление, то обновляем тег
                find = true;
                break;
            }
        }
    }
    if (!find) {
        only_frame.push_back(frame);
    }
}

// возвращает расстояние до следующего тега
uint32_t MP3parse::ParseOneTag(std::ifstream& file) {
    uint32_t dist = 0;
    const uint32_t kSizeOfHeader = 10;
    const uint32_t kSizeOfFramesHeader = 10;
    bool UpdateTag = false;

    uint64_t begin_position = file.tellg();
    // перемещаемся к флагам
    file.seekg(5, std::ios_base::cur);

    uint8_t tag_flag = file.get();
    bool bin_tag_flag[8];
    GetBin(tag_flag, bin_tag_flag);

    bool dop_header = bin_tag_flag[1];
    bool unsynchronisation = bin_tag_flag[0];

    uint32_t size_of_tag = ReadSynchInt(file);
    // учитывая данные о нижнем контикуле высчитываем размер всего тега
    size_of_tag = size_of_tag + kSizeOfHeader * bin_tag_flag[3] + kSizeOfHeader;

    uint32_t size_of_dop_header = 0;
    if (dop_header) {
        size_of_dop_header = ReadSynchInt(file);

        uint32_t flag_of_dop_header = file.get(); // в нашей версии-всегда равно 1
        flag_of_dop_header = file.get();
        bool bin_flag_of_dop_header[8];
        GetBin(flag_of_dop_header, bin_flag_of_dop_header);

        if (bin_flag_of_dop_header[1]) {
            UpdateTag = true;
        }
        file.seekg(size_of_dop_header - 6, std::ios_base::cur); //уже прочитали 6 байт
    }

    // Frames
    uint32_t size_of_frames_plus_padding =
            size_of_tag - kSizeOfHeader - size_of_dop_header - kSizeOfHeader * bin_tag_flag[3];
    uint32_t have_read = 0; // уже прочитано из size_of_frames_plus_padding

    while (have_read < size_of_frames_plus_padding) {
        std::string name_of_frame(4, ' ');
        file.read(name_of_frame.data(), 4);

        // это padding
        if (name_of_frame[0] == 0) {
            // переход в конец тега
            file.seekg(begin_position + size_of_tag, std::ios_base::beg);
            break;
        } else {
            // это frame
            uint32_t size_of_frame = ReadSynchInt(file);
            // проверяем, нужно ли нам обрабатывать  frame с таким именем
            if (BadFrames.find(name_of_frame) != BadFrames.end()) {
                have_read = have_read + size_of_frame + kSizeOfFramesHeader; //
                file.seekg(size_of_frame + 2, std::ios_base::cur);
            } else {
                uint8_t flag_of_frame_1 = file.get();
                uint8_t flag_of_frame_2 = file.get();
                bool bin_flag_of_frame_2[8];
                GetBin(flag_of_frame_2, bin_flag_of_frame_2);

                uint8_t count_of_inf_byte = 0; //биты, которые могут добавится после header

                if (bin_flag_of_frame_2[1]) {
                    count_of_inf_byte++;
                }

                if (bin_flag_of_frame_2[5]) {
                    count_of_inf_byte++;
                }

                if (bin_flag_of_frame_2[7]) {
                    count_of_inf_byte = count_of_inf_byte + 4;
                }

                std::string value_of_frame(size_of_frame, ' ');
                file.read(value_of_frame.data(), size_of_frame);

                if (bin_flag_of_frame_2[6] or unsynchronisation) {
                    value_of_frame = DecondUnsynchronisation(value_of_frame);
                }
                have_read = have_read + size_of_frame + kSizeOfFramesHeader;
                uint8_t null_byte = 0; //введено для поиска символа-разделителя в строке
                value_of_frame = value_of_frame.substr(count_of_inf_byte);

                if (name_of_frame == "TXXX" or name_of_frame == "WXXX") {
                    uint32_t index_null = value_of_frame.find(null_byte, 1);
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or
                        ((index_null + 1) >= value_of_frame.size())) {
                        XXframe* frame = new XXframe(name_of_frame, value_of_frame, 0, "", false);
                        big_data.push_back(frame);
                    } else {
                        XXframe* frame;
                        if (name_of_frame == "TXXX") {
                            frame = new XXframe(name_of_frame,
                                                DecoderCode(value_of_frame.substr(index_null + 1), value_of_frame[0]),
                                                value_of_frame[0], value_of_frame.substr(1, index_null - 1));
                        } else {
                            frame = new XXframe(name_of_frame, value_of_frame.substr(index_null + 1),
                                                value_of_frame[0], value_of_frame.substr(1, index_null - 1));
                        }

                        big_data.push_back(frame);
                    }
                } else if (name_of_frame == "POSS" or name_of_frame == "SYTC") {
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or value_of_frame.size() <= 1) {
                        Byteframe* frame = new Byteframe(name_of_frame, value_of_frame, 0, "", false);
                        // для уникальных фреймов возможно потребуется перезапись
                        UpdateFrame(frame, name_of_frame, UpdateTag);
                    } else {
                        Byteframe* frame;
                        if (name_of_frame == "POSS") {
                            frame = new Byteframe(name_of_frame, value_of_frame.substr(1), value_of_frame[0],
                                                  " Position: ",
                                                  true);
                        } else {
                            frame = new Byteframe(name_of_frame, value_of_frame.substr(1), value_of_frame[0],
                                                  " Tempo data: ",
                                                  true);
                        }
                        UpdateFrame(frame, name_of_frame, UpdateTag);
                    }
                } else if (name_of_frame[0] == 'T' or name_of_frame[0] == 'W' or name_of_frame == "SEEK") {
                    if (name_of_frame == "SEEK") {
                        dist = GetNum256(value_of_frame); // расстояние до следующего тега от конца этого
                        value_of_frame = std::to_string(dist);
                    }
                    // применено сжатие или шифрование
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5]) {
                        if (name_of_frame[0] == 'T') {
                            Frame* frame = new Frame(name_of_frame, value_of_frame, 0, false);
                            big_data.push_back(frame);
                        } else {
                            Frame* frame = new Frame(name_of_frame, value_of_frame, false);
                            if (name_of_frame == "SEEK") {
                                UpdateFrame(frame, name_of_frame, UpdateTag);
                            } else {
                                big_data.push_back(frame);
                            }

                        }
                    } else {
                        if (name_of_frame[0] == 'T') {
                            Frame* frame = new Frame(name_of_frame, value_of_frame.substr(1), value_of_frame[0], true);
                            big_data.push_back(frame);
                        } else {
                            Frame* frame = new Frame(name_of_frame, value_of_frame, true);
                            if (name_of_frame == "SEEK") {
                                UpdateFrame(frame, name_of_frame, UpdateTag);
                            } else {
                                big_data.push_back(frame);
                            }
                        }
                    }


                } else if (name_of_frame == "ENCR" or name_of_frame == "GRID") {
                    uint32_t index_null = value_of_frame.find(null_byte);
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or
                        ((index_null + 2) >= value_of_frame.size())) {
                        ByteIDframe* frame = new ByteIDframe(name_of_frame, value_of_frame, 0, "", "", "", false);
                        big_data.push_back(frame);
                    } else {
                        ByteIDframe* frame;
                        if (name_of_frame == "ENCR") {
                            ByteIDframe* frame = new ByteIDframe(name_of_frame, value_of_frame.substr(index_null + 2),
                                                                 value_of_frame[index_null + 1],
                                                                 value_of_frame.substr(0, index_null),
                                                                 " Method symbol : ", " Encryption data: ", true);
                        } else {
                            ByteIDframe* frame = new ByteIDframe(name_of_frame, value_of_frame.substr(index_null + 2),
                                                                 value_of_frame[index_null + 1],
                                                                 value_of_frame.substr(0, index_null),
                                                                 " Group symbol : ", " Group dependent data: ", true);
                        }
                        big_data.push_back(frame);
                    }
                } else if (name_of_frame == "UFID" or name_of_frame == "PRIV") {
                    uint32_t index_null = value_of_frame.find(null_byte);
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or
                        ((index_null + 1) >= value_of_frame.size())) {
                        TwoTextframe* frame = new TwoTextframe(name_of_frame, value_of_frame, " ", " ", false);
                        big_data.push_back(frame);
                    } else {
                        TwoTextframe* frame;
                        if (name_of_frame == "UFID") {
                            TwoTextframe* frame = new TwoTextframe(name_of_frame, value_of_frame.substr(index_null + 1),
                                                                   value_of_frame.substr(0, index_null),
                                                                   " Identifier: ", true);
                        } else {
                            TwoTextframe* frame = new TwoTextframe(name_of_frame, value_of_frame.substr(index_null + 1),
                                                                   value_of_frame.substr(0, index_null),
                                                                   " The private data: ", true);
                        }
                        big_data.push_back(frame);
                    }
                } else if (name_of_frame == "COMM" or name_of_frame == "USLT") {
                    uint32_t index_null = value_of_frame.find(null_byte, 1);
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or
                        ((index_null + 1) >= value_of_frame.size())) {
                        LangXXFrame* frame = new LangXXFrame(name_of_frame, value_of_frame,
                                                             0, "",
                                                             "", false);
                        big_data.push_back(frame);
                    } else {
                        LangXXFrame* frame = new LangXXFrame(name_of_frame, value_of_frame.substr(index_null + 1),
                                                             value_of_frame[0], value_of_frame.substr(1, 3),
                                                             value_of_frame.substr(4, index_null - 4), true);
                        big_data.push_back(frame);
                    }

                } else if (name_of_frame == "SIGN") {
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or value_of_frame.size() <= 1) {
                        SIGNframe* frame = new SIGNframe(name_of_frame, value_of_frame, 0, false);
                        big_data.push_back(frame);
                    } else {
                        SIGNframe* frame = new SIGNframe(name_of_frame, value_of_frame.substr(1), value_of_frame[0],
                                                         true);
                        big_data.push_back(frame);
                    }
                } else if (name_of_frame == "USER") {
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or value_of_frame.size() <= 3) {
                        LangTFrame* frame = new LangTFrame(name_of_frame, value_of_frame, 0, "", false);
                        big_data.push_back(frame);
                    } else {
                        LangTFrame* frame = new LangTFrame(name_of_frame, value_of_frame.substr(1), value_of_frame[0],
                                                           value_of_frame.substr(1, 3), true);
                        big_data.push_back(frame);
                    }


                } else if (name_of_frame == "OWNE") {
                    uint32_t index_null = value_of_frame.find(null_byte, 1);

                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or
                        ((index_null + 9) >= value_of_frame.size())) {
                        OWNEframe* frame = new OWNEframe(name_of_frame, 0, value_of_frame, "", "", false);
                        UpdateFrame(frame, name_of_frame, UpdateTag);
                    } else {
                        OWNEframe* frame = new OWNEframe(name_of_frame, value_of_frame[0],
                                                         value_of_frame.substr(1, index_null - 1),
                                                         value_of_frame.substr(index_null + 1, 8),
                                                         value_of_frame.substr(index_null + 9), true);
                        UpdateFrame(frame, name_of_frame, UpdateTag);
                    }

                } else if (name_of_frame == "COMR") {
                    uint8_t count_of_null = 0;
                    // количество нулевых байтов
                    for (uint32_t i = 0; i < value_of_frame.size(); i++) {
                        if (value_of_frame[i] == 0x0) {
                            count_of_null++;
                        }
                    }
                    std::string::size_type first_null = value_of_frame.find(null_byte, 1);
                    std::string::size_type second_null = value_of_frame.find(null_byte, first_null + 1);
                    std::string::size_type third_null = value_of_frame.find(null_byte, second_null +
                                                                                       2); // там может быть нулевой флаг
                    std::string::size_type fouth_null = value_of_frame.find(null_byte, third_null + 1);

                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or count_of_null < 3) {
                        COMRframe* frame = new COMRframe(name_of_frame, value_of_frame[0], "", "", "",
                                                         value_of_frame[second_null + 1], value_of_frame, "", "",
                                                         "", false, false);
                        big_data.push_back(frame);
                    }
                    bool bad = false;
                    if (count_of_null >= 4) {
                        // есть картинка
                        std::string::size_type fifth_null = value_of_frame.find(null_byte, fouth_null + 1);
                        if (fifth_null == std::string::npos) {
                            // нет picture_description
                            //неправильный формат строки
                            if ((fouth_null + 1) >= value_of_frame.size()) {
                                bad = true;
                            }
                            COMRframe* frame = new COMRframe(name_of_frame, value_of_frame[0],
                                                             value_of_frame.substr(1, first_null - 1),
                                                             value_of_frame.substr(first_null + 1, 8),
                                                             value_of_frame.substr(first_null + 9,
                                                                                   second_null - first_null - 9),
                                                             value_of_frame[second_null + 1],
                                                             value_of_frame.substr(second_null + 2,
                                                                                   third_null - second_null - 2),
                                                             value_of_frame.substr(third_null + 1), "",
                                                             value_of_frame.substr(fouth_null + 1), false,
                                                             true);
                            big_data.push_back(frame);
                        } else {
                            if ((fouth_null + 1) >= value_of_frame.size()) {
                                bad = true;
                            }
                            // есть picture_description
                            COMRframe* frame = new COMRframe(name_of_frame, value_of_frame[0],
                                                             value_of_frame.substr(1, first_null - 1),
                                                             value_of_frame.substr(first_null + 1, 8),
                                                             value_of_frame.substr(first_null + 9,
                                                                                   second_null - first_null - 9),
                                                             value_of_frame[second_null + 1],
                                                             value_of_frame.substr(second_null + 2,
                                                                                   third_null - second_null - 2),
                                                             value_of_frame.substr(third_null + 1),
                                                             value_of_frame.substr(fouth_null + 1,
                                                                                   fifth_null - fouth_null - 1),
                                                             value_of_frame.substr(first_null + 1), false,
                                                             true);
                            big_data.push_back(frame);
                        }

                    } else if (count_of_null == 3) {
                        if ((third_null + 1) >= value_of_frame.size()) {
                            bad = true;
                        }
                        // нет картинки
                        // применено сжатие или шифрование
                        COMRframe* frame = new COMRframe(name_of_frame, value_of_frame[0],
                                                         value_of_frame.substr(1, first_null - 1),
                                                         value_of_frame.substr(first_null + 1, 8),
                                                         value_of_frame.substr(first_null + 9,
                                                                               second_null - first_null - 9),
                                                         value_of_frame[second_null + 1],
                                                         value_of_frame.substr(second_null + 2,
                                                                               third_null - second_null - 2),
                                                         value_of_frame.substr(third_null + 1), " ", " ", false,
                                                         true);
                        big_data.push_back(frame);
                    }

                    if (bad) {
                        //неправильный формат строки
                        if (bin_flag_of_frame_2[4] == 1 or bin_flag_of_frame_2[5] == 1 or count_of_null < 3) {
                            COMRframe* frame = new COMRframe(name_of_frame, value_of_frame[0], "", "", "",
                                                             value_of_frame[second_null + 1], value_of_frame, "", "",
                                                             "", false, false);
                            big_data.push_back(frame);
                        }
                    }

                } else if (name_of_frame == "SYLT") {
                    std::string::size_type index_null = value_of_frame.find(null_byte,
                                                                            1); // первый байт может быть быть null(кодирровка)
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or (index_null + 1) >= value_of_frame.size()) {
                        SYLTframe* frame = new SYLTframe(name_of_frame, 3, " ", 0, 0, value_of_frame, "", false);
                        big_data.push_back(frame);
                    } else {
                        SYLTframe* frame = new SYLTframe(name_of_frame, value_of_frame[0], value_of_frame.substr(1, 3),
                                                         value_of_frame[4], value_of_frame[5],
                                                         value_of_frame.substr(6, index_null - 6),
                                                         value_of_frame.substr(index_null + 1), true);
                        big_data.push_back(frame);
                    }
                } else if (name_of_frame == "PCNT") {
                    // применено сжатие или шифрование
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5]) {
                        PCNTframe* frame = new PCNTframe(name_of_frame, value_of_frame, false);
                        UpdateFrame(frame, name_of_frame, UpdateTag);
                    } else {
                        PCNTframe* frame = new PCNTframe(name_of_frame, value_of_frame, true);
                        UpdateFrame(frame, name_of_frame, UpdateTag);
                    }
                } else if (name_of_frame == "POPM") {
                    uint32_t index_null = value_of_frame.find(null_byte);
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or
                        ((index_null + 1) >= value_of_frame.size())) {
                        POPMframe* frame = new POPMframe(name_of_frame, value_of_frame, " ", 0, false);
                        big_data.push_back(frame);
                    } else {
                        POPMframe* frame = new POPMframe(name_of_frame, value_of_frame.substr(index_null + 2),
                                                         value_of_frame.substr(0, index_null),
                                                         value_of_frame[index_null + 1], true);
                        big_data.push_back(frame);
                    }
                } else if (name_of_frame == "RVA2") {
                    uint32_t index_null = value_of_frame.find(null_byte);
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or
                        ((index_null + 1) >= value_of_frame.size())) {
                        RVA2frame* frame = new RVA2frame(name_of_frame, value_of_frame, "", false);
                        big_data.push_back(frame);
                    } else {
                        RVA2frame* frame = new RVA2frame(name_of_frame, value_of_frame.substr(0, index_null),
                                                         value_of_frame.substr(index_null + 1),
                                                         true);
                        big_data.push_back(frame);
                    }
                } else if (name_of_frame == "LINK") {
                    uint32_t index_null = value_of_frame.find(null_byte);
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or
                        (std::max((index_null + 1), (uint32_t) 4) >= value_of_frame.size())) {
                        LinkFrame* frame = new LinkFrame(name_of_frame, value_of_frame, "", 0, false);
                        big_data.push_back(frame);
                    } else {
                        LinkFrame* frame = new LinkFrame(name_of_frame, value_of_frame.substr(4, index_null - 4),
                                                         value_of_frame.substr(index_null + 1),
                                                         GetNum256(value_of_frame.substr(0, 4)), true);
                        big_data.push_back(frame);
                    }
                } else if (name_of_frame == "ETCO") {
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or value_of_frame.size() <= 2) {
                        ETCOframe* frame = new ETCOframe(name_of_frame, value_of_frame, 0, false);
                        UpdateFrame(frame, name_of_frame, UpdateTag);
                    } else {
                        ETCOframe* frame = new ETCOframe(name_of_frame, value_of_frame.substr(1), value_of_frame[0],
                                                         true);
                        UpdateFrame(frame, name_of_frame, UpdateTag);
                    }
                } else if (name_of_frame == "EQU2") {
                    uint32_t index_null = value_of_frame.find(null_byte);
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or
                        ((index_null + 1) >= value_of_frame.size())) {
                        EQU2frame* frame = new EQU2frame(name_of_frame, value_of_frame, 0, "", false);
                        big_data.push_back(frame);
                    } else {
                        EQU2frame* frame = new EQU2frame(name_of_frame, value_of_frame.substr(1, index_null - 1),
                                                         value_of_frame[0], value_of_frame.substr(index_null + 1),
                                                         true);
                        big_data.push_back(frame);
                    }
                } else if (name_of_frame == "RBUF") {
                    uint32_t index_null = value_of_frame.find(null_byte);
                    // применено сжатие или шифрование или неправильный формат строки
                    if (bin_flag_of_frame_2[4] or bin_flag_of_frame_2[5] or value_of_frame.size() <= 4) {
                        RBUFframe* frame = new RBUFframe(name_of_frame, value_of_frame, " ", 0, false);
                        UpdateFrame(frame, name_of_frame, UpdateTag);
                    } else {
                        RBUFframe* frame = new RBUFframe(name_of_frame,
                                                         std::to_string(GetNum256(value_of_frame.substr(0, 3))),
                                                         value_of_frame[3] % 2, GetNum256(value_of_frame.substr(4)),
                                                         true);
                        UpdateFrame(frame, name_of_frame, UpdateTag);
                    }
                } else {
                    Frame* frame = new Frame(name_of_frame, value_of_frame, false);
                    big_data.push_back(frame);
                }
            }
        }
    }
    return dist;
}

void MP3parse::Parsing() {
    std::ifstream file(path_to_mp3_, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << " Ошибка открытия файла1";
        exit(1);
    }
    // поиск ближаайшего тега
    if (!FindPlaceOfTag(file)) {
        // тегов в файле нет
        return;
    };

    uint32_t dist = ParseOneTag(file);
    while (dist != 0 and file.good()) {
        file.seekg(dist, std::ios_base::cur);
        dist = ParseOneTag(file);
    }
}


