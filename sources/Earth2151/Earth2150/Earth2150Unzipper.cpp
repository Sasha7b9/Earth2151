// 2025/02/22 23:32:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Earth2150Tool.h"
#include "Utils/StackBuffer.h"
#include "Earth2150/Packer/ArchiveWD.h"
#include <zlib.h>


namespace Earth2150
{
    namespace Unzipper
    {
        // Распаковать любой бругой файл
        bool UnzipAnother(pchar file_name, HeapBuffer &);
    }
}


bool Earth2150::Unzipper::UnzipFile(pchar file_name, HeapBuffer &data)
{
    if (Text::ExtractExtension(file_name) == "wd")
    {
//        return UnzipAnother(file_name, data);
    }

    return UnzipAnother(file_name, data);
}


bool Earth2150::Unzipper::UnzipAnother(pchar file_name, HeapBuffer &data)
{
    data.ResetWrite();

    File file_in;

    uint have = 0;

    if (file_in.OpenFile(file_name, kFileReadOnly) != kFileOkay)
    {
        return false;
    }

    uint64 file_size = file_in.GetFileSize();

    uint8 first_bytes[4];

    file_in.ReadFile(first_bytes, 4);

    file_in.SetFilePosition(0);

    if (first_bytes[0] == 0x78 && first_bytes[1] == 0x9c && first_bytes[2] == 0xFB && first_bytes[3] == 0xBF)   // Файл зашифрован
    {
        HeapBuffer buf_in{ 10 };
        buf_in.IncreaseSize(file_size);
        file_in.ReadFile(buf_in.data, file_size);
        buf_in.pointer = file_size;
        buf_in.ResetRead();

        z_stream strm;

#define CHUNK 1

        uint8 in[CHUNK];
        uint8 out[CHUNK];

        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;

        int ret = inflateInit(&strm);

        if (ret == Z_OK)
        {
            int bytes_left = (int)file_size;

            while (bytes_left)
            {
                do
                {
                    int bytes_read = (bytes_left >= CHUNK) ? CHUNK : bytes_left;

                    buf_in.ReadData(in, bytes_read);

                    strm.avail_in = bytes_read;

                    bytes_left -= bytes_read;

                    if (strm.avail_in == 0)
                    {
                        break;
                    }

                    strm.next_in = in;

                    do
                    {
                        strm.avail_out = CHUNK;
                        strm.next_out = out;
                        ret = inflate(&strm, Z_NO_FLUSH);

                        switch (ret)
                        {
                        case Z_NEED_DICT:
                            ret = Z_DATA_ERROR;     /* and fall through */
                            (void)inflateEnd(&strm);
                            return false;
                        case Z_DATA_ERROR:
                        case Z_MEM_ERROR:
                            (void)inflateEnd(&strm);
                            return false;
                        }
                        have = CHUNK - strm.avail_out;

                        data.WriteData(out, have);

                    } while (strm.avail_out == 0);

                } while (ret != Z_STREAM_END);

                inflateReset(&strm);
            }

            return true;
        }
    }
    else
    {
        file_in.SetFilePosition(0);

        uint8 *buffer = new uint8[file_size];

        file_in.ReadFile(buffer, file_size);

        data.IncreaseSize(file_size);

        data.WriteData(buffer, file_size);

        delete []buffer;

        return true;
    }

    return false;
}


void Earth2150::Unzipper::UnzipAllWD(pchar path)
{
    {                                                   // Проверяем, требуется ли распаковка
        Map<FileReference> file_map;

        char prev_path[256];

        std::strcpy(prev_path, path);

        char *pointer = prev_path + std::strlen(path) - 2;

        while (!Text::IsSeparator(*pointer))
        {
            pointer--;
        }

        *pointer = '\0';

        FileMgr::BuildFileMap(prev_path, &file_map);

        int counter = 0;

        FileReference *element = file_map.GetFirstMapElement();

        while (element)
        {
            counter++;

            element = element->GetNextMapElement();
        }

        if (counter > 9)                    // Файлы уже разархивированы
        {
            return;
        }
    }

    {
        Map<FileReference> file_map;

        FileMgr::BuildFileMap(path, &file_map);

        if (file_map.Empty())
        {
            LOG_ERROR("Оriginal files *wd not found");
        }
        else
        {
            FileReference *element = file_map.GetFirstMapElement();

            while (element)
            {
                String<> full_name(String<>(path) + element->fileName);

                ArchiveWD archive(full_name.c_str());

                archive.ReadContent();

                pchar pointer = full_name.c_str() + String<>(full_name).GetStringLength();

                while (!Text::IsSeparator(*pointer))
                {
                    pointer--;
                }
                pointer--;
                while (!Text::IsSeparator(*pointer))
                {
                    pointer--;
                }
                *((char *)(pointer + 1)) = '\0';

                archive.Unpack(full_name);

                element = element->GetNextMapElement();
            }
        }
    }
}
