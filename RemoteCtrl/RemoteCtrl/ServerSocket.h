#pragma once
#include "pch.h"
#include "framework.h"


class CServerSocket
{

public:
	static CServerSocket* getInstance()
	{
		if (m_instance == NULL) 
			m_instance = new CServerSocket();
		return m_instance;
	}

	bool InitSocket()
	{
		if (m_sock == -1) return false;
		sockaddr_in serv_addr;
		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(9527);
		if (bind(m_sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
			return false;

		return true;
	}

	bool AcceptClient()
	{
		sockaddr_in cli_addr;
		int len = sizeof(cli_addr);
		m_client = accept(m_sock, (sockaddr*)&cli_addr, &len);
		if (m_client == -1) return false;
		return true;
	}

	int DealCommand()
	{
		if (m_client == -1) return -1;
		char buf[1024] = "";

		while (1) {
			int len = recv(m_client, buf, sizeof(buf), 0);
			if (len <= 0) return -1;
			//TODO:处理命令

		}
	}

private:
	CServerSocket()
	{
		m_client = INVALID_SOCKET;
		if (!InitSockEnv()) {
			MessageBox(NULL, _T("无法初始化套接字环境"), _T("初始化错误"), MB_OK | MB_ICONERROR);
			exit(0);
		}
		m_sock = socket(PF_INET, SOCK_STREAM, 0);
	}
	~CServerSocket() {
		WSACleanup();
	}

	CServerSocket(const CServerSocket& ss) {
		m_sock = ss.m_sock;
		m_client = ss.m_client;
	}

	CServerSocket& operator=(const CServerSocket& ss) {
		if (this != &ss)
		{
			m_sock = ss.m_sock;
			m_client = ss.m_client;
		}
		return *this;
	}



	BOOL InitSockEnv()
	{
		WSADATA data;
		if (WSAStartup(MAKEWORD(1, 1), &data) != 0)
		{
			return FALSE;
		}
		return TRUE;
	}



	static void releaseInstance()
	{
		if (m_instance != NULL) {
			CServerSocket* tmp = m_instance;
			m_instance = NULL;
			delete tmp;
		}
	}
	
	class CHelper
	{
	public:
		CHelper() {
			CServerSocket::getInstance();
		}

		~CHelper() {
			CServerSocket::releaseInstance();
		}

	};

private:
	static CServerSocket* m_instance;
	SOCKET m_sock;
	SOCKET m_client;
	static CHelper m_helper;
	
};



