#include <iostream>

#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "TMath.h"

#include "NbdFitCore.h"

void NbdFitCore::Init(TH2F* h2) {
    this->h2 = h2;
    x1 = h2->GetYaxis()->GetBinLowEdge(1);
    x2 = h2->GetYaxis()->GetBinLowEdge(h2->GetNbinsY()+1);
    fNbd = new TF1("MyDoubleNbd", "[0]*ROOT::Math::negative_binomial_pdf(x, [1], [2])+[3]*ROOT::Math::negative_binomial_pdf(x, [4], [5])", x1, x2);
    std::cout << "[LOG] - From NbdFitCore: Read 2D histogram succeed." << std::endl;
    std::cout << "[LOG] - From NbdFitCore: TH2F name is " << h2->GetTitle() << std::endl;
    std::cout << "[LOG] - From NbdFitCore: X-axis is " << h2->GetXaxis()->GetTitle() << std::endl;
    std::cout << "[LOG] - From NbdFitCore: Y-axis is " << h2->GetYaxis()->GetTitle() << std::endl;
    std::cout << "[LOG] - From NbdFitCore: The fit range will be from " << x1 << " to " << x2 << std::endl;
}

void NbdFitCore::Init(TH2F* h2, double x1, double x2) {
    this->h2 = h2;
    this->x1 = x1;
    this->x2 = x2;
    fNbd = new TF1("MyDoubleNbd", "[0]*ROOT::Math::negative_binomial_pdf(x, [1], [2])+[3]*ROOT::Math::negative_binomial_pdf(x, [4], [5])", x1, x2);
    std::cout << "[LOG] - From NbdFitCore: Read 2D histogram succeed." << std::endl;
    std::cout << "[LOG] - From NbdFitCore: TH2F name is " << h2->GetTitle() << std::endl;
    std::cout << "[LOG] - From NbdFitCore: X-axis is " << h2->GetXaxis()->GetTitle() << std::endl;
    std::cout << "[LOG] - From NbdFitCore: Y-axis is " << h2->GetYaxis()->GetTitle() << std::endl;
    std::cout << "[LOG] - From NbdFitCore: The fit range will be from " << x1 << " to " << x2 << std::endl;
}

void NbdFitCore::DoFit(double x1, double x2) {
    h2->GetXaxis()->SetRangeUser(x1, x2);
    h1 = (TH1F*)h2->ProjectionY();
    h1->SetTitle(Form("%s within %.1f to %.1f", h2->GetXaxis()->GetTitle(), x1, x2));
    h1->Scale(1.0 / h1->Integral());

    // initialize parameters
    double p1, r1, p2, r2, a1, a2;
    double m = h1->GetMean();
    double s = h1->GetStdDev();
    p1 = m / s / s;
    r1 = m * p1 / (1 - p1);
    p2 = p1 / 4;
    r2 = m * p2 / (1 - p2);
    // a1 = h1->GetMaximum();
    a1 = 1.0;
    a2 = a1 * 0.0001;
    fNbd->SetParameter(0, a1);
    fNbd->SetParameter(1, p1);
    fNbd->SetParameter(2, r1);
    fNbd->SetParameter(3, a2);
    fNbd->SetParameter(4, p2);
    fNbd->SetParameter(5, r2);
    fNbd->SetParLimits(3, 1e-10, 1e-3);


    h1->Fit(fNbd, "N0Q");


    double mP1 = fNbd->GetParameter(1);
    double mR1 = fNbd->GetParameter(2);

    mMean = mR1*(1-mP1) / mP1;
    mSigma = pow(mR1*(1-mP1)/mP1/mP1, 0.5);
    mSkewness = pow((2-mP1) / pow(mR1*(1-mP1), 0.5), 0.3333);
    mMode = (mR1-1) * (1-mP1) / mP1;
}

double NbdFitCore::GetLeftBoundary(double n) {
    return mMode - mSigma*n;
}

double NbdFitCore::GetRightBoundary(double n) {
    return mMode + (mSkewness+1) * n * mSigma;
}

double NbdFitCore::GetMode() {
    return mMode;
}

/*
float NbdFitCore::GetMean() {
    return fNbd->Mean(x1, x2);
}

float NbdFitCore::GetSigma() {
    return pow(fNbd->CentralMoment(2, x1, x2), 0.5);
}

float NbdFitCore::GetSkewness() {
    return fNbd->CentralMoment(3, x1, x2) / pow(fNbd->CentralMoment(2, x1, x2), 1.5);
}
*/

