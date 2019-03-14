#include<iostream>
#include<cstring>
#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

const int MAX_THREADS=5;

typedef struct TAG_INFO
{
	char *fromfile;
	char *tofile;
	int num;
}info;

void* get_size(const char* filename)
{
	struct stat st;
	memset(&st,0,sizeof(st));
	stat(filename,&st);
	return st.st_size;
} 

void* threadDl(void* param)
{
	info info1=*((info*)param);
	FILE *fin=fopen(info1.fromfile,"r+");
	FILE *fout=fopen(info1.tofile,"w+");
	int size=get_size(info1.fromfile);
	fseek(fin,size*(info1.num)/(MAX_THREADS),SEEK_SET);
	fseek(fout,size*(info1.num)/(MAX_THREADS),SEEK_SET);
	char buff[1024]={"\0"};
	int len=total=0;
	while((len=fread(buff,1,sizeof(buff),fin))>0)
	{
		fwrite(buff,1,len,fout);
		total+=len;
		if(total>size/MAX_THREADS)
		{
			break;
		}
	}
	fclose(fin);
	fclose(fout);
}

int main(int argc,chat* argv[])
{
	creat(argv[2],0777);
	truncate(argv[2],get_size(argv[1]));
	pthread_t pid[MAX_THREADS];
	info info1;
	for(int i=0;i<MAX_THREADS;i++)
	{
		memset(&info1,0,sizeof(info1));
		info1.fromfile=argv[1];
		info1.tofile=argv[2];
		info1.num=i;
		pthread_creat(&pid[i],NULL,threadDL,(void*)&info1); 
	} 
	for(int j=0;j<MAX_THREADS;j++)
	{
		pthread_join(pid[j],NULL);
	} 
	return 0;
}















