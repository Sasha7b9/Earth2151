// 2022/12/18 11:57:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Files/Model2150.h"
#include "Earth2150/Parameters/Parameters2150.h"
#include "Earth2150/Earth2150Types.h"
#include "Settings.h"

// Путь к файлам этой игры
#define DATA_PATH(x) Earth2151::DataFile(x)


// Путь к файлам оригинальной игры
#define RESOURCE_PATH(x) Earth2151::ResourceFile(x)


namespace Pi
{
    enum : ActionType
    {
        CameraForward  = 'frwd',
        CameraBackward = 'bkwd',
        CameraLeft     = 'left',
        CameraRight    = 'rght',
        CameraUp       = 'jump',
        CameraDown     = 'down'
    };

    namespace TypeModel
    {
        const uint Brick = 'brik';
    }

    enum : LocatorType
    {
        kLocatorSpectator = 'spec'
    };

    enum : MaterialType
    {
        kMaterialSplatter = 'splt'
    };

    class MovementAction : public Action
    {
    private:

        uint        movementFlag;

    public:

        MovementAction(ActionType type, uint flag) : Action(type), movementFlag(flag) {};
        ~MovementAction() {};

        virtual void HandleEngage() override;
        virtual void HandleDisengage() override;
    };


    class Earth2151 : public Global<Earth2151>, public Application
    {
    public:

        Earth2151();
        virtual ~Earth2151() override;

        static bool target_destroed;

        virtual void UnloadWorld() override;

        static String<> DataFile(pchar name)
        {
            return String<>(TheResourceMgr->DataCatalog()->GetRootPath()) + "Earth2151/" + name;
        }

        static String<> ResourceFile(pchar name);

        // Поставить на паузу
        void Pause();

        bool InPaused();

        // Возобновить
        void Resume();

        Link<FilePicker> pickerLandscape;

    private:

        ControllerReg<ModelController> modelControllerReg{ kModel2150, "Model2150"};

        FullCommand<Earth2151>    gizmoCommand{ this, &Earth2151::HandleGizmoCommand, "gizmo", "Dnable/disable objects gizmo." };
        FullCommand<Earth2151>    cameraCommand{ this, &Earth2151::HandleCameraCommand, "camera", "Switching between RTS and spectator cameras." };

        LocatorRegistration     locatorReg{ kLocatorSpectator, "Spectator Camera" };
        MaterialRegistration    materialMarker{ kMaterialSplatter, "material/Splatter" };

        bool in_paused = false;

        static World *CreateWorld(pchar name, void *);

        static void EscapeCallback(void *);

        // Создаёт все модели из каталога meshes
        void CreateAllModels();

        void HandleGizmoCommand(Command *, pchar);
        void HandleCameraCommand(Command *, pchar);

        virtual void ApplicationTask() override;
    };
}


extern Earth2151      *TheEarth2151;
extern Level2150      *TheLevel;
extern Parameters2150 *TheParameter;
