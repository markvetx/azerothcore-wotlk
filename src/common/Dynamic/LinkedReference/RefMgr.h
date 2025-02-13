/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it and/or modify it under version 2 of the License, or (at your option), any later version.
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

#ifndef _REFMANAGER_H
#define _REFMANAGER_H
//=====================================================

#include "Dynamic/LinkedList.h"
#include "Dynamic/LinkedReference/Reference.h"

template <class TO, class FROM> class RefMgr : public LinkedListHead
{
public:
    typedef LinkedListHead::Iterator< Reference<TO, FROM>> iterator;
    RefMgr() = default;
    virtual ~RefMgr() { clearReferences(); }

    Reference<TO, FROM>* getFirst() { return ((Reference<TO, FROM>*) LinkedListHead::getFirst()); }
    [[nodiscard]] Reference<TO, FROM> const* getFirst() const { return ((Reference<TO, FROM> const*) LinkedListHead::getFirst()); }
    Reference<TO, FROM>* getLast() { return ((Reference<TO, FROM>*) LinkedListHead::getLast()); }
    [[nodiscard]] Reference<TO, FROM> const* getLast() const { return ((Reference<TO, FROM> const*) LinkedListHead::getLast()); }

    iterator begin() { return iterator(getFirst()); }
    iterator end() { return iterator(nullptr); }
    iterator rbegin() { return iterator(getLast()); }
    iterator rend() { return iterator(nullptr); }

    void clearReferences()
    {
        LinkedListElement* ref;
        while ((ref = getFirst()) != nullptr)
        {
            ((Reference<TO, FROM>*) ref)->invalidate();
            ref->delink();                              // the delink might be already done by invalidate(), but doing it here again does not hurt and insures an empty list
        }
    }
};

//=====================================================
#endif
