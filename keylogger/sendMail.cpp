#include "stdafx.h"
#include <comdef.h>
#include <iostream>
#include "aosmtp.h"
#include "aosmtp_i.c"
#include "sendmail.h"

using namespace std;

void
SendMail(const char* lpszSenderName,
		 const char* lpszSenderAddr,
		 const char* lpszRecipientName,
		 const char* lpszRecipientAddr,
		 const char* lpszEmailBody,
		 const char* lpszAttachment
		 )
{
	::CoInitialize( NULL );
	IMail* pSmtp = NULL;

	HRESULT hr = ::CoCreateInstance(CLSID_Mail, NULL, CLSCTX_ALL, IID_IMail, (void**)&pSmtp);

	if((pSmtp == NULL) || !SUCCEEDED(hr)){
		cout <<  "error with create ANSMTP instance" << endl;
		return;
	}

	LONG lRet = 0;
	pSmtp->Reset();
	pSmtp->put_ServerAddr(_bstr_t("")); //send email via dns lookup

	pSmtp->put_From(_bstr_t(lpszSenderName));
	pSmtp->put_FromAddr(_bstr_t(lpszSenderAddr));

	pSmtp->put_Subject(_bstr_t("This is a test from ansmtp"));
	pSmtp->put_BodyText(_bstr_t(lpszEmailBody));
	pSmtp->AddRecipient(_bstr_t(lpszRecipientName), _bstr_t(lpszRecipientAddr), 0, &lRet);//0 normal recipient, 1 cc, 2 bcc 
                       

	if(strcmp(lpszAttachment, "") != 0)
		pSmtp->AddAttachment( _bstr_t(lpszAttachment), &lRet);
	

	pSmtp->SendMail(&lRet);

	if(lRet != 0){
		BSTR bOut= NULL;

		pSmtp->GetLastErrDescription(&bOut);

		cout << (const char*)_bstr_t(bOut) << endl;
		::SysFreeString( bOut );
		bOut = NULL;
	}else{
		cout << "Message delivered" << endl;
	}

	pSmtp->Release();
	pSmtp = NULL;
}
