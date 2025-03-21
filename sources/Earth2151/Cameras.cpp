// 2025/02/14 22:55:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "GameWorld.h"
#include "Interface/GUI.h"
#include "Interface/Widgets/GameCursor.h"
#include "Input/Input.h"
#include "Input/Mouse.h"
#include "Utils/Math.h"
#include "Objects/World/Landscape.h"
#include "Interface/GUI.h"


CameraRTS *TheCameraRTS = nullptr;
CameraSpecatator *TheCameraSpectator = nullptr;


void CameraRTS::Create(LocatorMarker *)
{
    new CameraRTS();
}


CameraRTS::CameraRTS()
    : FrustumCamera(2.0f, 1.0f), Global(TheCameraRTS)
{
    FrustumCameraObject *object = GetObject();
    object->SetFrustumFlags(kFrustumInfinite);
//    object->SetFarDepth(1e7);                 // \todo
    SetObject(object);
    Node::UpdateNode();
}


void CameraRTS::ZoomIn()
{
    SetDistance(distance * 0.95f);
}

void CameraRTS::ZoomOut()
{
    SetDistance(distance * 1.05f);
}

void CameraRTS::TiltAngleIncrease()
{
    SetTiltAngle(angleTilt + STEP_TILT_ANGLE);
}

void CameraRTS::TiltAngleDecrease()
{
    SetTiltAngle(angleTilt - STEP_TILT_ANGLE);
}

void CameraRTS::MoveCamera()
{
    static bool is_first = true;

    if (is_first)
    {
        is_first = false;

        pointFocus = { Landscape::GetSize().x / 2.0f, Landscape::GetSize().y / 2.0f, 0.0f };

        distance = 100.0f;
    }
    else
    {
        float dt = TheTimeMgr->GetFloatDeltaTime() * 0.005f;
        float deltaFwd = 0.0f;                  // Смещение по линии вперёд/назад
        float deltaRight = 0.0f;                // Смещение по линии влево/вправо

        const float sensitivity = 300.0f;

        float deltaX = -sensitivity * TheInputMgr->GetMouseDeltaX();
        float deltaY = -sensitivity * TheInputMgr->GetMouseDeltaY();

        if (Input::keyCtlr && deltaX != 0.0f && deltaY != 0.0f && GUI::trackedWidget)
        {
            Point2D delta = TheGameCursor->position;
            delta -= GUI::mouse_pos;

            Point2D pos = GUI::widget_pos + delta;
            GUI::trackedWidget->SetWidgetPosition(pos);
            GUI::trackedWidget->InvalidateWidget();
        }

        speed.x += deltaX / 20.0f;
        speed.y += deltaY / 20.0f;

        if (TheMouse->RightIsPressed() && !TheGUI->UnderMouse())
        {
            if (TheMouse->LeftIsPressed())
            {
                distance += (deltaY * dt * distance / 10.0f);
            }
            else
            {
                azimuth -= deltaX * dt / 10.0f;
                angleTilt -= deltaY * dt / 10.0f;
            }
        }
        else if (TheMouse->MiddleIsPressed())
        {
            deltaFwd -= deltaY * dt / 5.0f;
            deltaRight += deltaX * dt / 5.0f;
        }
        else if (std::fabs(deltaX) > std::numeric_limits<float>::epsilon() ||
            std::fabs(deltaY) > std::numeric_limits<float>::epsilon())
        {
            TheGameWorld->ChangeCursorPosition(deltaX, deltaY);
        }

        Point2D cursorPos = TheGameCursor->position;

        float autoScrollThreshold = 0.005f;

        float displayWidth = (float)TheEngine->GetOutputResolution().x;
        float displayHeight = (float)TheEngine->GetOutputResolution().y;

        if ((cursorPos.x > autoScrollThreshold * displayWidth) && (cursorPos.x < (1.0f - autoScrollThreshold) * displayWidth))
        {
            speed.x = 0.0f;
        }

        deltaRight += speed.x * dt;

        if ((cursorPos.y > autoScrollThreshold * displayHeight) && (cursorPos.y < (1.0f - autoScrollThreshold) * displayHeight))
        {
            speed.y = 0.0f;
        }

        if (TheGUI->UnderMouse())
        {
            return;
        }

        deltaFwd -= speed.y * dt;

        if ((movementFlags & (uint)E::MoveForward))
        {
            deltaFwd += 3.0f * dt;
        }
        if ((movementFlags & (uint)E::MoveBackward))
        {
            deltaFwd -= 3.0f * dt;
        }
        if ((movementFlags & (uint)E::MoveRight))
        {
            deltaRight += 3.0f * dt;
        }
        if ((movementFlags & (uint)E::MoveLeft))
        {
            deltaRight -= 3.0f * dt;
        }
        if ((movementFlags & (uint)E::RotateLeft))
        {
            azimuth -= 1.0f * dt / 3;
        }
        if ((movementFlags & (uint)E::RotateRight))
        {
            azimuth += 1.0f * dt / 3;
        }
        if ((movementFlags & (uint)E::RotateTop))
        {
            angleTilt -= 1.0f * dt / 3;
        }
        if ((movementFlags & (uint)E::RotateDown))
        {
            angleTilt += 1.0f * dt / 3;
        }
        if ((movementFlags & (uint)E::ZoomIn))
        {
            distance += 5.0f * dt;
        }
        if ((movementFlags & (uint)E::ZoomOut))
        {
            distance -= 5.0f * dt;
        }
        if (movementFlags & (uint)E::MoveCenter)
        {
            pointFocus = Point3D(Landscape::GetSize().x / 2.0f, Landscape::GetSize().y / 2.0f, 0.0f);
        }

        distance = M::LimitationFloat(distance, MIN_DISTANCE, MAX_DISTANCE);
        angleTilt = M::LimitationFloat(angleTilt, MIN_TILT_ANGLE, MAX_TILT_ANGLE);

        Vector3D vecMoveXY = GetVectorMoveXY(deltaRight, deltaFwd, azimuth); //-V764
        pointFocus += vecMoveXY;

        Point3D camPosition(0.0f, -distance, 0.0f);

        Transform3D rotateTransform = Transform3D::MakeRotationX(angleTilt);

        Matrix3D orbitMatrix = Matrix3D::MakeRotationZ(azimuth);

        rotateTransform = orbitMatrix * rotateTransform;

        camPosition = rotateTransform * camPosition;
        camPosition += pointFocus;

        SetNodePosition(camPosition);

        LookAtPoint(pointFocus);
    }
}


