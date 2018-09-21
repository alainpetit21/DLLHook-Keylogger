#ifndef _BFILEW32_H_
#define _BFILEW32_H_

class CBFile
{
public:
	explicit	CBFile(void);
	explicit	CBFile(unsigned char p_filename);
				~CBFile(void);

	void			OpenRead(void);
	void 			OpenWrite(void);
	void 			Close(void);
	unsigned long	Read(void* p_buf, unsigned long p_nb);
	unsigned long	Write(void* p_buf, unsigned long p_nb);
	void			SetFilePath(const char* szFileName);
	char*			GetFilePath();
	long			GetPos();

	unsigned char	m_filename[MAX_PATH];
	HANDLE			m_hfile;
};

#endif	//	_BFILEW32_H_