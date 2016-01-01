//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PVMParam.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPVMParamForm *PVMParamForm;
//---------------------------------------------------------------------------
__fastcall TPVMParamForm::TPVMParamForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TPVMParamForm::OKBtnClick(TObject *Sender)
{
    PVMParamForm->Hide();        
}
//---------------------------------------------------------------------------

