// 2022/10/01 09:05:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Canvas.h"
#include "Resources/Archive.h"
#include "Models/Model.h"
#include "Resources/Description.h"


class PageInfo : public Canvas
{
public:

    static const int PIXELS_IN_LINE = 18;

    PageInfo(wxWindow *parent);

    static PageInfo *self;

    void SetDescriptionFile(const Description *);

private:

    const Description *description;

    void OnPaintEvent(wxPaintEvent &);
    void OnSizeEvent(wxSizeEvent &);
    void OnScrollTrackEvent(wxScrollWinEvent &);
    void OnScrollLineUpEvent(wxScrollWinEvent &);
    void OnScrollLineDownEvent(wxScrollWinEvent &);
    void OnMouseWheel(wxMouseEvent &);

    void DrawDescription() const;

    int DrawCell(int x, int y, int width, const wxString &);

    struct ScrollBar
    {
        // Вызывается при установке нового DescriptionFile
        void Reset();

        // Возвращает полную область скроллбара
        int GetRange();

        // Возвращает видимую область скроллбара
        int GetThumb();

        int SetPosition(int pos);
        int GetPosition() const { return position; }

        void MoveOnLines(int num_lines);

        // Возвращает первую выводимую строку текста
        int GetFirstLine() const;

        PageInfo *keeper = nullptr;

    private:
        int position = 0;

    } scroll_bar;
};
