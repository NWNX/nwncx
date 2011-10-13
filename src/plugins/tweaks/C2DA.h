#ifndef C2DA_h_
#define C2DA_h_

#include <windows.h>
#include "CExoString.h"
#include "CResRef.h"

class C2DA
{
public:
  /* 0x0/0 */ unsigned long __VFN_table;
  /* 0x4/4 */ unsigned long m_bAutoRequest;
  /* 0x8/8 */ unsigned long m_pRes;
  /* 0xC/12 */ CResRef m_cResRef;
  /* 0x1C/28 */ CExoString m_sDefaultValue;
  /* 0x24/36 */ CExoString *m_pRowLabel;
  /* 0x28/40 */ CExoString *m_pColumnLabel;
  /* 0x2C/44 */ CExoString **m_pArrayData;
  /* 0x30/48 */ unsigned long m_nNumRows;
  /* 0x34/52 */ unsigned long m_nNumColumns;
  /* 0x38/56 */ unsigned long m_bLoaded;

  C2DA(CResRef, int);
  int Load2DArray();
  int GetCExoStringEntry(int, CExoString const &, CExoString *);
  
};

#endif