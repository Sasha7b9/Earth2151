// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Interface/Panels/PanelWindow.h"
#include "Interface/Controls/Button.h"
#include "Input/Mouse.h"


class PanelMapSprocket;


enum
{
    kMutatorPanelMap = 'map_'
};


class PanelMap : public PanelWindow, public Global<PanelMap>
{

public:
    PanelMap();
    virtual ~PanelMap();

    static void Destroy();

    void PreprocessWidget() override;
    void HandleHideShow(Widget *button, const WidgetEventData *data);
    void HandleObserver(MouseObservable *, uint);

private:

    PanelMapSprocket *mutator = nullptr;

    Observer<PanelMap, MouseObservable> observer;
};


class PanelMapSprocket : public Sprocket
{
    friend class SprocketReg <PanelMapSprocket>;

public:
    PanelMapSprocket(PanelMap *panel);
    ~PanelMapSprocket()
    {
        SAFE_DELETE(buffer);
    }

    virtual void MoveSprocket() override;
    virtual void ResetSprocket() override;

    Point2D CoordWorldToMap(const Point3D &coord);
    Point3D CoordMapToWorld(const Point2D &coord);

private:

    Canvas   *buffer = nullptr;     // Здесь хранится карта
    PanelMap *panel = nullptr;      // А сюда мы копируем карту и рисуем поверх зону обзора
    float     scale;
    Point2D   leftTop;

    virtual Sprocket *Replicate() const override;
};


extern PanelMap *ThePanelMap;
