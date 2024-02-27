#pragma once

class AnimUtils
{
public:
    template<typename T>
    static T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if (!Animation) return nullptr;
        
        for (const auto NotifyEvents = Animation->Notifies; auto NotifyEvent : NotifyEvents)
        {
            if (auto AnimNotify = Cast<T>(NotifyEvent.Notify); AnimNotify)
            {
                return AnimNotify;
            }
        }
        return nullptr;
    }
};