void CameraRTS::SetPointFocus(const Point3D &point)
{
    Vector3D delta = point - pointFocus;

    pointFocus = point;

    SetNodePosition(GetNodePosition() + delta);

    LookAtPoint(pointFocus);

    InvalidateNode();
}


void CameraRTS::MoveOn(const Vector3D &shift)
{
    SetNodePosition(GetNodePosition() + shift);
    pointFocus += shift;
    LookAtPoint(pointFocus);
}

void CameraRTS::SetTiltAngle(float tiltAngle)
{
    angleTilt = M::LimitationFloat(tiltAngle, MIN_TILT_ANGLE, MAX_TILT_ANGLE);
}

void CameraRTS::SetDistance(float _distance)
{
    distance = M::LimitationFloat(_distance, MIN_DISTANCE, MAX_DISTANCE);
}

Vector3D CameraRTS::GetVectorMoveXY(float deltaFwd, float deltaRight, float anglePlaneXY)
{
    return Matrix3D::MakeRotationZ(anglePlaneXY) * Vector3D(deltaFwd, deltaRight, 0.0f);
}

Ray CameraRTS::GetWorldRayFromPoint(const Point2D& p)
{
    const Rect2D &viewRect = GetObject()->GetViewRect();
    float x = (float)(p.x) / (float)viewRect.GetRectWidth();
    float y = (float)(p.y) / (float)viewRect.GetRectHeight();

    Ray ray;

    CastRay(x, y, &ray);

    const Transform3D &cameraTransform = GetNodeTransform();
    ray.origin = cameraTransform * ray.origin;
    ray.direction = (cameraTransform * ray.direction).Normalize();

    ray.radius = 0.0f;
    ray.tmin = 0.0f;
    ray.tmax = 500.0f;

    return ray;
}

bool CameraRTS::GetIntersectionPlaneZ(const Point2D& p, Point3D *point)
{
    Ray ray = GetWorldRayFromPoint(p);

    Vector3D a(1.0f, 0.0f, 0.0f);
    Vector3D b(0.0f, 0.0f, 0.0f);
    Vector3D c(0.0f, 1.0f, 0.0f);

    Vector3D n = Cross(b - a, c - a);

    Point3D x = ray.origin;
    Point3D y = x + ray.direction;

    Vector3D v = a - x;

    Vector3D w = y - x;

    float d = Dot(n, v);
    float e = Dot(n, w);

    if(std::fabs(e) > std::numeric_limits<float>::epsilon())
    {
        *point = x + w * d / e;
        return true;
    }

    *point = Point3D(0, 0, 0);
    return false;
}


CameraSpecatator::CameraSpecatator(float projection, float aspect, float radius) :
    SpectatorCamera(projection, aspect, radius),
    Global<CameraSpecatator>(TheCameraSpectator)
{
}



void CameraSpecatator::Create(LocatorMarker *marker)
{
    new CameraSpecatator(2.67F, 1.0F, 0.3F);

    if (marker)
    {
        marker->SetNodeName("Locator");

        const Vector3D direction = marker->GetWorldTransform()[0];
        float azimuth = Arctan(direction.y, direction.x);
        float altitude = Arctan(direction.z, Sqrt(direction.x * direction.x + direction.y * direction.y));

        TheCameraSpectator->SetCameraAzimuth(azimuth);
        TheCameraSpectator->SetCameraAltitude(altitude);
        TheCameraSpectator->SetNodePosition(marker->GetWorldPosition());
    }
    else
    {
        TheCameraSpectator->SetNodePosition(Point3D(0.0F, 0.0F, 1.0F));
    }
}
