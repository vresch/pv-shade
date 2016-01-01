//---------------------------------------------------------------------------

#ifndef PVMParamH
#define PVMParamH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPVMParamForm : public TForm
{
__published:	// IDE-managed Components
        TLabeledEdit *IscEdt;
        TLabeledEdit *I0Edt;
        TLabeledEdit *kiEdt;
        TLabeledEdit *T_refEdt;
        TLabeledEdit *AEdt;
        TLabeledEdit *ncellEdt;
        TRadioGroup *RadioGroup1;
        TRadioButton *SiRBtn;
        TRadioButton *GaAsRBtn;
        TRadioButton *CdTeRBtn;
        TButton *OKBtn;
        TLabel *Label1;
        void __fastcall OKBtnClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
        __fastcall TPVMParamForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPVMParamForm *PVMParamForm;
//---------------------------------------------------------------------------
#endif
