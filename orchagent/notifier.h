#pragma once

#include "orch.h"

class Notifier : public Executor {
public:
    Notifier(swss::NotificationConsumer *select, Orch *orch, const std::string &name)
        : Executor(select, orch, name)
    {
    }

    swss::NotificationConsumer *getNotificationConsumer() const
    {
        return static_cast<swss::NotificationConsumer *>(getSelectable());
    }

    void execute()
    {
        // pops notification first, then high priority notification will not block low priority event
        swss::NotificationConsumer *consumer = getNotificationConsumer();
        consumer->saveToSync();
        m_orch->doTask(*getNotificationConsumer());
    }
    
    void drain()
    {
        swss::NotificationConsumer *consumer = getNotificationConsumer();
        if (!consumer->syncIsEmpty())
        {
            m_orch->doTask(*consumer);
        }
    }

private:
    std::deque<swss::KeyOpFieldsValuesTuple> m_notifications;
};
