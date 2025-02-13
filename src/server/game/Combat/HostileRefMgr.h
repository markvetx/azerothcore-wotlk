/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it and/or modify it under version 2 of the License, or (at your option), any later version.
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

#ifndef _HOSTILEREFMANAGER
#define _HOSTILEREFMANAGER

#include "Common.h"
#include "RefMgr.h"

class Unit;
class ThreatMgr;
class HostileReference;
class SpellInfo;

//=================================================

class HostileRefMgr : public RefMgr<Unit, ThreatMgr>
{
private:
    Unit* iOwner;
public:
    explicit HostileRefMgr(Unit* owner) { iOwner = owner; }
    ~HostileRefMgr() override;

    Unit* GetOwner() { return iOwner; }

    // send threat to all my hateres for the victim
    // The victim is hated than by them as well
    // use for buffs and healing threat functionality
    void threatAssist(Unit* victim, float baseThreat, SpellInfo const* threatSpell = nullptr);

    void addTempThreat(float threat, bool apply);

    void addThreatPercent(int32 percent);

    // The references are not needed anymore
    // tell the source to remove them from the list and free the mem
    void deleteReferences(bool removeFromMap = false);

    // Remove specific faction references
    void deleteReferencesForFaction(uint32 faction);

    // pussywizard: for combat bugs
    void deleteReferencesOutOfRange(float range);

    HostileReference* getFirst() { return ((HostileReference*) RefMgr<Unit, ThreatMgr>::getFirst()); }

    void updateThreatTables();

    void setOnlineOfflineState(bool isOnline);

    // set state for one reference, defined by Unit
    void setOnlineOfflineState(Unit* creature, bool isOnline);

    // delete one reference, defined by Unit
    void deleteReference(Unit* creature);

    void UpdateVisibility(bool checkThreat);
};
//=================================================
#endif
