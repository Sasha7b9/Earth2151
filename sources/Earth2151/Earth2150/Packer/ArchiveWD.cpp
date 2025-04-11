// 2022/09/23 20:23:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Packer/ArchiveWD.h"
#include "Earth2150/Packer/MemoryInputStream.h"
#include "Earth2150/Packer/ResourceFactory.h"
#include "Earth2150/Packer/MemoryBufferWD.h"
#include "Utils/StringUtils.h"
#include <zlib.h>


ArchiveWD::ArchiveWD(pchar full_path) : file_name(full_path)
{
    if (Text::ExtractExtension(full_path) != "wd")
    {
        return;
    }

    File file;
    if (file.OpenFile(file_name, kFileReadOnly) != kFileOkay)
    {
        return;
    }

    if (!IsValidWDFile(file))
    {
        return;
    }

    uint64 file_size = file.GetFileSize();

    uint dirLn = 0;

    file.SetFilePosition(file_size - 4);
    file.ReadFile(&dirLn, 4);

    //wxMemoryBuffer dirData = file.ReadBytes(file.GetSize() - dirLn, dirLn);
    MemoryBufferWD dirData(dirLn);
    file.SetFilePosition(file_size - dirLn);
    file.ReadFile(dirData.GetData(), dirLn);

    file.CloseFile();

    unsigned long dest_len = dirLn * 10;
    MemoryBufferWD dir(dest_len);

    int result = uncompress((Bytef *)dir.GetData(), &dest_len, (const Bytef *)dirData.GetData(), dirData.GetSize());

    int counter = 0;

    if (result == Z_OK)
    {
        MemoryInputStream stream((uint8 *)dir.GetData() + 10, dest_len - 10);
        while (!stream.Eof())   
        {
            counter++;

            ResourceWD resource = ResourceFactory::Create(stream);
            resources.emplace_back(resource);
        }
    }
    else
    {
        LOG_ERROR("Can not uncompress file %s", full_path);
    }
}


bool ArchiveWD::IsCorrectFile(const String<> &path)
{
    return Text::ExtractExtension(path) == "wd";
}


bool ArchiveWD::ReadContent()
{
    File file;
    if (file.OpenFile(file_name, kFileReadOnly) != kFileOkay)
    {
        return false;
    }

    if (!IsValidWDFile(file))
    {
        return false;
    }

    for (ResourceWD &resource : resources)
    {
        file.SetFilePosition(resource.info.offset);

        MemoryBufferWD buffer(resource.info.length);

        file.ReadFile(buffer.GetData(), buffer.GetSize());

        resource.data.Set(buffer.GetData(), buffer.GetSize());

        if (resource.info.decompressedLength != resource.info.length)
        {
            unsigned long dest_len = 10 * 1024 * 1204;
            MemoryBufferWD buf(dest_len);

            int result = uncompress((Bytef *)buf.GetData(), &dest_len, (const Bytef *)buffer.GetData(), buffer.GetSize());

            if ((int)dest_len != resource.info.decompressedLength)
            {
                LOG_WRITE("Тhe size does not match the required one");
            }

            if (result == Z_OK)
            {
                resource.data.Set(buf.GetData(), dest_len);
            }
            else
            {
                LOG_ERROR("Bad decompress");
            }
        }
    }

    file.CloseFile();

    return true;
}


void ArchiveWD::Unpack(const String<> &path)
{
    for (auto desc : resources)
    {
        if (desc.file_name.GetStringLength() == 0)
        {
            continue;
        }

        if (desc.data.GetSize())
        {
            String<> path_resource{ desc.file_name };

            if (Text::ExtractExtension(path_resource) != "")
            {
                String<> full_name = path + path_resource;          // Полное имя файла, который должен быть записан

                {                                                   // Создаём директорий для ресурса
                    String<> directory_name = full_name;

                    pchar pointer = directory_name.c_str() + directory_name.GetStringLength();

                    while (!Text::IsSeparator(*pointer))
                    {
                        pointer--;
                    }

                    *((char *)pointer) = '\0';

                    TheFileMgr->CreateDirectory(directory_name);
                }

                File file_resource;

                if (SU::EndsWith(full_name.c_str(), ".tex") || SU::EndsWith(full_name.c_str(), ".TEX"))
                {
                    full_name.AppendString("_", 1);
                }

                file_resource.OpenFile(full_name, kFileCreate);

                file_resource.WriteFile(desc.data.GetData(), desc.data.GetSize());

                file_resource.CloseFile();

                // \todo
//                if (desc.unknown_data.size())   
//                {
//                    wxFile file_unknown;
//
//                    file_unknown.Create(file.GetPath() + wxFileName::GetPathSeparator() + desc.file_name + ".unknownData", true);
//
//                    for (uint i = 0; i < desc.unknown_data.size(); i++)
//                    {
//                        file_unknown.Write(&desc.unknown_data[i], 1);
//                    }
//                }
//
//                if (typeid(desc) == typeid(TranslatableResource))
//                {
//                    wxFile file_trans;
//
//                    file_trans.Create(file.GetPath() + wxFileName::GetPathSeparator() + desc.file_name + ".translationId", true);
//
//                    file_trans.Write(((TranslatableResource *)&desc)->translationID); //-V717
//                }
            }
        }
    }
}


bool ArchiveWD::IsValidWDFile(File & /*stream*/)
{
//    wxZlibInputStream zstream(stream);
//
//    uint8 buffer[8];
//    static const uint8 template_buffer[8] = { 0xff, 0xa1, 0xd0, '1', 'W', 'D', 0x00, 0x02 };
//
//    zstream.Read(buffer, 8);
//
//    return std::memcmp(buffer, template_buffer, 8) == 0;

    return true;
}


void ArchiveWD::GetDescription(DescriptionArchive * /*description*/)
{
//    description->file_name = wxString::Format("File : %s", file_name.GetFullPath().c_str());
//
//    if (file_name.GetExt() == "wd")
//    {
//        int counter = 1;
//
//        description->count_resources = resources.size();
//
//        for (const Resource &resource : resources)
//        {
//            if (resource.file_name.empty())
//            {
//                description->AppendInfo({ counter++, std::string("Empty name resource") });
//
//                continue;
//            }
//
//            if (resource.info.length)
//            {
//                description->AppendInfo({ counter++, resource.file_name.c_str(), resource.info.length, resource.info.decompressedLength });
//            }
//            else
//            {
//                description->AppendInfo({ counter++, "Empty resource" });
//            }
//        }
//    }
}


bool DescriptionArchive::IsValid() const
{
    return false;
}


void DescriptionArchive::AppendInfo(const InfoArchive &info)
{
    push_back(info);
}


int DescriptionArchive::Size() const
{
    return (int)size();
}
