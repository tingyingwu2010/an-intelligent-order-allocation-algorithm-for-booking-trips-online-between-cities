#include "WininetHttp.h"
#include <iostream>
#include<sstream>
//#include <json/json.h>
//#pragma comment(lib, "jsoncpp.lib")
#include <fstream>
#pragma comment(lib, "Wininet.lib")
#include <tchar.h>
using namespace std;

string dis, tim;

CWininetHttp::CWininetHttp(void) :m_hSession(NULL), m_hConnect(NULL), m_hRequest(NULL)
{
}

CWininetHttp::~CWininetHttp(void)
{
	Release();
}


//  ͨ��HTTP����Get��Post��ʽ��ȡ��Ϣ [3/14/2017/shike]
const std::string CWininetHttp::RequestJsonInfo(const std::string& lpUrl,
	HttpRequest type/* = Hr_Get*/,
	std::string strHeader/*=""*/,
	std::string strPostData/*=""*/)
{
	std::string strRet = "";
	try
	{
		if (lpUrl.empty())
		{
			throw Hir_ParamErr;
		}
		Release();
		m_hSession = InternetOpen(_T("Http-connect"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);    //�ֲ� ׼������

		if (NULL == m_hSession)
		{
			throw Hir_InitErr;
		}

		INTERNET_PORT port = INTERNET_DEFAULT_HTTP_PORT;
		std::string strHostName = "";
		std::string strPageName = "";

		ParseURLWeb(lpUrl, strHostName, strPageName, port);    //����url,��ȡstrHostName��strPageName  
															   //printf("lpUrl:%s,\nstrHostName:%s,\nstrPageName:%s,\nport:%d\n", lpUrl.c_str(), strHostName.c_str(), strPageName.c_str(), (int)port);

		m_hConnect = InternetConnectA(m_hSession, strHostName.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);							//���� strHostName

		if (NULL == m_hConnect)
		{
			throw Hir_ConnectErr;
		}

		std::string strRequestType;
		if (Hr_Get == type)
		{
			strRequestType = "GET";
		}
		else
		{
			strRequestType = "POST";
		}

		m_hRequest = HttpOpenRequestA(m_hConnect, strRequestType.c_str(), strPageName.c_str(), "HTTP/1.1", NULL, NULL, INTERNET_FLAG_RELOAD, NULL);			//����strPageName
		if (NULL == m_hRequest)
		{
			throw Hir_InitErr;
		}

		DWORD dwHeaderSize = (strHeader.empty()) ? 0 : strlen(strHeader.c_str());
		BOOL bRet = FALSE;
		if (Hr_Get == type)
		{
			bRet = HttpSendRequestA(m_hRequest, strHeader.c_str(), dwHeaderSize, NULL, 0);
		}
		else
		{
			DWORD dwSize = (strPostData.empty()) ? 0 : strlen(strPostData.c_str());
			bRet = HttpSendRequestA(m_hRequest, strHeader.c_str(), dwHeaderSize, (LPVOID)strPostData.c_str(), dwSize);
		}
		if (!bRet)
		{
			throw Hir_SendErr;
		}

		char szBuffer[READ_BUFFER_SIZE + 1] = { 0 };
		DWORD dwReadSize = READ_BUFFER_SIZE;
		if (!HttpQueryInfoA(m_hRequest, HTTP_QUERY_RAW_HEADERS, szBuffer, &dwReadSize, NULL))
		{
			throw Hir_QueryErr;
		}
		if (NULL != strstr(szBuffer, "404"))
		{
			throw Hir_404;
		}

		while (true)
		{
			bRet = InternetReadFile(m_hRequest, szBuffer, READ_BUFFER_SIZE, &dwReadSize);
			if (!bRet || (0 == dwReadSize))
			{
				break;
			}
			szBuffer[dwReadSize] = '\0';
			//cout << szBuffer << endl;
			strRet.append(szBuffer);
		}
		int dID = strRet.find("distance");
		int tID = strRet.find("duration");

		int Fdid = strRet.find(":", dID);
		int Edid = strRet.find(",", Fdid);
		int Dl = Edid - Fdid - 1;

		int Ftid = strRet.find(":", tID);
		int Etid = strRet.find(",", Ftid);
		int Tl = Etid - Ftid - 1;

		dis = strRet.substr(Fdid + 1, Dl);         //����		��
		tim = strRet.substr(Ftid + 1, Tl);         //ʱ��		��
	}
	catch (HttpInterfaceError error)
	{
		m_error = error;
	}
	//return std::move(strRet);
	string spell = dis + ";" + tim;
	return spell;
}


// ����URL��ַ [3/14/2017/shike]
void CWininetHttp::ParseURLWeb(std::string strUrl, std::string& strHostName, std::string& strPageName, WORD& sPort)
{
	sPort = 80;
	string strTemp(strUrl);
	std::size_t nPos = strTemp.find("http://");
	if (nPos != std::string::npos)
	{
		strTemp = strTemp.substr(nPos + 7, strTemp.size() - nPos - 7);
	}

	nPos = strTemp.find('/');
	if (nPos == std::string::npos)    //û���ҵ�
	{
		strHostName = strTemp;
	}
	else
	{
		strHostName = strTemp.substr(0, nPos);
	}

	std::size_t nPos1 = strHostName.find(':');
	if (nPos1 != std::string::npos)
	{
		std::string strPort = strTemp.substr(nPos1 + 1, strHostName.size() - nPos1 - 1);
		strHostName = strHostName.substr(0, nPos1);
		sPort = (WORD)atoi(strPort.c_str());
	}
	if (nPos == std::string::npos)
	{
		return;
	}
	strPageName = strTemp.substr(nPos, strTemp.size() - nPos);
}

// �رվ�� [3/14/2017/shike]
void CWininetHttp::Release()
{
	ReleaseHandle(m_hRequest);
	ReleaseHandle(m_hConnect);
	ReleaseHandle(m_hSession);
}

// �ͷž�� [3/14/2017/shike]
void CWininetHttp::ReleaseHandle(HINTERNET& hInternet)
{
	if (hInternet)
	{
		InternetCloseHandle(hInternet);
		hInternet = NULL;
	}
}


string test(string start, string end, bool flag)
{
	//����http����
	CWininetHttp whttp = CWininetHttp();
	//string url = "http://api.map.baidu.com/direction/v2/driving?origin=" + start + "&destination=" + end + "&ak=ptbL1YLsafmSDlYf8pShFeygdlljtqdr";

	string url = "http://api.map.baidu.com/direction/v2/driving?origin=" + start + "&destination=" + end + "&ak=5EKp6DViGnDg3nx4gvPzMS0OmOpG05rB";
	string json = whttp.RequestJsonInfo(url, Hr_Get, "", "");
	//cout << "����json" << json << endl; 
	char buf[2000];


	if (!flag) {
		double d = atof(dis.c_str()) / 1000.0;
		sprintf(buf, "%f", d);
	}

	else {
		double time = atof(tim.c_str()) / 60.0;
		sprintf(buf, "%f", time);
	}
	return string(buf);
}

vector<string> test2(string start, string end)
{
	//����http����
	CWininetHttp whttp = CWininetHttp();
	//string url = "http://api.map.baidu.com/direction/v2/driving?origin=" + start + "&destination=" + end + "&ak=ptbL1YLsafmSDlYf8pShFeygdlljtqdr";

	string url = "http://api.map.baidu.com/direction/v2/driving?origin=" + start + "&destination=" + end + "&ak=5EKp6DViGnDg3nx4gvPzMS0OmOpG05rB";
	//string url = "http://api.map.baidu.com/directionlite/v1/driving?origin=" + start + "&destination=" + end + "&ak=5EKp6DViGnDg3nx4gvPzMS0OmOpG05rB";  //GET����
	string json = whttp.RequestJsonInfo(url, Hr_Get, "", "");
	//cout << "����json" << json << endl; 
	char buf[2000];

	vector<string> tmpstr;
	double d = atof(dis.c_str()) / 1000.0;							//ǧ��
	sprintf(buf, "%f", d);
	tmpstr.push_back(buf);

	double time = atof(tim.c_str()) / 60.0;							//����
	sprintf(buf, "%f", time);
	tmpstr.push_back(buf);

	return tmpstr;
}


int TotalPointDis1(string P[], int len)
{
	//����http����
	CWininetHttp whttp = CWininetHttp();

	string start = P[0];
	string end = P[len];   //�����յ�
	string mid = "";
	for (int i = 1; i < len; i++) {
		if (i != 1)
			mid.append("|");
		mid.append(P[i]);    //;����
	}

	//string url = "http://api.map.baidu.com/direction/v2/driving?origin=" + start + "&destination=" + end + "&waypoints=" + mid + +"&ak=ptbL1YLsafmSDlYf8pShFeygdlljtqdr";
	string url = "http://api.map.baidu.com/direction/v2/driving?origin=" + start + "&destination=" + end + "&waypoints=" + mid + +"&ak=5EKp6DViGnDg3nx4gvPzMS0OmOpG05rB";
	string json = whttp.RequestJsonInfo(url, Hr_Get, "", "");
	//cout << "����json" << json << endl; 
	return 0;
}

string TotalPointDis(vector<string> P)
{
	//����http����
	CWininetHttp whttp = CWininetHttp();

	int len = P.size() - 1;
	string start = P[0];
	string end = P[len];   //�����յ�
	string mid = "";
	for (int i = 1; i < len; i++) {
		if (i != 1)
			mid.append("|");
		mid.append(P[i]);    //;����
	}

	string url = "http://api.map.baidu.com/direction/v2/driving?origin=" + start + "&destination=" + end + "&waypoints=" + mid + +"&ak=ptbL1YLsafmSDlYf8pShFeygdlljtqdr";
	//string url = "http://api.map.baidu.com/direction/v2/driving?origin=" + start + "&destination=" + end + "&waypoints=" + mid + +"&ak=5EKp6DViGnDg3nx4gvPzMS0OmOpG05rB";
	string json = whttp.RequestJsonInfo(url, Hr_Get, "", "");
	//cout << "����json" << json << endl; 
	return dis;
}
