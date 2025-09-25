// 2025/02/02 17:38:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "AngelScript/ControllerAS.h"
#include "Objects/Task.h"
#include "Graphics/Primitives.h"
#include "Game/Parameters/Models/Abstracts/Entity.h"


// Базовый контроллер для объектов всех типов
class GenericObject : public ControllerAS
{
public:

    void SetTask(Task *_task_)
    {
        task = _task_;
    }

    void SetNodeScale(float scale);

    float CalculateAngle(const Vector3D &, const Vector3D &);

protected:

    GenericObject(pchar name_type) : ControllerAS(name_type) { }

    ~GenericObject();

    Task *task = nullptr;

    virtual void OnEventRebuild() override;

    virtual void PreprocessController() override;

    float GetScale() const
    {
        return scale;
    }

private:

    float scale = 1.0f;

    void AppendGizmo();
};


// Базвоый контроллер для тех, что не передвигаются - здания
class StaticObject : public GenericObject
{
protected:

    StaticObject(pchar name_type) : GenericObject(name_type) { }
};


// Базовый контроллер для перемещающихся объектов
class MovedObject : public GenericObject
{
public:

    // +1, -1 - уменьшение, увеличение скорости
    void MoveForward(float direction);

    Vector3D GetDirectionForward() { return direction; }

    Vector3D GetDirectionUp() { return up; }

    // direction = +1, -1 лево/право
    void _RotateAboutAxis(const Vector3D &, float direction);

    void Rotate(const Vector3D &axis, const Vector3D &dir_on_target);

    // Сориентиовать в данном направлении
    void SetNodeDirection(const Vector3D &dir, const Vector3D &up);

protected:

    MovedObject(pchar name_type) : GenericObject(name_type) { }

    virtual void OnEventRebuild() override;

    virtual void PreprocessController() override;

    virtual void Move() override;

private:

    Vector3D direction = Vector3D::forward;     // \warn Этот вектор всегда должен быть нормализован
    Vector3D up = Vector3D::up;                 // \warn Этот вектор всегда должен быть нормализован

    float speed = 0.0f;
    float acceleration_speed = 1.0f;
    float max_speed = 10.0f;
    float slow_speed = 2.0f;
    float rotate_speed = 90.0f;

    float GetAccelerationSpeed() const { return acceleration_speed; }   // Скорость ускорения
    float GetSlowSpeed() const { return slow_speed; }                   // Скорость замедления
    float GetMaxSpeed() const { return max_speed; }                     // Максимальная скорость движения
    float GetRotateSpeed() const { return rotate_speed; }               // Скорость поворота

    // Скомпенсировать направления модели исходя из установленного в ресурсе
    Vector3D GetCompensatedDirection(const Vector3D &);
};
