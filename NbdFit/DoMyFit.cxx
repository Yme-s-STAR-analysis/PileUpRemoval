#include <iostream>

#include "TFile.h"
#include "TH2F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TLine.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TLegend.h"

#include "NbdFitCore.h"

int main(int argc, char** argv) {

    std::cout << "[LOG] - From Main: Target root file name: " << argv[1] << std::endl;
    std::cout << "[LOG] - From Main: Target histogram name: " << argv[2] << std::endl;
    auto tf = new TFile(argv[1]);
    TH2F* h2 = (TH2F*)tf->Get(argv[2]);
    TH1F* h1;

    double xMin = h2->GetXaxis()->GetXmin();
    double xMax = h2->GetXaxis()->GetXmax();

    NbdFitCore ff;
    ff.Init(h2, -0.5, 450.5);

    // x1 and x2 is for the range of slices and fit
    // xMin and xMax just indicates the axis range
    int x1 = atoi(argv[3]);
    int x2 = atoi(argv[4]);
    int nBins = atoi(argv[5]);
    double stride = (x2 - x1)*1.0 / nBins;

    std::cout << "[LOG] - From Main: Slice (of X-axis) from " << x1 << " to " << x2 << " with " << nBins << " steps, stride: " << stride << std::endl;

    const char* sname = argv[6];
    std::cout << "[LOG] - From Main: The out put file will named as: " << sname << ".pdf / .png" << std::endl;

    const int nMax = 1000;
    double mult[nMax] = {0.0};
    double mode[nMax] = {0.0};
    double lower[nMax] = {0.0};
    double upper[nMax] = {0.0};

    double upperN = atof(argv[7]);
    double lowerN = atof(argv[8]);

    std::cout << "[LOG] - From Main: The upper cut factor: " << upperN << std::endl;
    std::cout << "[LOG] - From Main: The lower cut factor: " << lowerN << std::endl;

    // double upperN = 3.5;
    // double lowerN = 3.0;

    double m, sig, s;

    TCanvas* c = new TCanvas();

    TLatex* lat = new TLatex();
    lat->SetTextFont(64);
    lat->SetTextSize(24);
    lat->SetTextColor(2);
    lat->SetTextAlign(22);

    TLine* lineL = new TLine();
    TLine* lineR = new TLine();
    TLine* lineM = new TLine();
    lineL->SetLineColor(2);
    lineR->SetLineColor(4);
    lineM->SetLineColor(1);
    lineL->SetLineStyle(2);
    lineR->SetLineStyle(2);
    lineM->SetLineStyle(4);
    lineM->SetLineWidth(3);

    TLegend* leg = new TLegend(0.57, 0.14, 0.87, 0.47);
    leg->SetLineWidth(0);

    gStyle->SetOptStat(0);

    // the cover
    c->Clear();
    c->cd();
    lat->DrawLatexNDC(0.5, 0.516, "Pile-up rejection");
    lat->DrawLatexNDC(0.5, 0.337, argv[2]);
    c->Print(Form("%s.pdf(", sname));

    for (int i=0; i<nBins; i++) {
        ff.DoFit(x1+i*stride, x1+(i+1)*stride);
        mult[i] = x1+(i+0.5)*stride;
        mode[i] = ff.GetMode();
        // lower[i] = ff.GetRightBoundary(lowerN);
        // upper[i] = ff.GetLeftBoundary(upperN);
        upper[i] = ff.GetRightBoundary(upperN);
        lower[i] = ff.GetLeftBoundary(lowerN);
        c->Clear();
        c->cd();
        gPad->SetLogy();
        h1 = ff.GetHist1D();
        h1->GetYaxis()->SetRangeUser(1e-6, 10);
        h1->DrawClone("hist");
        ff.GetFunction()->DrawClone("lsame");
        lineL->DrawLine(upper[i], 1e-6, upper[i], 10);
        lineR->DrawLine(lower[i], 1e-6, lower[i], 10);
        lineM->DrawLine(mode[i], 1e-6, mode[i], 10);
        c->Print(Form("%s.pdf", sname));
    }

    // make the heatmap (with boundary)
    c->Clear();
    c->cd();
    h2->GetXaxis()->SetRangeUser(xMin, xMax);
    h2->Draw("colz");
    gPad->SetLogy(false);
    gPad->SetLogz();
    TGraph* tgMode = new TGraph(nBins, mult, mode);
    TGraph* tgUpper = new TGraph(nBins, mult, upper);
    TGraph* tgLower = new TGraph(nBins, mult, lower);
    tgMode->SetMarkerStyle(24);
    tgUpper->SetMarkerStyle(22);
    tgLower->SetMarkerStyle(23);
    tgMode->SetMarkerColor(4);
    tgUpper->SetMarkerColor(1);
    tgLower->SetMarkerColor(1);
    // tgMode->SetLineColor(1);
    // tgUpper->SetLineColor(2);
    // tgLower->SetLineColor(2);
    tgMode->Draw("psame");
    tgUpper->Draw("psame");
    tgLower->Draw("psame");

    // TF1* funcUpper = new TF1("fUpper", "pol3", h2->GetYaxis()->GetXmin(), h2->GetYaxis()->GetXmax());
    // TF1* funcLower = new TF1("fLower", "pol3", h2->GetYaxis()->GetXmin(), h2->GetYaxis()->GetXmax());
    TF1* funcUpper = new TF1("fUpper", "pol3", 0, x2);
    TF1* funcLower = new TF1("fLower", "pol3", 0, x2);

    tgUpper->Fit(funcUpper, "RNQ");
    tgLower->Fit(funcLower, "RNQ");

    tgUpper->SetLineColor(2);
    tgLower->SetLineColor(2);
    tgUpper->SetLineStyle(2);
    tgLower->SetLineStyle(2);

    funcUpper->Draw("lsame");
    funcLower->Draw("lsame");

    std::cout << "[LOG] - From Main: The fit results: Upper line polynomial(3) parameters are: ";
    std::cout << funcUpper->GetParameter(0) << ", " << funcUpper->GetParameter(1) << ", " << funcUpper->GetParameter(2) << ", " << funcUpper->GetParameter(3) << std::endl;
    std::cout << "[LOG] - From Main: The fit results: Lower line polynomial(3) parameters are: ";
    std::cout << funcLower->GetParameter(0) << ", " << funcLower->GetParameter(1) << ", " << funcLower->GetParameter(2) << ", " << funcLower->GetParameter(3) << std::endl;

    leg->AddEntry(tgMode, "Mode", "p");
    leg->AddEntry(tgUpper, Form("Upper cut: %.1f(1+S)#sigma", upperN), "lp");
    leg->AddEntry(tgLower, Form("Lower cut: %.1f#sigma", lowerN), "lp");

    leg->Draw("same");

    c->Print(Form("%s.pdf", sname));
    c->Print(Form("%s.png", sname));



    c->Clear();
    c->cd();
    lat->DrawLatexNDC(0.5, 0.5, "- END -");
    c->Print(Form("%s.pdf)", sname));




    return 0;

}
