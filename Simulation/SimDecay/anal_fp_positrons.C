 {
   #include <TFile.h>
   gROOT->Reset();
   //#include "Riostream.h"
   
   ifstream in;
   in.open("positrons.dat");  // CHANGE THIS PATH TO YOUR OWN PATH

   Float_t x, y, z, th, ph;
   Int_t nlines = 0;
   TFile *f = new TFile("datapositrons.root","RECREATE");
   TH1F *e_x = new TH1F("e/x","e+ distribution over x in FP",200,-3000.0,4000.0);
   TH1F *e_y = new TH1F("e/y","e+ distribution over y in FP",200,-250.0,250.0);
   TH1F *e_th = new TH1F("e/th","e+ distribution over th in FP",200, 0.0,2000.0);
   TH1F *e_ph = new TH1F("e/ph","e+ distribution over ph in FP",200,-300.0,300.0);
   TNtuple *ntuple = new TNtuple("Pions","Pions in FP","x:y:z:th:ph");

   TH2F *his_xy = new TH2F("e/xy","2D e+ distribution x vs. y in FP",200,-3000.0,4000.0, 200, -500.0, 500.0);
   TH2F *his_thph = new TH2F("e/thph","2D e+ distribution th vs. ph in FP",200,0.0,2000.0, 200, -500.0, 500.0);


   while (1) {
      in >> x>>y>>z>>th>>ph;
      cout<<"x: "<<x<<", y: "<<y<<", z: "<<z<<", th: "<<th<<", ph: "<<ph<<endl;
      if (!in.good()) break;
      e_x->Fill(x);
      e_y->Fill(y);
      e_th->Fill(th);
      e_ph->Fill(ph);
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
  e_x->Draw();
  c1->cd(2);
  e_y->Draw();
  c1->cd(3);
  e_th->Draw();
  c1->cd(4);
  e_ph->Draw();

  TCanvas* c2 = new TCanvas("c2");
  c2->Divide(1,2,0.001,0.001);
  c2->cd(1);
  his_xy->Draw("box");
  c2->cd(2);
  his_thph->Draw("box");

}
