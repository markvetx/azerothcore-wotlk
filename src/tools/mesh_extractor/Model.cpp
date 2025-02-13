/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

#include "Model.h"
#include "MPQMgr.h"
#include "Utils.h"

Model::Model( std::string path ) : IsCollidable(false), IsBad(false)
{
    Stream = MPQHandler->GetFile(Utils::FixModelPath(path));
    if (!Stream)
    {
        IsBad = true;
        return;
    }
    Header.Read(Stream);
    if (Header.OffsetBoundingNormals > 0 && Header.OffsetBoundingVertices > 0 &&
            Header.OffsetBoundingTriangles > 0 && Header.BoundingRadius > 0.0f)
    {
        IsCollidable = true;
        ReadVertices();
        ReadBoundingNormals();
        ReadBoundingTriangles();
    }
}

Model::~Model()
{
    if (Stream)
        fclose(Stream);
}

void Model::ReadVertices()
{
    fseek(Stream, Header.OffsetBoundingVertices, SEEK_SET);
    Vertices.reserve(Header.CountBoundingVertices);
    for (uint32 i = 0; i < Header.CountBoundingVertices; ++i)
    {
        Vertices.push_back(Vector3::Read(Stream));
        if (Constants::ToWoWCoords)
            Vertices[i] = Utils::ToWoWCoords(Vertices[i]);
    }
}

void Model::ReadBoundingTriangles()
{
    fseek(Stream, Header.OffsetBoundingTriangles, SEEK_SET);
    Triangles.reserve(Header.CountBoundingTriangles / 3);
    for (uint32 i = 0; i < Header.CountBoundingTriangles / 3; i++)
    {
        Triangle<uint16> tri;
        tri.Type = Constants::TRIANGLE_TYPE_DOODAD;
        int count = 0;
        count += fread(&tri.V0, sizeof(uint16), 1, Stream);
        count += fread(&tri.V1, sizeof(uint16), 1, Stream);
        count += fread(&tri.V2, sizeof(uint16), 1, Stream);
        if (count != 3)
            printf("Model::ReadBoundingTriangles: Error reading data, expected 3, read %d\n", count);
        Triangles.push_back(tri);
    }
}

void Model::ReadBoundingNormals()
{
    fseek(Stream, Header.OffsetBoundingNormals, SEEK_SET);
    Normals.reserve(Header.CountBoundingNormals);
    for (uint32 i = 0; i < Header.CountBoundingNormals; i++)
        Normals.push_back(Vector3::Read(Stream));
}
