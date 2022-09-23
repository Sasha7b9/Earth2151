// 2022/04/29 13:57:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Frame.h"

using namespace std;


void Application::Init()
{
    Log::Init();
}


void Application::Update()
{

}


void Frame::OnClose()
{
    Log::DeInit();
}
