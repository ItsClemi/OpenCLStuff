#include "stdafx.h"
#include "AppDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CAppDoc, CDocument)
END_MESSAGE_MAP()



CAppDoc::CAppDoc()
{
}

CAppDoc::~CAppDoc()
{
}

BOOL CAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}


void CAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

#ifdef _DEBUG
void CAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

