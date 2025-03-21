// 2025/02/14 22:42:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Controls/Canvas.h"
#include "Utils/Math.h"


Canvas::Canvas(const Vector2D &size) : Widget()
{
    Widget::SetWidgetSize(size);

    colorBackground = Color::black;
    colorBrush = Color::white;

    textureHeader = new TextureHeader();

    textureHeader->textureType = kTextureType2D;
    textureHeader->textureFlags = kTextureDynamic;
    textureHeader->colorSemantic = kTextureSemanticDiffuse;
    textureHeader->alphaSemantic = kTextureSemanticTransparency;
    textureHeader->textureFormat = kTextureFormatLinearRGBA;
    textureHeader->textureWidth = (int)size.x;
    textureHeader->textureHeight = (int)size.y;
    textureHeader->textureDepth = 1;
    textureHeader->wrapMode[0] = kTextureWrapClamp;
    textureHeader->wrapMode[1] = kTextureWrapClamp;
    textureHeader->wrapMode[2] = kTextureWrapClamp;
    textureHeader->mipmapCount = 1;
    textureHeader->mipmapDataOffset = 0;
    textureHeader->auxiliaryDataSize = 0;
    textureHeader->auxiliaryDataOffset = 0;

    imageData = new uint[(size_t)(size.x * size.y)];

    texture = Texture::GetTexture(textureHeader, imageData);

    image = new ImageWidget(size, texture);

    Clear();

    AppendNewSubnode(image);
}


Canvas::Canvas(const Canvas *rhs) : Widget()
{
    const Vector2D size{ (float)rhs->textureHeader->textureWidth, (float)rhs->textureHeader->textureHeight };

    Widget::SetWidgetSize(size);

    colorBackground = rhs->colorBackground;
    colorBrush = rhs->colorBrush;

    textureHeader = new TextureHeader();

    *textureHeader = *rhs->textureHeader;

    imageData = new uint[(size_t)(size.x * size.y)];

    texture = Texture::GetTexture(textureHeader, imageData);

    image = new ImageWidget(size, texture);

    Clear();

    AppendNewSubnode(image);
}


Canvas::~Canvas()
{
    SAFE_DELETE_ARRAY(imageData);
    SAFE_DELETE(textureHeader);
    delete image;
    texture->Release();
}


void Canvas::EndPaint()
{
    texture->UpdateRect(Rect2D(0, 0, (int)GetWidgetSize().x, (int)GetWidgetSize().y), 0, imageData);
}


void Canvas::SetColorBackground(const ColorRGBA &color)
{
    colorBackground = color;
}


void Canvas::SetColorBrush(const ColorRGBA &color)
{
    colorBrush = color;
}


void Canvas::DrawPoint(float x, float y)
{
    SetPoint(x, y, M::ColorToUINT(colorBrush));
}


bool Canvas::SetPoint(float x, float y, uint colorUINT)
{
    float width = (float)GetWidgetSize().x;
    float height = (float)GetWidgetSize().y;
    if(x >= 0 && x < width && y >= 0 && y < height)
    {
        uint pointer = (uint)(x + 0.5f) + (uint)((height - (uint)(y + 0.5f) - 1) * width);

        imageData[pointer] = colorUINT;
        return true;
    }
    return false;
}


void Canvas::Clear()
{
    uint colorUINT = M::ColorToUINT(colorBackground);

    int width = (int)GetWidgetSize().x;
    int height = (int)GetWidgetSize().y;

    for(int i = 0; i < (int)(width * height); i++)
    {
        imageData[i] = colorUINT;
    }
}


void Canvas::CopyFrom(const Canvas *rhs)
{
    float width = GetWidgetSize().x;
    float height = GetWidgetSize().y;

    std::memcpy(imageData, rhs->imageData, width * height * sizeof(*imageData));

    EndPaint();
}


void Canvas::DrawRectangle(const Point2D &leftTop, const Point2D &size)
{
    DrawLine(leftTop.x, leftTop.y, leftTop.x + size.x, leftTop.y);
    DrawLine(leftTop.x + size.x, leftTop.y, leftTop.x + size.x, leftTop.y + size.y);
    DrawLine(leftTop.x, leftTop.y + size.y, leftTop.x + size.x, leftTop.y + size.y);
    DrawLine(leftTop.x, leftTop.y, leftTop.x, leftTop.y + size.y);
}


void Canvas::DrawRectangle(float x, float y, float eWidth, float eHeight)
{
    DrawLine(x, y, x + eWidth, y);
    DrawLine(x + eWidth, y, x + eWidth, y + eHeight);
    DrawLine(x, y + eHeight, x + eWidth, y + eHeight);
    DrawLine(x, y, x, y + eHeight);
}


