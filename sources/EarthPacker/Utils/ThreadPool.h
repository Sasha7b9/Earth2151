// 2022/10/03 16:25:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace ThreadPool
{
    // ��������� ����� �������. ���� ��������� ������� ����, �������, ���� ��������
    void AppendTask(void *func(void *), void *);

    // ������� ���������� ���� �������
    void WaitComplete();
}
