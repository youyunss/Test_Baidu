#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>   //域名&&ip

#define MAXBUF 6*1024*1024

using namespace std;

int main(int argc, char **argv)
{
	int sockfd, sendlen, recvlen;
	struct sockaddr_in dest;
	char buf[MAXBUF + 1];
	string m_host = "www.baidu.com";
	string m_data = "/index.html";
	struct hostent *p_hostent = gethostbyname(m_host.c_str());
	char str[32];
	printf(" address: %s\n", inet_ntop(p_hostent->h_addrtype, p_hostent->h_addr, str, sizeof(str)));

	if ((sockfd = socket(AF_INET, 1, 0)) < 0)
	{
		perror("Socket");
		exit(errno);
	}

	printf("socket created\n");
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;					
	dest.sin_port = htons(80);			
	//cout<<dest.sin_addr.s_addr<<endl;
	//dest.sin_addr.s_addr = *(p_hostent->h_addr);
	//cout<<dest.sin_addr.s_addr<<endl;
	//cout<<str<<endl;
	cout<<dest.sin_addr.s_addr<<endl;
	if (inet_aton(str, (struct in_addr*) &dest.sin_addr.s_addr) == 0)			
	{
		perror(str);
		exit(errno);
	}
	cout<<dest.sin_addr.s_addr<<endl;
	if (connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) == -1)
	{
		perror("Connect ");
		exit(errno);
	}
	printf("begin send GET request to www.baidu.com:\n");
	
	string sendData = "GET " + m_data + " HTTP/1.1\r\n"; //如果是POST需要将数据绑在最后发送
	sendData += "Host:" + m_host + "\r\n";
	sendData += "Accept:*/*\r\n";
	sendData += "\r\n";
	sendlen = send(sockfd, sendData.c_str(), sendData.size(), 0);
	if (sendlen <= 0)
	{
		printf("message send failure! errno code is %d, erron message is '%s'\n", errno, strerror(errno));
	}
	else
	{
		cout<<sendData<<endl;
		printf("send message successful, %d bytes send!\n", sendlen);
	}

	cout<<endl<<endl<<endl;
	bzero(buf, MAXBUF + 1);
	recvlen = recv(sockfd, buf, MAXBUF, 0);
	if (recvlen > 0)
	{
		printf("message recv successful :'%s'\n, recv %dByte recv\n", buf, recvlen);
	}
	else if (recvlen < 0)
	{
		printf("recv failure! errno code is %d, errno message is '%s'\n", errno, strerror(errno));
	}
	else
	{
		printf("the other one close quit\n");
	}
	close(sockfd);

	return 0;
}
