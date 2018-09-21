#include "stdafx.h"
#include "BFileW32.h"

CBFile::CBFile():
m_hfile(0)
{

}

CBFile::~CBFile(void)
{
	if(m_hfile)
		Close();
}

void
CBFile::OpenRead(void)
{
	m_hfile= CreateFile((char*)m_filename, GENERIC_READ, 0, 0, OPEN_EXISTING,0,0);
}


void
CBFile::OpenWrite(void)
{
	m_hfile= CreateFile((char*)m_filename, GENERIC_WRITE, 0, 0, OPEN_ALWAYS,0,0);
	
	SetFilePointer((HANDLE)m_hfile, 0, 0, FILE_END);

}

void
CBFile::Close(void)
{
	::CloseHandle((HANDLE)m_hfile);
	m_hfile=0;
}

unsigned long
CBFile::Read(void* p_buf, unsigned long p_nb)
{
unsigned long dwRead;

	::ReadFile((HANDLE)m_hfile, p_buf, p_nb, &dwRead, NULL);
	return dwRead;
}

unsigned long
CBFile::Write(void* p_buf, unsigned long p_nb)
{
unsigned long dwWritten;

	::WriteFile((HANDLE)m_hfile, p_buf, p_nb, &dwWritten, NULL);
	return dwWritten;
}

void
CBFile::SetFilePath(const char* szFileName)
{
	strcpy((char*)m_filename, szFileName);
}

char*	
CBFile::GetFilePath(void)
{
	return (char *)m_filename;
}

long
CBFile::GetPos(void)
{
	if(m_hfile == 0)
		return -1;

	long dwPos= ::SetFilePointer((HANDLE)m_hfile, 0, 0, FILE_CURRENT);

	
	return dwPos;
}
