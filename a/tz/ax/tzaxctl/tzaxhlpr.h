/////////////////////////////////////////////////////////////////////////////
// COleFont wrapper class

class COleFont : public COleDispatchDriver
{
public:
   COleFont() {}     // Calls COleDispatchDriver default constructor
   COleFont(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   COleFont(const COleFont& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   CString GetName();
   void SetName(LPCTSTR);
   CY GetSize();
   void SetSize(const CY&);
   BOOL GetBold();
   void SetBold(BOOL);
   BOOL GetItalic();
   void SetItalic(BOOL);
   BOOL GetUnderline();
   void SetUnderline(BOOL);
   BOOL GetStrikethrough();
   void SetStrikethrough(BOOL);
   short GetWeight();
   void SetWeight(short);
   short GetCharset();
   void SetCharset(short);

// Operations
public:
};


class CPicture : public COleDispatchDriver
{
public:
   CPicture() {}     // Calls COleDispatchDriver default constructor
   CPicture(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   CPicture(const CPicture& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   long GetHandle();
   long GetHPal();
   void SetHPal(long);
   short GetType();
   long GetWidth();
   long GetHeight();

// Operations
public:
   // method 'Render' not emitted because of invalid return type or parameter type
};
