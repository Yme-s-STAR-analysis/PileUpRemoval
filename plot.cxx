#include "CentCorrTool/CentParams.h"

void plot(){
    TFile* tf[3];
    tf[0] = new TFile("pile_up_raw.root");
    tf[1] = new TFile("pile_up_off.root");
    tf[2] = new TFile("pile_up_corr.root");
    TH2F* th[3];
    TLine* l = new TLine();
    TCanvas* c = new TCanvas("cas", "", 1200, 500);
    TCanvas* c2 = new TCanvas();
    c->Divide(3, 1, 0, 0);
    TF1* tfunc = new TF1("func", "pol3", 0, 800);
    tfunc->SetLineColor(2);
    tfunc->SetLineStyle(2);
    TLatex* lat = new TLatex();
    lat->SetTextSize(0.08);
    gStyle->SetOptStat(0);

    const char* hname = "h2BetaRef";

    // nTofBeta
    for (int i=0; i<3; i++) {
        tf[i]->GetObject(hname, th[i]);
        th[i]->SetXTitle("nTofBeta");
    }
    th[0]->SetYTitle("RefMult");
    th[0]->SetTitle("Raw");
    th[1]->SetTitle("w/ Offical nTofMatch cut");
    th[2]->SetTitle("w/ CF's nTofBeta cut");
    th[0]->GetYaxis()->SetTitleOffset(1.4);
    th[0]->GetYaxis()->CenterTitle();

    c->cd(1);
    gPad->SetTopMargin(0);
    gPad->SetLeftMargin(0.2);
    gPad->SetRightMargin(0);
    gPad->SetLogz();
    th[0]->Draw("col");
    lat->DrawLatex(110, 700, "STAR BES-II");
    lat->DrawLatex(110, 600, "Au+Au @ 14.6 GeV");

    c->cd(2);
    gPad->SetLeftMargin(0);
    gPad->SetTopMargin(0);
    gPad->SetRightMargin(0);
    gPad->SetLogz();
    th[1]->Draw("col");
    tfunc->SetParameters(cent_conf::beta_upper_pars);
    tfunc->DrawClone("lsame");
    tfunc->SetParameters(cent_conf::beta_lower_pars);
    tfunc->DrawClone("lsame");
    lat->SetTextColor(2);
    lat->SetTextSize(0.04);
    lat->DrawLatex(300, 100, "nTofBeta Cut lines");
    lat->SetTextColor(1);
    lat->SetTextSize(0.03);
    lat->DrawLatex(110, 730, Form(
        "y > %.2f + %.2f x + %.4e x^{2} + %.4e x^{3}", 
        cent_conf::beta_upper_pars[0],
        cent_conf::beta_upper_pars[1],
        cent_conf::beta_upper_pars[2],
        cent_conf::beta_upper_pars[3]
    ));
    lat->DrawLatex(110, 710, Form(
        "y < %.2f + %.2f x + %.4e x^{2} + %.4e x^{3}", 
        cent_conf::beta_lower_pars[0],
        cent_conf::beta_lower_pars[1],
        cent_conf::beta_lower_pars[2],
        cent_conf::beta_lower_pars[3]
    ));

    c->cd(3);
    gPad->SetLeftMargin(0);
    gPad->SetTopMargin(0);
    gPad->SetRightMargin(0);
    gPad->SetLogz();
    th[2]->Draw("col");

    c->Print("pileup_plots.pdf");
    c->Print("pileup_plots.png");

    // refmult/3/3X dist
    TH1D* h1[3];
    TH1D* hdiff;
    double ratio;
    double diff;
    TLegend* leg = new TLegend(0.15, 0.15, 0.43, 0.43);

    c2->cd();
    gPad->SetLogy();
    for (int i=0; i<3; i++) {
        h1[i] = (TH1D*)tf[i]->Get("h1RefMult");
    }
    hdiff = (TH1D*)h1[0]->Clone();
    hdiff->Add(h1[2], -1);
    h1[0]->SetTitle(";RefMult;Counts");
    h1[0]->SetLineColor(4);
    h1[2]->SetLineColor(2);
    h1[0]->SetMarkerColor(4);
    h1[2]->SetMarkerColor(2);
    h1[0]->Draw("epl");
    h1[2]->Draw("eplsame");

    hdiff->SetMarkerColor(40);
    hdiff->SetLineColor(40);
    hdiff->Draw("eplsame");

    leg->Clear();
    leg->AddEntry(h1[0], "Raw", "epl");
    leg->AddEntry(h1[2], "Pile-up rejected", "epl");
    leg->AddEntry(hdiff, "Pile up events", "epl");
    leg->Draw();
    lat->SetTextSize(0.06);
    lat->DrawLatex(100, 2e4, "STAR BES-II");
    lat->DrawLatex(100, 2e3, "Au+Au @ 14.6 GeV");
    diff = h1[0]->GetEntries() - h1[2]->GetEntries();
    cout << "Removed events: " << diff << ".\n";
    diff = h1[0]->Integral(50, -1) - h1[2]->Integral(50, -1);
    ratio = diff * 100 / h1[0]->Integral(50, -1);
    lat->SetTextSize(0.04);
    lat->DrawLatex(300, 2.3e6, Form("Pile-up level ~%.1f %% ", ratio));
    c2->Print("hRefMult.pdf");
    c2->Print("hRefMult.png");

    c2->Clear();
    c2->cd();
    gPad->SetLogy();
    for (int i=0; i<3; i++) {
        h1[i] = (TH1D*)tf[i]->Get("h1RefMult3");
    }
    hdiff = (TH1D*)h1[0]->Clone();
    hdiff->Add(h1[2], -1);
    h1[0]->SetTitle(";RefMult3;Counts");
    h1[0]->SetLineColor(4);
    h1[2]->SetLineColor(2);
    h1[0]->SetMarkerColor(4);
    h1[2]->SetMarkerColor(2);
    h1[0]->Draw("epl");
    h1[2]->Draw("eplsame");

    hdiff->SetMarkerColor(40);
    hdiff->SetLineColor(40);
    hdiff->Draw("eplsame");

    leg->Clear();
    leg->AddEntry(h1[0], "Raw", "epl");
    leg->AddEntry(h1[2], "Pile-up rejected", "epl");
    leg->AddEntry(hdiff, "Pile up events", "epl");
    leg->Draw();
    lat->SetTextSize(0.06);
    lat->DrawLatex(100, 2e4, "STAR BES-II");
    lat->DrawLatex(100, 2e3, "Au+Au @ 14.6 GeV");
    lat->SetTextSize(0.04);
    lat->DrawLatex(300, 2.3e6, Form("Pile-up level ~%.1f %% ", ratio));
    c2->Print("hRefMult3.pdf");
    c2->Print("hRefMult3.png");

    c2->Clear();
    c2->cd();
    gPad->SetLogy();
    for (int i=0; i<3; i++) {
        h1[i] = (TH1D*)tf[i]->Get("h1RefMult3X");
    }
    hdiff = (TH1D*)h1[0]->Clone();
    hdiff->Add(h1[2], -1);
    h1[0]->SetTitle(";RefMult3X;Counts");
    h1[0]->SetLineColor(4);
    h1[2]->SetLineColor(2);
    h1[0]->SetMarkerColor(4);
    h1[2]->SetMarkerColor(2);
    h1[0]->Draw("epl");
    h1[2]->Draw("eplsame");

    hdiff->SetMarkerColor(40);
    hdiff->SetLineColor(40);
    hdiff->Draw("eplsame");

    leg->Clear();
    leg->AddEntry(h1[0], "Raw", "epl");
    leg->AddEntry(h1[2], "Pile-up rejected", "epl");
    leg->AddEntry(hdiff, "Pile up events", "epl");
    leg->Draw();
    lat->SetTextSize(0.06);
    lat->DrawLatex(100, 2e4, "STAR BES-II");
    lat->DrawLatex(100, 2e3, "Au+Au @ 14.6 GeV");
    lat->SetTextSize(0.04);
    lat->DrawLatex(300, 2.3e6, Form("Pile-up level ~%.1f %% ", ratio));
    c2->Print("hRefMult3X.pdf");
    c2->Print("hRefMult3X.png");

    return;
}
