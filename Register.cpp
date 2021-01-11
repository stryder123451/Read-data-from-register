#include <iostream>
#include <Windows.h>
#include <iostream>
#include <fstream>
#pragma comment(lib, "winmm.lib")
using namespace std;

int main(void)
{

    HWND window = FindWindowA(0,("Prince of Persia"));
    DWORD_PTR pid = 0;
    GetWindowThreadProcessId(window, &pid);
	std::cout<<pid<<"\n";
    DWORD address = 0x004D3D98; // address of the instruction after the call
    DebugActiveProcess(pid); // PID of target process
    DWORD dwThreadID = GetWindowThreadProcessId(window, &pid);
    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS | CONTEXT_INTEGER;
    ctx.Dr0 = address;
    ctx.Dr7 = 0x00000001;
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadID);
    SetThreadContext(hThread, &ctx); // hThread with enough permissions
    DEBUG_EVENT dbgEvent;
    DWORD ebx = 0;
    DWORD ecx = 0;
	int gg=0;
    while (!gg)
    {
        if (WaitForDebugEvent(&dbgEvent, INFINITE) == 0)
            break;

        if (dbgEvent.dwDebugEventCode == EXCEPTION_DEBUG_EVENT &&
            dbgEvent.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_SINGLE_STEP)
        {
            if (dbgEvent.u.Exception.ExceptionRecord.ExceptionAddress == (LPVOID)address)
            {
                GetThreadContext(hThread, &ctx);
                ebx = ctx.Ebx; // ebx get
				gg=1; 
				ctx.Dr0 = 0;
				ctx.Dr1 = 0;
				ctx.Dr2 = 0;
				ctx.Dr3 = 0;
				ctx.Dr6 = 0;
				ctx.Dr7 = 0;  
             }              	
				GetThreadContext(hThread, &ctx);
                ctx.Dr7 = 0x00000000;
                SetThreadContext(hThread, &ctx);           
        }
		 ContinueDebugEvent(dbgEvent.dwProcessId, dbgEvent.dwThreadId, DBG_CONTINUE); 			     
	}

	DWORD SkinAdd = ebx+0x7F4;
	std::ofstream out;          // поток для записи
    out.open("add.txt"); // окрываем файл для записи
    if (out.is_open())
    {
        out << SkinAdd << std::endl;
    }*/
   
	int SkinAdd=ebx+0x7f4;
	int s=0;
	int Logic=0;
	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	mciSendString(L"play 4.mp3 wait", NULL, 0, NULL);
	while (true)
	{
	if(GetAsyncKeyState(VK_TAB)==-32767&&SkinAdd>0x7f4)
	{   
		ReadProcessMemory(pHandle,(LPCVOID)SkinAdd,&s,sizeof(s),0);
		if (s==0)
	   {   
        Logic=1; 
		WriteProcessMemory(pHandle,(LPVOID)SkinAdd,&Logic,sizeof(Logic),0);
	   }
	   if (s==1)
	   {  
        Logic=0;
		WriteProcessMemory(pHandle,(LPVOID)SkinAdd,&Logic,sizeof(Logic),0);
	   }
	}

	std::cout<<"Main loop starting...";
    system("CLS");
	}
	

    return 0;
	}