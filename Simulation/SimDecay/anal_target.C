 {
   #include <TFile.h>
   gROOT->Reset();
   //#include "Riostream.h"
   
   ifstream in;
   in.open("tocke.dat");  // CHANGE THIS PATH TO YOUR OWN PATH

   Float_t dp, y, z, th, ph;
   Int_t nlines = 0;
   TFile *f = new TFile("datatarget.root","RECREATE");
   TH1F *his_dp = new TH1F("Particle/dp","Initial Particle distribution over dp in TG",200,-100.0,100.0);
   TH1F *his_y = new TH1F("Particle/y","Initial Particle distribution over y in TG",200,-50.0,50.0);
   TH1F *his_th = new TH1F("Particle/th","Initial Particle distribution over th in TG",200, -120.0,120.0);
   TH1F *his_ph = new TH1F("Particle/ph","Initial Particle distribution over ph in TG",200,-120.0,120.0);
   TNtuple *ntuple = new TNtuple("Target","Initial particles in TG","dp:y:z:th:ph");

   TH2F *his_dpy = new TH2F("Particle/dpy","2D Initial Particle distribution  over dp vs. y in TG",200,-100.0,100.0, 200, -50.0, 50.0);
   TH2F *his_thph = new TH2F("Particle/thph","2D Initial Particle distribution  over th vs. ph in TG",200,-120.0,120.0, 200, -120.0, 120.0);

   while (1) {
      in >> dp>>y>>z>>th>>ph;
      cout<<"dp: "<<dp<<", y: "<<y<<", z: "<<z<<", th: "<<th<<", ph: "<<ph<<endl;
      if (!in.good()) break;
      his_dp->Fill(dp);
      his_y->Fill(y);
      his_th->Fill(th);
      his_ph->Fill(ph);
      ntuple->Fill(dp,y,z,th, ph);
      his_dpy->Fill(dp,y);
      his_thph->Fill(th,ph);

      nlines++;
   }
   printf(" found %d points\n",nlines);

   in.close();

   f->Write();

  TCanvas* c1 = new TCanvas("c1");
  c1->Divide(2,2,0.001,0.001);
  
  c1->cd(1);
  his_dp->Draw();
  c1->cd(2);
  his_y->Draw();
  c1->cd(3);
  his_th->Draw();
  c1->cd(4);
  his_ph->Draw();

  TCanvas* c2 = new TCanvas("c2");
  c2->Divide(1,2,0.001,0.001);
  c2->cd(1);
  his_dpy->Draw("box");
  c2->cd(2);
  his_thph->Draw("box");

}
