// 2025/03/25 12:25:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Unit : public CommandObject
{
    Unit(pchar f, pchar n, ScriptReader *r) : CommandObject(f, n, r) { }
};


struct UTank : public Unit
{
    UTank(pchar f, pchar n, ScriptReader *r) : Unit(f, n, r)  { }
};


struct UnitEx : public Unit
{
    UnitEx(pchar f, pchar n = "", ScriptReader *r = nullptr) : Unit(f, n, r) { }
    void CommandMove(int nX, int nY, int nZ) { }
};


struct UAircraft : public UTank
{
    UAircraft(pchar f, pchar n, ScriptReader *r) : UTank(f, n, r) { }
};


struct UBuilder : public UTank
{
    UBuilder(pchar f, pchar n, ScriptReader *r) : UTank(f, n, r) { }
};


struct UCarrier : public UTank
{
    UCarrier(pchar f, pchar n, ScriptReader *r) : UTank(f, n, r) { }
};


struct UCivil : public Unit
{
    UCivil(pchar f, pchar n, ScriptReader *r) : Unit(f, n, r) { }
};


struct UHarvester : public UTank
{
    UHarvester(pchar f, pchar n, ScriptReader *r) : UTank(f, n, r) { }
};


struct URepairer : public UTank
{
    URepairer(pchar f, pchar n, ScriptReader *r) : UTank(f, n, r) { }
};


struct USapper : public UTank
{
    USapper(pchar f, pchar n, ScriptReader *r) : UTank(f, n, r) { }
};


struct USupplier : public UTank
{
    USupplier(pchar f, pchar n, ScriptReader *r) : UTank(f, n, r) { }
};
