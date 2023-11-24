#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TChain.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TString.h"

#include "tree_reader.h"
#include "./CentCorrTool/CentParams.h"


int main(){
    /*
    
        This src2 is for:
            - check the [Official corrected] RefMult, RefMult3(X) distributions.
            - and the heatmaps between them and "nTofMatch/nTofBeta"
    
    
    */
    using std::cout;
    using std::string;
    using std::vector;
    using std::ifstream;
    vector<string> file_list;
    ifstream fin("file.list");
    string strtmp;
    int n_file = 0;
    while (std::getline(fin, strtmp)){
        n_file ++;
        if (n_file % 500 == 0){
            cout << "[LOG] " << n_file << " files found.\n";
        }
        file_list.push_back(strtmp);
    }
    cout << "[LOG] All " << n_file << " files found.\n";

    TChain* chain = new TChain("centTree");

    Int_t ifile = 0;
    for (auto iter=file_list.begin(); iter!=file_list.end(); iter++){
        ifile ++;
        if (ifile % 500 == 0){
            cout << "[LOG] " << ifile << " trees read.\n";
        }
        chain->Add(iter->c_str());
    }
    cout << "[LOG] All " << ifile << " trees read.\n";

    cout << "[LOG] Preparing to get histograms.\n";
    TreeReader* maker = new TreeReader();
    maker->init_reader(chain);
    Int_t nev = maker->get_entries();

    CentCorrTool* corr = maker->GetCentCorrTool();
    corr->SetDoMatchPileUp(true);
    corr->SetDoBetaPileUp(false);
    corr->SetDoLumi(false);
    corr->SetDoVz(false);
    corr->ReadParams();

    TH1F* h1Ref = new TH1F("h1RefMult", ";RefMult;", 850, -0.5, 849.5);
    TH1F* h1Ref3 = new TH1F("h1RefMult3", ";RefMult3;", 850, -0.5, 849.5);
    TH1F* h1Ref3X = new TH1F("h1RefMult3X", ";RefMult3X;", 1050, -0.5, 1049.5);

    TH2F* h2MatchRef = new TH2F(
        "h2MatchRef", ";nTofMatch;RefMult",
        800, -0.5, 799.5,
        850, -0.5, 849.5
    );
    TH2F* h2MatchRef3 = new TH2F(
        "h2MatchRef3", ";nTofMatch;RefMult3",
        800, -0.5, 799.5,
        850, -0.5, 849.5
    );
    TH2F* h2MatchRef3X = new TH2F(
        "h2MatchRef3X", ";nTofMatch;RefMult3X",
        800, -0.5, 799.5,
        1050, -0.5, 1049.5
    );

    TH2F* h2BetaRef = new TH2F(
        "h2BetaRef", ";nTofBeta;RefMult",
        800, -0.5, 799.5,
        850, -0.5, 849.5
    );
    TH2F* h2BetaRef3 = new TH2F(
        "h2BetaRef3", ";nTofBeta;RefMult3",
        800, -0.5, 799.5,
        850, -0.5, 849.5
    );
    TH2F* h2BetaRef3X = new TH2F(
        "h2BetaRef3X", ";nTofBeta;RefMult3X",
        800, -0.5, 799.5,
        1050, -0.5, 1049.5
    );


    for (Int_t i=0; i<nev; i++) {
        maker->make(i);
        int trg = maker->get_converted_trg();
        if (trg < 0 || trg >= cent_conf::nTrg) {
            continue;
        }
        int vzbin = corr->GetPileUpVzBin(maker->vz);
        if (vzbin < 0) { continue; }
        if (!corr->IsPileUp(maker->RefMult, maker->nTofMatch, maker->nTofBeta, vzbin)) {
            h1Ref->Fill(maker->RefMult);
            h1Ref3->Fill(maker->RefMult3);
            h1Ref3X->Fill(maker->RefMult3X);

            h2MatchRef->Fill(maker->nTofMatch, maker->RefMult);
            h2MatchRef3->Fill(maker->nTofMatch, maker->RefMult3);
            h2MatchRef3X->Fill(maker->nTofMatch, maker->RefMult3X);
            h2BetaRef->Fill(maker->nTofBeta, maker->RefMult);
            h2BetaRef3->Fill(maker->nTofBeta, maker->RefMult3);
            h2BetaRef3X->Fill(maker->nTofBeta, maker->RefMult3X);
        }
    }


    cout << "[LOG] All done, now saving.\n";
    TFile* tfout = new TFile("pile_up_off.root", "recreate");
    tfout->cd();
    h1Ref->Write();
    h1Ref3->Write();
    h1Ref3X->Write();
    h2MatchRef->Write();
    h2MatchRef3->Write();
    h2MatchRef3X->Write();
    h2BetaRef->Write();
    h2BetaRef3->Write();
    h2BetaRef3X->Write();
    tfout->Close();
    cout << "[LOG] This is the end of this workshop.\n";
    return 0;
}