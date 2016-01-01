//Simulation of PV Array's V-I & V-P Characteristics under non-uniform conditions
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Main.h"
#include "PVMParam.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
//#include "Func.cpp"
TMainForm *MainForm;

//---------------------------------------------------------------------------

__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{

}

//---------------------------------------------------------------------------

void __fastcall TMainForm::CalcBtnClick(TObject *Sender)
{
//Clear Chart - очистка области графиков - надо заменить на динамическое создание графиков. при многократном пересчете ошибка из-за доступа к пам€ти
    GraphG1IV->Clear();
    GraphG1PV->Clear();
    GraphG2IV->Clear();
    GraphG2PV->Clear();
    GraphG3IV->Clear();
    GraphG3PV->Clear();

//Var Declaration
    const nGrp=3; //кол-во групп с одинаковым частичным затенени€м - сделать динамически
    double ix, vS[nGrp];
    double T, S, liam, isc;
    int nUnshaded[nGrp];
    int nShaded[nGrp];
    int nParal[nGrp];

//PVM Parameters - ѕараметры фотомодул€
    Isc_ref = atof(PVMParamForm->IscEdt->Text.c_str());
    I0_ref = atof(PVMParamForm->I0Edt->Text.c_str());
    T_ref = 273.18 + atof(PVMParamForm->T_refEdt->Text.c_str());
    kI = atof(PVMParamForm->kiEdt->Text.c_str());
    A = atof(PVMParamForm->AEdt->Text.c_str());
    ncell = atoi(PVMParamForm->ncellEdt->Text.c_str());
    if(PVMParamForm->SiRBtn->Checked == true){Esi = 1.12;};
    if(PVMParamForm->GaAsRBtn->Checked == true){Esi = 1.42;};
    if(PVMParamForm->CdTeRBtn->Checked == true){Esi = 1.5;};

//User Settings
    T = 273.18 + atof(TempEdt->Text.c_str());
    S = atof(InsolEdt->Text.c_str());
    liam = atof(ShadeEdt->Text.c_str());
    nUnshaded[0] = atoi(nUnshadedEdt1->Text.c_str());
    nShaded[0] = atoi(nShadedEdt1->Text.c_str());
    nParal[0] = atoi(nParalEdt1->Text.c_str());
    nUnshaded[1] = atoi(nUnshadedEdt2->Text.c_str());
    nShaded[1] = atoi(nShadedEdt2->Text.c_str());
    nParal[1] = atoi(nParalEdt2->Text.c_str());
    nUnshaded[2] = atoi(nUnshadedEdt3->Text.c_str());
    nShaded[2] = atoi(nShadedEdt3->Text.c_str());
    nParal[2] = atoi(nParalEdt3->Text.c_str());

    isc = Isc(T,S);
    a1=q/(A*k*T);
//    double i0 = I0(T); //не работает

    if(nUnshaded[0]+nShaded[0]==nUnshaded[1]+nShaded[1] && nUnshaded[0]+nShaded[0]==nUnshaded[2]+nShaded[2] || nParal[0]==0 || nParal[1]==0 || nParal[2]==0){
    //проверка на соответствие по напр€жению (одинаковое количество модулей в ветке)
        OutFile1=fopen("G1.dat", "wt"); //открытие файла дл€ вывода данных в файл
        OutFile2=fopen("G2.dat", "wt");
        OutFile3=fopen("G3.dat", "wt");
        for(ix=-1;ix<isc+0.05;ix+=0.001){
            for(int j=0;j<nGrp;j++){
                //if(nParal[j]>0){
                    vS[j]=vS_func(ix, isc, nUnshaded[j], nShaded[j], liam);
                    //fprintf(OutFile, "%.3f %.4f\n", ix, vS[j]);
                //}else{fprintf(OutFile, "Group is disconnected from the network");}
            }

            fprintf(OutFile1, "%.3f %.4f\n", ix, vS[0]);
            fprintf(OutFile2, "%.3f %.4f\n", ix, vS[1]);
            fprintf(OutFile3, "%.3f %.4f\n", ix, vS[2]);

            GraphG1IV->AddXY(vS[0],nParal[0]*ix, "", clRed);
            GraphG1PV->AddXY(vS[0],nParal[0]*ix*vS[0], "", clRed);
            GraphG2IV->AddXY(vS[1],nParal[1]*ix, "", clBlue);
            GraphG2PV->AddXY(vS[1],nParal[1]*ix*vS[1], "", clBlue);
            GraphG3IV->AddXY(vS[2],nParal[2]*ix, "", clGreen);
            GraphG3PV->AddXY(vS[2],nParal[2]*ix*vS[2], "", clGreen);
        }
    }

   char *DatFile="G1.dat";
   int k=0, m=0;
   double x1,x2,f;
   do{
   InFile = fopen(DatFile, "rt");
   }while(InFile==NULL);

   calc_lines();
   rewind(InFile);
   dynarray();
   readmatrix();

   for(k=1; k<=N; k++){
       h[k] = x[k] - x[k-1];
       if(h[k]==0){
	   printf("\nError, x[%d]=x[%d]\n", k, k-1);
	   return;
       }
       l[k] = (y[k] - y[k-1])/h[k];
   }
   delta[1] = - h[2]/(2*(h[1]+h[2]));
   lambda[1] = 1.5*(l[2] - l[1])/(h[1]+h[2]);
   for(k=3; k<=N; k++){
      delta[k-1] = - h[k]/(2*h[k-1] + 2*h[k] + h[k-1]*delta[k-2]);
      lambda[k-1] = (3*l[k] - 3*l[k-1] - h[k-1]*lambda[k-2]) /
		    (2*h[k-1] + 2*h[k] + h[k-1]*delta[k-2]);
   }
   c[0] = 0;
   c[N] = 0;
   for(k=N; k>=2; k--){
      c[k-1] = delta[k-1]*c[k] + lambda[k-1];
   }
   for(k=1; k<=N; k++){
      d[k] = (c[k] - c[k-1])/(3*h[k]);
      b[k] = l[k] + (2*c[k]*h[k] + h[k]*c[k-1])/3;
   }

 /* ƒинамическое создание графиков
    TLineSeries *GraphIV = new TLineSeries(MainForm->IVChrt);
    TLineSeries *GraphPV = new TLineSeries(MainForm->PVChrt);
    GraphIV->Name = "GraphIV" + (String)(MainForm->IVChrt->SeriesList->Count);
    GraphPV->Name = "GraphPV" + (String)(MainForm->PVChrt->SeriesList->Count);
    MainForm->IVChrt->AddSeries(GraphIV);
    MainForm->PVChrt->AddSeries(GraphPV);
 */

/*for(k=0; k<=N; k++){
   GraphIV->AddXY(x[k],y[k], "", clRed);//Ёксп точки
   GraphPV->AddXY(x[k],x[k]*y[k], "", clRed);//Ёксп точки
   }
*/

/*
    FILE* OutFile=NULL;
    char *filename="experSpline.dat";
    OutFile=fopen(filename, "wt");
   x1=0;
   do{
      for(k=1; k<=N; k++){
         if((x1>=x[k-1])&&(x1<=x[k])) {m=k; x2=x1-x[m];
            break;
         }
         f=y[m]+b[m]*x2+c[m]*x2*x2+d[m]*x2*x2*x2;
         fprintf(OutFile, "%.3f %.4f\n", x1, f);
         GraphIV->AddXY(x1,f, "", clBlack);//—плайн
         x1+=0.001;
      }
   }while (x1<250);
  */
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::nUnshadedEdt1Change(TObject *Sender)
{
char buffer[3];
nModulesTxt->Caption=itoa(
atoi(nUnshadedEdt1->Text.c_str())+
atoi(nShadedEdt1->Text.c_str()), buffer, 10);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::nShadedEdt1Change(TObject *Sender)
{
char buffer[3];
nModulesTxt->Caption=itoa(
atoi(nUnshadedEdt1->Text.c_str())+
atoi(nShadedEdt1->Text.c_str()), buffer, 10);
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::OnCanResize(TObject *Sender, int &NewWidth,
      int &NewHeight, bool &Resize)
{
IVChrt->Left = 140;
IVChrt->Align = alTop && alRight;
IVChrt->Width = MainForm->ClientWidth - IVChrt->Left;
IVChrt->Height = MainForm->ClientHeight/2;

PVChrt->Left = 140;
PVChrt->Top =  MainForm->ClientHeight/2;
PVChrt->Align = alBottom && alRight;
PVChrt->Width = MainForm->ClientWidth - PVChrt->Left;
PVChrt->Height = MainForm->ClientHeight/2;
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::PVMParamBtnClick(TObject *Sender)
{
    PVMParamForm->Show();        
}
//---------------------------------------------------------------------------

