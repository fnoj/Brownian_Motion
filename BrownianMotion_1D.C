///////////////////////////////////////////////////////////////////////////
//                                                                       //
//  NOTICE OF COPYRIGHT                                                   //
//                                                                       //
//                Copyright (C) 2015   John Suárez                       //
//                https://github.com/fnoj/BuffonsNeedle                  //
//                                                                       //
// This program is free software; you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by  //
// the Free Software Foundation; either version 2 of the License, or     //
// (at your option) any later version.                                   //
//                                                                       //
// This program is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of        //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
// GNU General Public License for more details:                          //
//                                                                       //
//          http://www.gnu.org/copyleft/gpl.html                         //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <TRandom.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

//
TTimer *timep;
TCanvas *c1;
TRandom3 *r1;
TMultiGraph *mg;
TGraph *gr1;
TGraph *gr2;

Int_t a;

Int_t x;
Int_t xf;
Int_t xf2;
Float_t rmin;
Float_t rmax;
Float_t rprom;
Float_t rprom2;
Float_t var;

Int_t N;
Int_t Nbins;

TH1F *H1;

void control(){

  a=1; //Size step
  x=0;
  xf=0;
  xf2=0;

  r1->SetSeed(0);

  timep->TurnOn();

  for(Int_t i=1;i<=N;i++){
    x=a*r1->Uniform(-2,2);
    if(i!=1 && x==0){i--; continue;}
    xf=xf+x;
    xf2=xf2+(x*x);
    gr1->SetPoint(i+1,i+1,xf);
    gr2->SetPoint(i+1,i+1,xf2);
  }
  timep->TurnOff();
  rprom = rprom + xf;
  rprom2 = rprom2 + (xf*xf);
  if(xf>rmax){rmax=xf;}//Limits to Histogram
  if(xf<rmin){rmin=xf;}
}

void BrownianMotion_1D(){

  rmin=0;
  rmax=0;

  c1 = new TCanvas("c1","Brownian Motion",10,10,1000,500);
  c1->Divide(2);

  r1 = new TRandom3;

  timep= new TTimer(0.1);
  timep->Connect("Timeout()","BrownianMotion_1D","","control()");


  N=10000; //Number of Steps
  Nbins=100000; //Number of Repetitions
  rprom=0;

  mg = new TMultiGraph("mg","<x> and <x*x> vs. t");
  gr1 = new TGraph();
  gr2 = new TGraph();
  gr1->SetTitle("<x>");
  gr1->SetLineColor(3);

  gr2->SetTitle("<x*x>");
  gr2->SetLineColor(2);

  mg->Add(gr1);
  mg->Add(gr2);
  gr1->GetYaxis()->SetTitle("<x*x>");

  H1 = new TH1F("Histogram","Brownian Motion",Nbins,-Nbins,Nbins);

  for(Int_t k=0;k<=0;k++){
    for(Int_t j=0;j<Nbins;j++){
      control();
      H1->Fill(xf);
      cout<<j<<endl;
    }
    H1->SetAxisRange(rmin,rmax,"X");
    H1->SetXTitle("Final Position(x)");
    H1->SetYTitle("Frequence");

    cout <<"N:"<<N<<"\t | \t <R>:"<<rprom/Nbins<<"\t | \t <R²>:"<<rprom2/Nbins<<endl;

    c1->cd(1);
    H1->Draw();

    c1->cd(2);
    mg->Draw("AL");
    mg->GetXaxis()->SetTitle("N Steps");
    mg->GetYaxis()->SetTitle("<x> and <x*x>");
    gPad->Modified();
    c1->cd(2)->BuildLegend(0.1,0.7,0.48,0.9);
    N = N +100;
  }
}
