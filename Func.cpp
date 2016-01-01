void interpol(char *DatFile)
{
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


    TLineSeries *GraphIV = new TLineSeries(MainForm->IVChrt);
    TLineSeries *GraphPV = new TLineSeries(MainForm->PVChrt);
    GraphIV->Name = "GraphIV" + (String)(MainForm->IVChrt->SeriesList->Count);
    GraphPV->Name = "GraphPV" + (String)(MainForm->PVChrt->SeriesList->Count);
    MainForm->IVChrt->AddSeries(GraphIV);
    MainForm->PVChrt->AddSeries(GraphPV);

    FILE* OutFile=NULL;
    char *filename="dd.dat";
    OutFile=fopen(filename, "wt");
    for(k=0;k<=N;k++){
    fprintf(OutFile, "%.3f %.4f\n", x[k], y[k]);
    }

for(k=0; k<=N; k++){
   GraphIV->AddXY(x[k],y[k], "", clRed);//Эксп точки
   GraphPV->AddXY(x[k],x[k]*y[k], "", clRed);//Эксп точки
   }

   x1=0;
   do{
   for(k=1; k<=N; k++){
   if((x1>=x[k-1])&&(x1<=x[k])) {m=k; x2=x1-x[m];
   break;}
   f=y[m]+b[m]*x2+c[m]*x2*x2+d[m]*x2*x2*x2;
   //Series2->AddXY(x1,f, "", clBlue);}//Сплайн
      x1+=0.001;}
   } while (x1<25);


}