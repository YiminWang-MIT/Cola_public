{
#include <TFile.h>
   gROOT->Reset();
   //#include "Riostream.h"

   TFile *f = new TFile("analysis.root","RECREATE");
   Float_t x, y, z, th, ph, dp;
   Int_t nlines = 0;   

   // Read data at target

   ifstream Tin;
   Tin.open("tocke.dat");  // CHANGE THIS PATH TO YOUR OWN PATH
   TH1F *his_dp = new TH1F("Particle/dp","Initial Particle distribution over dp in TG",200,-100.0,100.0);
   TH1F *his_y = new TH1F("Particle/y","Initial Particle distribution over y in TG",200,-50.0,50.0);
   TH1F *his_th = new TH1F("Particle/th","Initial Particle distribution over th in TG",200, -120.0,120.0);
   TH1F *his_ph = new TH1F("Particle/ph","Initial Particle distribution over ph in TG",200,-120.0,120.0);
   TNtuple *Tntuple = new TNtuple("Target","Initial particles in TG","dp:y:z:th:ph");

   while (1) {
      Tin >> dp>>y>>z>>th>>ph;
      cout<<"dp: "<<dp<<", y: "<<y<<", z: "<<z<<", th: "<<th<<", ph: "<<ph<<endl;
      if (!Tin.good()) break;
      his_dp->Fill(dp);
      his_y->Fill(y);
      his_th->Fill(th);
      his_ph->Fill(ph);
      Tntuple->Fill(dp,y,z,th, ph);

      nlines++;
   }
   printf(" found %d points\n",nlines);

   Tin.close();

   // Read data for kaons
   ifstream Kin;
   Kin.open("datakaons.dat");  // CHANGE THIS PATH TO YOUR OWN PATH

   TH1F *K_x = new TH1F("K/x","K+ distribution over x in FP",200,-3000.0,4000.0);
   TH1F *K_y = new TH1F("K/y","K+ distribution over y in FP",200,-500.0,500.0);
   TH1F *K_th = new TH1F("K/th","K+ distribution over th in FP",200, 0.0,2000.0);
   TH1F *K_ph = new TH1F("K/ph","K+ distribution over ph in FP",200,-500.0,500.0);
   TNtuple *Kntuple = new TNtuple("Kaons","Kaons in FP","x:y:z:th:ph");

   while (1) {
      Kin >> x>>y>>z>>th>>ph;
      cout<<"x: "<<x<<", y: "<<y<<", z: "<<z<<", th: "<<th<<", ph: "<<ph<<endl;
      if (!Kin.good()) break;
      K_x->Fill(x);
      K_y->Fill(y);
      K_th->Fill(th);
      K_ph->Fill(ph);
      Kntuple->Fill(x,y,z,th, ph);
      nlines++;
   }
   printf(" found %d points\n",nlines);

   Kin.close();


  // Read data for pions

   ifstream PIin;
   PIin.open("datapions.dat");  // CHANGE THIS PATH TO YOUR OWN PATH

   nlines = 0;
   TH1F *pi_x = new TH1F("Pi/x","Pi+ distribution over x in FP",200,-3000.0,4000.0);
   TH1F *pi_y = new TH1F("Pi/y","Pi+ distribution over y in FP",200,-500.0,500.0);
   TH1F *pi_th = new TH1F("Pi/th","Pi+ distribution over th in FP",200, 0.0,2000.0);
   TH1F *pi_ph = new TH1F("Pi/ph","Pi+ distribution over ph in FP",200,-500.0,500.0);
   TNtuple *PIntuple = new TNtuple("Pions","Pions in FP","x:y:z:th:ph");

   while (1) {
      PIin >> x>>y>>z>>th>>ph;
      cout<<"x: "<<x<<", y: "<<y<<", z: "<<z<<", th: "<<th<<", ph: "<<ph<<endl;
      if (!PIin.good()) break;
      pi_x->Fill(x);
      pi_y->Fill(y);
      pi_th->Fill(th);
      pi_ph->Fill(ph);
      PIntuple->Fill(x,y,z,th, ph);
      nlines++;
   }
   printf(" found %d points\n",nlines);

   PIin.close();

  // Read data for muons

   ifstream MUin;
   MUin.open("datamuons.dat");  // CHANGE THIS PATH TO YOUR OWN PATH

   nlines = 0;
   TH1F *mu_x = new TH1F("Mux","Mu+ distribution over x in FP",200,-3000.0,4000.0);
   TH1F *mu_y = new TH1F("Mu/y","Mu+ distribution over y in FP",200,-500.0,500.0);
   TH1F *mu_th = new TH1F("Mu/th","Mu+ distribution over th in FP",200, 0.0,2000.0);
   TH1F *mu_ph = new TH1F("Mu/ph","Mu+ distribution over ph in FP",200,-500.0,500.0);
   TNtuple *MUntuple = new TNtuple("Muons","Muons in FP","x:y:z:th:ph");

   while (1) {
      MUin >> x>>y>>z>>th>>ph;
      cout<<"x: "<<x<<", y: "<<y<<", z: "<<z<<", th: "<<th<<", ph: "<<ph<<endl;
      if (!MUin.good()) break;
      mu_x->Fill(x);
      mu_y->Fill(y);
      mu_th->Fill(th);
      mu_ph->Fill(ph);
      MUntuple->Fill(x,y,z,th,ph);
      nlines++;
   }
   printf(" found %d points\n",nlines);

   MUin.close();

  TH1F *Particles_x = new TH1F("Particles/x","Distribution of particles over x in FP",200,-3000.0,4000.0);
  TH1F *Particles_y = new TH1F("Particles/y","Distribution of particles over y in FP",200,-500.0,500.0);
  TH1F *Particles_th = new TH1F("Particles/th","Distribution of particles over th in FP",200,0.0,2000.0);
  TH1F *Particles_ph = new TH1F("Particles/ph","Distribution of particles over ph in FP",200,-500.0,500.0);

  Particles_x->Add(K_x);
  Particles_x->Add(pi_x);
  Particles_x->Add(mu_x);

  Particles_y->Add(K_y);
  Particles_y->Add(pi_y);
  Particles_y->Add(mu_y);

  Particles_th->Add(K_th);
  Particles_th->Add(pi_th);
  Particles_th->Add(mu_th);

  Particles_ph->Add(K_ph);
  Particles_ph->Add(pi_ph);
  Particles_ph->Add(mu_ph);

  f->Write();

  

  TCanvas* c1 = new TCanvas("c1");
  c1->Divide(2,2,0.001,0.001);
  
  c1->cd(1);
  Particles_x->SetFillColor(kWhite);
  Particles_x->Draw();
  K_x->SetFillColor(kGreen);
  K_x->Draw("same");
  pi_x->SetFillColor(kBlue);
  pi_x->Draw("same");
  mu_x->SetFillColor(kRed);
  mu_x->Draw("same");
 
  c1->cd(2);
  Particles_y->SetFillColor(kWhite);
  Particles_y->Draw();
  K_y->SetFillColor(kGreen);
  K_y->Draw("same");
  pi_y->SetFillColor(kBlue);
  pi_y->Draw("same");
  mu_y->SetFillColor(kRed);
  mu_y->Draw("same");
 
  c1->cd(3);
  Particles_th->SetFillColor(kWhite);
  Particles_th->Draw();
  K_th->SetFillColor(kGreen);
  K_th->Draw("same");
  pi_th->SetFillColor(kBlue);
  pi_th->Draw("same");
  mu_th->SetFillColor(kRed);
  mu_th->Draw("same");
 
  c1->cd(4);
  Particles_ph->SetFillColor(kWhite);
  Particles_ph->Draw();
  K_ph->SetFillColor(kGreen);
  K_ph->Draw("same");
  pi_ph->SetFillColor(kBlue);
  pi_ph->Draw("same");
  mu_ph->SetFillColor(kRed);
  mu_ph->Draw("same");


  TCanvas* c2 = new TCanvas("c2");
  c2->Divide(2,2,0.001,0.001);
  
  c2->cd(1);
  his_dp->Draw();
  c2->cd(2);
  his_y->Draw();
  c2->cd(3);
  his_th->Draw();
  c2->cd(4);
  his_ph->Draw();

}
