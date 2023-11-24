/*
    Tree Reader v2.0

    - The latest centrality tree class see: 

    https://github.com/Yme-s-STAR-analysis/CentralityTree

    - Update date: 22.11.2023

    - Author: Yige Huang

*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TF1.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"

#include "CentCorrTool/CentCorrTool.h"

class CentCorrTool;

using std::cout;

class TreeReader{
    private:
        CentCorrTool* corr;
        TChain* chain;
        Int_t nev;

    public:
        Int_t trgid;

        Int_t RefMult;
        Int_t RefMult3;
        Int_t RefMult3X;
        Int_t nTofMatch;
        Int_t nTofMatchZ;
        Int_t nTofBeta;
        Int_t tofMult;
        Double_t vz;
        Double_t zdcx;

        TreeReader(){
            corr = new CentCorrTool();
            chain = 0;
        }

        ~TreeReader(){}

        CentCorrTool* GetCentCorrTool() {
            // use this function to get the corr
            // in outter codes
            return corr;
        }

        void init_reader(TChain* chain_) {
            chain = chain_;
            chain->SetBranchAddress("TriggerId", &trgid);
            chain->SetBranchAddress("refMult", &RefMult);
            chain->SetBranchAddress("refMult3", &RefMult3);
            chain->SetBranchAddress("refMult3X", &RefMult3X);
            chain->SetBranchAddress("nTofMatch", &nTofMatch);
            chain->SetBranchAddress("nTofMatchZ", &nTofMatchZ);
            chain->SetBranchAddress("nTofBeta", &nTofBeta);
            chain->SetBranchAddress("Vz", &vz);
            chain->SetBranchAddress("ZDCx", &zdcx);

            if (chain != 0) {
                nev = chain->GetEntries();
            } else {
                nev = -1;
            }
        }

        Int_t get_entries() {
            return nev;
        }

        void make(Int_t i){
            if (i >= nev) {
                cout << "[WARNING] " << i << " out of range (max entry: " << nev << ").\n";
                return;
            }

            chain->GetEntry(i);
            if (i % 100000 == 0){
                cout << "[LOG] " << i << " / " << nev << " events processed.\n";
            }
        }

        Int_t get_converted_trg() {
            // return the Trigger ID index (for array index use)
            return corr->ConvertTrg(trgid);
        }
};
