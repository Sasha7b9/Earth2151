// 2025/03/25 12:23:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Scripting/Objects/SCommandObject.h"



S::CommandObject::CommandObject(pchar file_name, pchar name, ScriptReader *_reader) :
    SObject(file_name, name, _reader)
{
}
