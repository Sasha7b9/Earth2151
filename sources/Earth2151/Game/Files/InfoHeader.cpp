// 2025/03/05 09:33:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Files/InfoHeader.h"


bool InfoHeader::Fill(FileReader &reader)
{
    bool flag = false;

    int pointer = 0;

    for (int i = 0; i < 32; i++)
    {
        pointer = reader.pointer;

        uint8 pattern[8] = { 0x50, 0x41, 0x52, 0x00, 0x99, 0x00, 0x00, 0x00 };

        uint8 bytes[8];

        reader.ReadBuffer(bytes, 8);

        if (std::memcmp(pattern, bytes, 8) == 0)
        {
            flag = true;
            pointer = reader.pointer;
            break;
        }

        reader.pointer = pointer + 1;
    }

    if (!flag)
    {
        return true;
    }

    flags.value = (FileFlags::E)reader.ReadByte();

    if (flags.IsNamed())
    {
        translationId = reader.ReadString();
    }

    if (flags.IsResource())
    {
        resourceType.value = (ResType::E)reader.ReadUInt();
    }

    if (flags.IsGUID())
    {
        reader.ReadBuffer(guid, 16);
    }
    else
    {
        std::memset(guid, 0, 16);
    }

    reader.pointer = pointer;

    return true;
}
