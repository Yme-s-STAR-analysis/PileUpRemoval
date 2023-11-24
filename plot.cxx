#include "Conf.h"

void plot(){
    TFile* tf1 = new TFile("pile_up_raw.root");
    TFile* tf2 = new TFile("pile_up_corr.root");
    TH2F* th21;
    TH2F* th22;
    TLine* l = new TLine();
    TCanvas* c = new TCanvas("cas", "", 600, 800);
    TCanvas* c2 = new TCanvas();
    c->Divide(2, 2, 0, 0);
    TF1* tfunc = new TF1("func", "pol3", 0, 800);
    tfunc->SetLineColor(2);
    tfunc->SetLineStyle(2);
    TLatex* lat = new TLatex();
    lat->SetTextSize(0.08);
    gStyle->SetOptStat(0);

    // beta eta 1
    c->cd(1);
    tf1->GetObject("h2Ref3VsBetaEta1", th21);
    th21->SetTitle("Raw;;RefMult3");
    gPad->SetRightMargin(0);
    gPad->SetBottomMargin(0.04);
    gPad->SetLogz();
    th21->Draw("col");
    tfunc->SetParameters(cent_conf::beta_eta1_upper_pars);
    tfunc->DrawClone("lsame");
    tfunc->SetParameters(cent_conf::beta_eta1_lower_pars);
    tfunc->DrawClone("lsame");
    c->cd(2);
    tf2->GetObject("h2Ref3VsBetaEta1", th22);
    th22->SetTitle("Pile up rejected;;");
    gPad->SetLeftMargin(0);
    gPad->SetBottomMargin(0.04);
    gPad->SetLogz();
    th22->Draw("col");
    lat->DrawLatex(150, 100, "Beta_eta1");

    // nTofMatch
    c->cd(3);
    tf1->GetObject("h2Ref3VsNTofMatch", th21);
    th21->SetTitle(";;RefMult3");
    gPad->SetTopMargin(0);
    gPad->SetRightMargin(0);
    gPad->SetLogz();
    th21->Draw("col");
    tfunc->SetParameters(cent_conf::nTofMatch_upper_pars);
    tfunc->DrawClone("lsame");
    tfunc->SetParameters(cent_conf::nTofMatch_lower_pars);
    tfunc->DrawClone("lsame");
    lat->DrawLatex(110, 200, "STAR BES-II");
    lat->DrawLatex(110, 100, "Au+Au @ 17.3");

    c->cd(4);
    tf2->GetObject("h2Ref3VsNTofMatch", th22);
    th22->SetTitle(";;");
    gPad->SetTopMargin(0);
    gPad->SetLeftMargin(0);
    gPad->SetLogz();
    th22->Draw("col");
    lat->DrawLatex(150, 100, "nTofMatch");

    c->Print("pileup_plots.pdf");
    c->Print("pileup_plots.png");

    // refmult3 dist
    c2->cd();
    gPad->SetLogy();
    TH1D* h1 = (TH1D*)tf1->Get("h1RefMult3");
    TH1D* h2 = (TH1D*)tf2->Get("h1RefMult3");
    TH1D* hdiff = (TH1D*)h1->Clone();
    hdiff->Add(h2, -1);
    h1->SetTitle(";RefMult3;Counts");
    h1->SetLineColor(4);
    h2->SetLineColor(2);
    h1->SetMarkerColor(4);
    h2->SetMarkerColor(2);
    h1->Draw("epl");
    h2->Draw("eplsame");

    hdiff->SetMarkerColor(40);
    hdiff->SetLineColor(40);
    hdiff->Draw("eplsame");

    TLegend* leg = new TLegend(0.15, 0.15, 0.43, 0.43);
    leg->AddEntry(h1, "Raw", "epl");
    leg->AddEntry(h2, "Pile-up rejected", "epl");
    leg->AddEntry(hdiff, "Pile up events", "epl");
    leg->Draw();
    lat->SetTextSize(0.06);
    lat->DrawLatex(100, 2e4, "STAR BES-II");
    lat->DrawLatex(100, 2e3, "Au+Au @ 17.3 GeV");
    Double_t ratio = (h1->GetEntries() - h2->GetEntries()) * 100.0 / h1->GetEntries();
    Double_t diff = h1->GetEntries() - h2->GetEntries();
    cout << "Removed events: " << diff << ".\n";
    lat->SetTextSize(0.04);
    lat->DrawLatex(300, 2.3e6, Form("~%.1f %% events removed (%.2e)", ratio, diff));
    c2->Print("hRefMult3.pdf");
    c2->Print("hRefMult3.png");

    tf1->Close();
    tf2->Close();

    return 0;
}
