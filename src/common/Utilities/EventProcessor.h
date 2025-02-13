/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it and/or modify it under version 2 of the License, or (at your option), any later version.
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

#ifndef __EVENTPROCESSOR_H
#define __EVENTPROCESSOR_H

#include "Define.h"
#include "Duration.h"

#include <map>

class EventProcessor;

// Note. All times are in milliseconds here.
class BasicEvent
{

    friend class EventProcessor;

    enum class AbortState : uint8
    {
        STATE_RUNNING,
        STATE_ABORT_SCHEDULED,
        STATE_ABORTED
        };

    public:
        BasicEvent()
            : m_abortState(AbortState::STATE_RUNNING), m_addTime(0), m_execTime(0) { }

        virtual ~BasicEvent() { } // override destructor to perform some actions on event removal

        // this method executes when the event is triggered
        // return false if event does not want to be deleted
        // e_time is execution time, p_time is update interval
        [[nodiscard]] virtual bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) { return true; }

        [[nodiscard]] virtual bool IsDeletable() const { return true; }   // this event can be safely deleted

        virtual void Abort(uint64 /*e_time*/) { }           // this method executes when the event is aborted

        // Aborts the event at the next update tick
        void ScheduleAbort();

    private:
        void SetAborted();
        bool IsRunning() const { return (m_abortState == AbortState::STATE_RUNNING); }
        bool IsAbortScheduled() const { return (m_abortState == AbortState::STATE_ABORT_SCHEDULED); }
        bool IsAborted() const { return (m_abortState == AbortState::STATE_ABORTED); }

        AbortState m_abortState;                            // set by externals when the event is aborted, aborted events don't execute

        // these can be used for time offset control
        uint64 m_addTime;                                   // time when the event was added to queue, filled by event handler
        uint64 m_execTime;                                  // planned time of next execution, filled by event handler
};

typedef std::multimap<uint64, BasicEvent*> EventList;

class EventProcessor
{
    public:
        EventProcessor() : m_time(0) { }
        ~EventProcessor();

        void Update(uint32 p_time);
        void KillAllEvents(bool force);
        void AddEvent(BasicEvent* Event, uint64 e_time, bool set_addtime = true);
        void ModifyEventTime(BasicEvent* event, Milliseconds newTime);
        [[nodiscard]] uint64 CalculateTime(uint64 t_offset) const;

        //calculates next queue tick time
        [[nodiscard]] uint64 CalculateQueueTime(uint64 delay) const;

    protected:
        uint64 m_time;
        EventList m_events;
        bool m_aborting;
};

#endif
