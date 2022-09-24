// 2022/09/24 14:56:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Model.h"
#include "Utils/FileInputStream.h"
#include "Models/Collections/MountPoints.h"


Models::Model::Model(const wxString &path) : file_path(path)
{
    FileInputStream stream(path);

    CheckHeader(stream);

    type = stream.ReadUINT();

    model_template = new ModelTemplate(stream);

    stream.ReadBytes(10);

    mount_points = new MountPoints(stream);

    lights = new Lights(stream);

    stream.ReadBytes(64);
    stream.ReadBytes(488);

    unknown_val1 = stream.Read2Bytes();
    unknown_val2 = stream.Read2Bytes();
    unknown_val3 = stream.Read2Bytes();
    unknown_val4 = stream.Read2Bytes();
    unknown_val5 = stream.Read2Bytes();

    if (type != 0)
    {
        LOG_ERROR("Not supported mesh format");
    }


}


void Models::Model::CheckHeader(FileInputStream &stream)
{
    wxMemoryBuffer span = stream.ReadBytes(8);

    uint8 template_buffer[8] = { 0x4d, 0x45, 0x53, 0x48, 0x01, 0x00, 0x00, 0x00 };

    if (std::memcmp(span.GetData(), template_buffer, 8) != 0)
    {
        LOG_ERROR("Not mesh format");
    }
}
