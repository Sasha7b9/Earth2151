// 2025/01/29 20:14:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/Terrain.h"


Terrain::Terrain(pchar name_node) : Node()
{
    SetNodeName(name_node);

    TerrainMaterial material{ {0, 0, 255, 128}, {0, 255, 0, 128} };
    TerrainBlock *block = new TerrainBlock({ 30, 30, 30 }, 1.0f, &material);
    block->SetWorldPosition({ 0, 0, 10.0f });
    TerrainBlockObject *obj = block->GetObject();
    obj->SetVoxelScale(1.0f);
    const Integer2D size_map{ 100, 100 };
    const int blockSizeX = size_map.x >> 4;
    const int blockSizeY = size_map.y >> 4;
    int blockSizeZ = 100;
    block->SetBlockSize({ blockSizeX, blockSizeY, blockSizeZ });

    Color4U *field = new Color4U[blockSizeX * blockSizeY];

    Color4U *pixel = field;

    for (int i = 0; i < blockSizeX; i++)
    {
        for (int j = 0; j < blockSizeY; j++)
        {
            *pixel = Color4U(std::rand(), std::rand(), std::rand(), std::rand());
            pixel++;
        }
    }

    block->SetBlockToHeightField((uint8 *)field, 100, 100);

    delete[] field;

    const Integer3D &size0 = block->GetBlockSize();
    Integer3D size1((size0.x + 1) & ~1, (size0.y + 1) & ~1, (size0.z + 1) & ~1);
    Integer3D size2((size0.x + 3) & ~3, (size0.y + 3) & ~3, (size0.z + 3) & ~3);

    for (machine k = 0; k < size2.z; k += 4)
    {
        for (machine j = 0; j < size2.y; j += 4)
        {
            for (machine i = 0; i < size2.x; i += 4)
            {
                TerrainGeometry *geometry = new TerrainLevelGeometry(block, Integer3D(i, j, k), 2);
                TerrainGeometryObject *object = geometry->GetObject();
                object->BuildTerrain(geometry);
                block->AddTerrainGeometry(geometry);
            }
        }
    }

    for (machine k = 0; k < size1.z; k += 2)
    {
        for (machine j = 0; j < size1.y; j += 2)
        {
            for (machine i = 0; i < size1.x; i += 2)
            {
                TerrainGeometry *geometry = new TerrainLevelGeometry(block, Integer3D(i, j, k), 1);
                TerrainGeometryObject *object = geometry->GetObject();
                object->BuildTerrain(geometry);
                block->AddTerrainGeometry(geometry);
            }
        }
    }

    for (machine k = 0; k < size0.z; k++)
    {
        for (machine j = 0; j < size0.y; j++)
        {
            for (machine i = 0; i < size0.x; i++)
            {
                TerrainGeometry *geometry = new TerrainGeometry(block, Integer3D(i, j, k));
                TerrainGeometryObject *object = geometry->GetObject();
                object->BuildTerrain(geometry);
                block->AddTerrainGeometry(geometry);
            }
        }
    }

    Node::AppendNewSubnode(block);
}
