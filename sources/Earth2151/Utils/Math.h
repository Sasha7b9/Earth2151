// 2025/02/14 22:45:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace M
{
    int ResolveMathExpression(pchar);

    uint  ColorToUINT(const ColorRGBA &);

    float LimitationFloat(float value, float min, float max);

    ColorRGBA OneComplement(const ColorRGBA &);

    ColorRGBA UINTtoColor(uint);

    ColorRGBA SetColorBrightness(const ColorRGBA &, float bightness);       // brightness = 0.0f ... 1.0F;

    bool PointInRect(const Point2D *point, float xRect, float yRect, float widthRect, float heightRect);

    bool BetweenTwoPoints(const Point2D &p0, const Point2D &p1, const Point2D &value);

    template<class T>
    void Swap(T &value1, T &value2)
    {
        T temp = value1;
        value1 = value2;
        value2 = temp;
    }

    // Упорядочить по возрастанию
    template<class T>
    void PutInOrder(T &value1, T &value2)
    {
        if (value2 < value1)
        {
            Swap<T>(value1, value2);
        }
    }

    inline uint32 Hash(const uint8 *buffer, int size)
    {
        uint32 hash = 0;
        for (int i = 0; i < size; i++)
        {
            uint32 c = *reinterpret_cast<const uint8 *>(buffer);

            hash ^= c;
            hash = hash * 0x6B84DF47U + 1;

            buffer++;
        }

        return (hash);
    }
}