void Canvas::DrawCircle(float x0, float y0, float radius)
{
    int numPoints = (int)radius;
    if(radius < 5)
    {
        numPoints *= 2;
    }
    float step = Math::two_pi / (float)(numPoints);

    float xPrev = x0 + radius * Cos(0.0F) + 0.5F;
    float yPrev = y0 + radius * Sin(0.0F) + 0.5F;

    for(int i = 1; i < numPoints; i++)
    {
        float x = x0 + radius * Cos((float)(i) * step) + 0.5F;
        float y = y0 + radius * Sin((float)(i) * step) + 0.5F;
        DrawLine(xPrev, yPrev, x, y);
        xPrev = x;
        yPrev = y;
    }

    DrawLine(xPrev, yPrev, x0 + radius * Cos(0.0F) + 0.5F, y0 + radius * Sin(0.0F) + 0.5F);
}


void Canvas::FillCircle(float x, float y, float radius)
{
    DrawCircle(x, y, radius);
    FillRegion(x, y, colorBrush);
}


void Canvas::FillRegion(float x, float y, const ColorRGBA &color)
{
    uint colorUINT = M::ColorToUINT(color);

    FillFourPoints(x, y, colorUINT);
}


void Canvas::FillRegion(float x0, float y0, float eWidth, float eHeight)
{
    for(int y = (int)y0; y <= (int)(y0 + eHeight); y++)
    {
        DrawLine(x0, (float)y, x0 + eWidth, (float)y);
    }
}


bool Canvas::FillFourPoints(float x, float y, uint colorUINT)
{

    float width = GetWidgetSize().x;
    float height = GetWidgetSize().y;

    if(x < 0 || x >= width || y < 0 || y >= height)
    {
        return false;
    }

    if(GetColorUINT((int)x, (int)y) == colorUINT)
    {
        return false;
    }

    if(!SetPoint(x, y, colorUINT))
    {
        return false;
    }

    FillFourPoints(x - 1, y, colorUINT);
    FillFourPoints(x, y - 1, colorUINT);
    FillFourPoints(x + 1, y, colorUINT);
    FillFourPoints(x, y + 1, colorUINT);

    return true;
}


uint Canvas::GetColorUINT(int x, int y)
{
    int width = (int)GetWidgetSize().x;
    int height = (int)GetWidgetSize().y;
    return imageData[(uint)(x + (height - y - 1) * width)];
}


void Canvas::DrawLine(const Point2D &p0, const Point2D &p1)
{
    DrawLine(p0.x, p0.y, p1.x, p1.y);
}


void Canvas::DrawLine(float x0, float y0, float x1, float y1)
{
    if(fabs(x1 - x0) < 0.5f && fabs(y1 - y0) < 0.5f)
    {
        x0++;
    }
    int x = (int)x0;
    int y = (int)y0;
    int dx = (int)Fabs((float)(x1 - x0));
    int dy = (int)Fabs((float)(y1 - y0));
    int s1 = (int)Fsgn((float)(x1 - x0));
    int s2 = (int)Fsgn((float)(y1 - y0));
    int temp = 0;
    bool exchange = false;
    if(dy > dx)
    {
        temp = dx;
        dx = dy;
        dy = temp;
        exchange = true;
    }
    else
    {
        exchange = false;
    }
    int e = 2 * dy - dx;
    for(int i = 0; i <= dx; i++)
    {
        DrawPoint((float)x, (float)y);
        while(e >= 0 && dx != 0)
        {
            if(exchange)
            {
                x += s1;
            }
            else
            {
                y += s2;
            }
            e -= 2 * dx;
        }
        if(exchange)
        {
            y += s2;
        }
        else
        {
            x += s1;
        }
        e = e + 2 * dy;
    }
}


void Canvas::DrawPolyLine(const Point2D *points, int numPoints)
{
    for(int i = 0; i < numPoints - 1; i++)
    {
        DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }
    DrawLine(points[0].x, points[0].y, points[numPoints - 1].x, points[numPoints - 1].y);
}


void Canvas::ClearAndFillRegion(const Point2D *points, int numPoints, const ColorRGBA &colorFill, const ColorRGBA &colorBorder, const Point2D &pointFill)
{
    ColorRGBA colBackground = colorBackground;
    ColorRGBA colBrush = colorBrush;

    SetColorBackground(ColorRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    Clear();
    SetColorBrush(colorFill);
    DrawPolyLine(points, numPoints);
    FillRegion(pointFill.x, pointFill.y, colorBrush);
    SetColorBrush(colorBorder);
    DrawPolyLine(points, numPoints);

    SetColorBackground(colBackground);
    SetColorBrush(colBrush);
}
