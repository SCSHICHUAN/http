
#pragma warning(disable:4996)//��Ҫ��4996����
#pragma comment(lib,"ws2_32.lib")//lib��̬���ӿ�

#include<WinSock2.h>//socket2.2�İ汾
#include<stdio.h>
#include<stdlib.h>

WSADATA wsaData;
SOCKET  s;
SOCKADDR_IN ServerAddr;
int     Ret;
int     Port = 8080;


int main()
{
	//1.���غͳ�ʼ�������
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("WSAStarup faiked with error. %d\n", Ret);
	}

	//2.�����׽���
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("socket filed with error. %d\n", WSAGetLastError());
		WSACleanup();
	}

	//3.��ʼ����
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port); //���ӵĶ˿�
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("116.62.11.154"); // 

  // printf("We are trying to  connect to %s:%d...\n", inet_ntoa(ServerAddr.sin_addr), htons(ServerAddr.sin_port));

																  //4.����
	if ((connect(s, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr))) == SOCKET_ERROR)
	{
		printf("connect failed with error. %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		
	}
	else
	{
		printf("�ͷ��������ӳɹ�!\n");
	}





	//ƴ��http�ַ�����ʽ

	char str1[4096];

	memset(str1, 0, 4096);
	strcat(str1, "POST /mobile/list.do HTTP/1.1\n");  //�����У�  |����ʽ|�ո�|��Ҫ���ʵľ���λ��|�ո�|http�汾|���з���|
	strcat(str1, "Host: 116.62.11.154:8080\n");       //����ͷ��  |keyͷ���ֶ�|ð��|ֵ|���з���|
	strcat(str1, "mobile: 18717791650\n");
	strcat(str1, "password: 123\n");
	strcat(str1, "ContentType: application/x-www-form-urlencoded; charset=UTF-8\n");
	strcat(str1, "Content-Length: 0");
	strcat(str1, "\r\n\r\n");                          //��β��   |�س����лس�����| 











	//5.����
	if ((Ret = send(s, str1, sizeof(str1), 0)) == SOCKET_ERROR)
	{
		printf("Sent failed with error %d.\n", WSAGetLastError());

		
		closesocket(s);
		WSACleanup();
	}
	else
	{
		printf("�Ѿ�����http��ʽ������!\n");
	}



	char DataBuffer[10240];//����ͷ��˷��͵�����
						   //6.�ÿͷ��˵��׽���ȥ����,�ͷ��˵�����,������recv��
	if ((Ret = recv(s, DataBuffer, sizeof(DataBuffer), 0)) == SOCKET_ERROR)
	{
		printf("recv failed with error %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
	}
	else
	{
		wchar_t *pwText = NULL;
		DataBuffer[Ret] = '\0';
		DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, DataBuffer, -1, NULL, 0);    //����ԭʼASCII����ַ���Ŀ       
		pwText = new wchar_t[dwNum];                                              //����ASCII����ַ�������UTF8�Ŀռ�
		MultiByteToWideChar(CP_UTF8, 0, DataBuffer, -1, pwText, dwNum);           //��ASCII��ת����UTF8
		

		int iSize;
		char* pszMultiByte;

		iSize = WideCharToMultiByte(CP_ACP, 0, pwText, -1, NULL, 0, NULL, NULL); //iSize =wcslen(pwsUnicode)+1=6
		pszMultiByte = (char*)malloc(iSize * sizeof(char)); //����Ҫ pszMultiByte = (char*)malloc(iSize*sizeof(char)+1);
		WideCharToMultiByte(CP_ACP, 0, pwText, -1, pszMultiByte, iSize, NULL, NULL);

		printf("http����ɹ�:\n%s", pszMultiByte);

		
	}


	closesocket(s);
	WSACleanup();

	system("pause");
	return 0;

}