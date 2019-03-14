#include<iostream>
#include<cstring>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

void get_size(const chat* filename)
{
	int len;
	FILE *fp;
	fp=fopen(filename,"r");
	fseek(fp,0,SEEK_SET);
	len=ftell(fp);
	rewind(fp);
	return len;
}

int main(int argc,char* argv[])
{
	creat(argv[2],0777);
	int size=get_size(argv[2]);
	truncate(argv[2],size);
	char *filein=argv[1];
	char *fileout=argv[2];
	FILE *fin=fopen(filein,"r+");
	FILE *fout=fopen(filein,"w+");
	int len=total=0;
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		fseek(fin,0,SEEK_SET);
		fseek(fout,0,SEEK_SET);
		char buff[1024]={"\0"};
		while((len=fread(buff,1,sizeof(buff),fin))>0)
		{
			fwrite(buff,1,len,fout);
			total+=len;
			if(total>size/2)
			{
				break;
			}
		}
		exit(0);
	}
	else
	{
		fseek(fin,size/2,SEEK_SET);
		fseek(fout,size/2,SEEK_SET);
		char* buff[1024]={"\0"};
		while((len=fread(buff,1,sizeof(buff),fin))>0)
		{
			fwrite(buff,1,len,fout);
			total+=len;
			if(total>size/2)
			{
				break;
			}
		}
		exit(0);
	}
} 
