// 2022/08/24 14:57:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/ReceivedData.h"
#include "Data/Sensors.h"
#include "Utils/DynamicBuffer.h"
#include "Utils/Math.h"
#include "Frame.h"
#include <cstring>


namespace ReceivedData
{
    static DynamicBuffer <16>buffer;

    static bool FindFirstABC();

    static bool ParseCommand(char bytes[16]);
}


void ReceivedData::Append(uint8 *data, int size)
{
    buffer.Append(data, size);
}


void ReceivedData::Update()
{
    while (buffer.Size() >= 16 && FindFirstABC())
    {
        char bytes[16];

        std::memcpy(bytes, buffer.Data(), 16);

        buffer.RemoveFirst(16);

        if (!ParseCommand(bytes))
        {
            static int counter = 0;
            LOG_ERROR("Can not parse command %d", counter++);
        }
    }
}


bool ReceivedData::FindFirstABC()
{
    int removed_bytes = 0;

    while (buffer.Size() >= 3)
    {
        if (std::memcmp("ABC", buffer.Data(), 3) == 0)
        {
            return true;
        }

        buffer.RemoveFirst(1);

        removed_bytes++;
    }

    if (removed_bytes != 0)
    {
        static int counter = 0;
        LOG_ERROR("error command %d", counter++);
    }

    return false;
}


bool ReceivedData::ParseCommand(char message[16])
{
    uint8 type = message[3];

    uint id = 0;

    std::memcpy(&id, &message[4], 4);

    uint hash = 0;

    std::memcpy(&hash, &message[8], 4);

    float value = 0.0f;

    std::memcpy(&value, &message[12], 4);

    if (Math::CalculateHash(&value, 4) == hash)
    {
        Sensor::Pool::AppendMeasure(id, type, value);

        return true;
    }

    return false;
}
