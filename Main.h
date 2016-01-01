//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
#include <Forms.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <TeeFunci.hpp>
//---------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <conio>
#include <math.h>
#include <stdio.h>
#include <algorithm.h>

//---------------------------------------------------------------------------
//Declarations
double Isc_ref, I0_ref, T_ref, kI, Esi, A;
const double k = 1.38e-23;
const double q = 1.6e-19;
double Rp = 9;
double Rs = 9.e-4;
int ncell;

double eps = 0.001;

//Параметры уравнения f(i,v)=0
double i2=2.e-5, a1, a2=a1*Rs, a3=1./Rp;   //i2 ->i0

double Isc(double T, double S){
    return (Isc_ref + kI * (T - T_ref)) * S / 1000;}
double I0(double T){
    return I0_ref * (T/T_ref)*(T/T_ref)*(T/T_ref) * exp((q*Esi)/(A*k)) * (1./T_ref - 1./T);}

//Функция f(i,v)=0
double f(double i, double v, double i1){
    return i1+i2-i2*exp(a1*v+a2*i)-a3*v-i;}

//Производная от f(i)
double fdi(double i, double v){
    return -1-a2*i2*exp(a1*v+a2*i);}

//Производная от f(v)
double fdv(double i, double v){
    return -a3-a1*i2*exp(a1*v+a2*i);}

//Вычисление корня уравнения м.Ньютона
double root(double v, double i1)
{
    double a=0, b=i1;  //a - exit condition, b - first value
    double delta=1, i=b, i_;
    while(fabs(delta)>=eps)
        {
        i_=i-f(i,v,i1)/fdi(i,v);
        delta=i_-i;
        i=i_;
        if(i_<a)break;
        }
    return i_;
}

//счетчик - i, неизвестная - v
double vroot(double i, double i1)
{
    double a=-1, b=1; //a - exit condition, b - first value
    double delta=1, v=b, v_;
    while(fabs(delta)>=eps)
        {
        v_=v-f(i,v,i1)/fdv(i,v);
        delta=v_-v;
        v=v_;
        if(v_<a)break;
        }
    return v_;
}

//Series connection V calculation
double vS_func(double i, double isc, int ush, int sh, double liam)
{
    double v, vS_=0, id, i1;
    for(int j=0; j<ush+sh; j++)
        {
      //  i1=isc+(-5+rand()%10+1)*0.01;

        if(j>=ush){v=vroot(i,isc*liam);}
        else v=vroot(i,isc);
      //  i=root(v,i1);
      //  id=i2*(exp(-a1*v)-1);
      //  i=id+i;
      //  v=vroot(-1,3,i,i1);
        if(v<=-0.05){v=-0.05;}
        vS_+=v;
        }
    return ncell*vS_;
}

double iP_func(double v, double *i1, int n)
{
double i, iP_=0, id;
for(int j=0; j<n; j++)
        {
        i=root(v,i1[j]);
      //  id=i2*(exp(-a1*v)-1);
      //  i=id+i;
      //  v=vroot(-1,3,i,i1[j]);
        iP_+=i;
        if(iP_<=-0.1){
        iP_=-0.1;}
        }
return iP_;
}


float *x,*y,*h,*l,*delta,*lambda,*c,*d,*b;
int N;
FILE* InFile=NULL;
FILE* OutFile1=NULL;
FILE* OutFile2=NULL;
FILE* OutFile3=NULL;


//Функция подсчета числа строк в файле
void calc_lines(){
   int nelf=0;
   do{
       nelf = 0;
       while(fgetc(InFile)!='\n' && !feof(InFile)) nelf=1;
       if(nelf) N++;
   }while(!feof(InFile));
   N--;
}
//Ф-ция чтения матрицы из файла
void readmatrix(){
   int i=0;
   for(i=0; i<N+1; i++){
       fscanf(InFile, "%f", &x[i]);
       fscanf(InFile, "%f", &y[i]);
   }
}
//Динамич массивы
void dynarray(){
   x = new float[N+1];  //эксп абсцисса
   y = new float[N+1];  //эксп ордината
   h = new float[N+1];
   l = new float[N+1];
   delta = new float[N+1];
   lambda = new float[N+1];
   c = new float[N+1];
   d = new float[N+1];
   b = new float[N+1];
}


//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TChart *IVChrt;
        TLineSeries *GraphG1IV;
        TLineSeries *GraphG2IV;
        TLineSeries *GraphG3IV;
        TLineSeries *GraphGIV;
        TChart *PVChrt;
        TLineSeries *GraphG1PV;
        TLineSeries *GraphG2PV;
        TLineSeries *GraphG3PV;
        TLineSeries *GraphGPV;
        TButton *CalcBtn;
        TLabel *nModulesLbl;
        TStaticText *nModulesTxt;
        TGroupBox *GenSetBox;
        TLabeledEdit *TempEdt;
        TLabeledEdit *InsolEdt;
        TLabeledEdit *ShadeEdt;
        TGroupBox *GroupBox1;
        TLabeledEdit *nUnshadedEdt1;
        TLabeledEdit *nShadedEdt1;
        TLabeledEdit *nParalEdt1;
        TGroupBox *GroupBox2;
        TLabeledEdit *nUnshadedEdt2;
        TLabeledEdit *nShadedEdt2;
        TLabeledEdit *nParalEdt2;
        TGroupBox *GroupBox3;
        TLabeledEdit *nUnshadedEdt3;
        TLabeledEdit *nShadedEdt3;
        TLabeledEdit *nParalEdt3;
        TButton *PVMParamBtn;
        void __fastcall CalcBtnClick(TObject *Sender);
        void __fastcall nUnshadedEdt1Change(TObject *Sender);
        void __fastcall nShadedEdt1Change(TObject *Sender);
        void __fastcall OnCanResize(TObject *Sender, int &NewWidth,
          int &NewHeight, bool &Resize);
        void __fastcall PVMParamBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif


