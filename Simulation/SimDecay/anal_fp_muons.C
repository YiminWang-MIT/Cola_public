 {
   #include <TFile.h>
   gROOT->Reset();
   //#include "Riostream.h"
   
   ifstream in;
   in.open("datamuons.dat");  // CHANGE THIS PATH TO YOUR OWN PATH

   Float_t x, y, z, th, ph;
   Int_t nlines = 0;
   TFile *f = new TFile("datamuons.root","RECREATE");
   TH1F *mu_x = new TH1F("Mu/x","Mu+ distribution over x in FP",200,-3000.0,4000.0);
   TH1F *mu_y = new TH1F("Mu/y","Mu+ distribution over y in FP",200,-500.0,500.0);
   TH1F *mu_th = new TH1F("Mu/th","Mu+ distribution over th in FP",200, 0.0,2000.0);
   TH1F *mu_ph = new TH1F("Mu/ph","Mu+ distribution over ph in FP",200,-500.0,500.0);
   TNtuple *ntuple = new TNtuple("Muons","Muons in FP","x:y:z:th:ph");

   TH2F *his_xy = new TH2F("Mu/xy","2D Mu+ distribution x vs. y in FP",200,-3000.0,4000.0, 200, -500.0, 500.0);
   TH2F *his_thph = new TH2F("Mu/thph","2D Mu+ distribution th vs. ph in FP",200,0.0,2000.0, 200, -500.0, 500.0);

   while (1) {
      in >> x>>y>>z>>th>>ph;
      cout<<"x: "<<x<<", y: "<<y<<", z: "<<z<<", th: "<<th<<", ph: "<<ph<<endl;
      if (!in.good()) break;
      mu_x->Fill(x);
      mu_y->Fill(y);
      mu_th->Fill(th);
      mu_ph->Fill(ph);
      ntuple->Fill(x,y,z,th, ph);

      his_xy->Fill(x,y);
      his_thph->Fill(th,ph);

      nlines++;
   }
   printf(" found %d points\n",nlines);

   in.close();

   f->Write();

  TCanvas* c1 = new TCanvas("c1");
  c1->Divide(2,2,0.001,0.001);
  
  c1->cd(1);
  mu_x->Draw();
  c1->cd(2);
  mu_y->Draw();
  c1->cd(3);
  mu_th->Draw();
  c1->cd(4);
  mu_ph->Draw();

  TCanvas* c2 = new TCanvas("c2");
  c2->Divide(1,2,0.001,0.001);
  c2->cd(1);
  his_xy->Draw("box");
  c2->cd(2);
  his_thph->Draw("box");


}
