#pragma once


class CAppDoc : public CDocument
{
protected:
	CAppDoc( );
	DECLARE_DYNCREATE( CAppDoc )

public:
	virtual ~CAppDoc( );


public:
	virtual BOOL OnNewDocument( );
	virtual void Serialize( CArchive& ar );

public:
#ifdef _DEBUG
	virtual void AssertValid( ) const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

protected:
	DECLARE_MESSAGE_MAP( )
};
