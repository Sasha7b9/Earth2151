// 2025/02/02 12:59:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Task
{
public:

    virtual ~Task()
    {
    }

    virtual void Update() = 0;

    bool IsFinished() const
    {
        return is_finished;
    }

protected:

    bool is_finished = false;
};
