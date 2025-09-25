// 2025/02/22 22:59:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/HeapBuffer.h"
#include "Utils/StringUtils.h"
#include "Graphics/CanvasTexture.h"
#include "Game/Earth2150Types.h"


namespace Earth2150
{
    namespace Reader
    {
        // Возвращает список всех обнаруженнх файлов [.lnd,.mis] в заданном каталоге
        void ReadListLands(pchar dir, Array<Level2150> &);

        // Читает raw данные из заданных в info файлов в lnd, mis (распакованные)
        bool ReadLand(Level2150 &);

        bool ReadTex(FileTex2150 &);
    }

    namespace Unzipper
    {
        // Распаковать все файлы WD по указанному пути
        void UnzipAllWD(pchar path);

        // Разархивировать файл
        bool UnzipFile(pchar file_name, HeapBuffer &);
    }
}
