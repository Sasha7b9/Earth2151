// 2022/09/14 14:35:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Utils/SystemUtils.h"
#include <vector>


#pragma warning(push)
#pragma warning(disable:4191 4312)


namespace System
{
    // Захватывает обратную трассировку стека, проходя вверх по стеку и записывая информацию для каждого кадра.
    static int capture_stack_trace(int frames_to_skip, int max_frames, void **out_frames);

    static std::vector<std::string> get_stack_frames_text(void **frames, int num_frames);
}


void System::DisplayLastError(pchar file, int line)
{
#ifdef WIN32

//#define LMEM_ZEROINIT       0x0040
//typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;

    void* lpMsgBuf = nullptr;
    void* lpDisplayBuf = nullptr;
    DWORD dw = GetLastError();

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char *)&lpMsgBuf,
        0, NULL);

    lpDisplayBuf = (void *)LocalAlloc(LMEM_ZEROINIT, (lstrlenA((char *)lpMsgBuf) + 40) * sizeof(char));

    char buffer[500];
    sprintf(buffer, "%s:%d : Error %d : %s", file, line, (int)dw, (char *)lpMsgBuf);
    if (strlen(buffer))
    {
        buffer[strlen(buffer) - 1] = '\0';
    }

    LOG_ERROR_HI(buffer);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);

#else

    LOG_ERROR_HI("error %d in line %s : %d", errno, file, line);

#endif
}


void System::LogBacktrace()
{
    const int max_frames = 64;
    void *frames[max_frames];
    int num_frames = capture_stack_trace(1, max_frames, &frames[0]);
    LOG_WRITE("Num frames = %d", num_frames);
}


int System::capture_stack_trace(int frames_to_skip, int max_frames, void **out_frames)
{
#ifdef WIN32

    return RtlCaptureStackBackTrace(frames_to_skip + 1, max_frames, out_frames, nullptr);

#else

    return 0;

#endif
}


#pragma warning(pop)
